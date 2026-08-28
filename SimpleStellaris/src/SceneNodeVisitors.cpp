#include "SceneNodeVisitors.h"
#include "ECSGame.h"
#include "Components.h"
#include "CommonGameCode.h"
#include "EntitiesFunctions.h"
#include <random>
#include <iostream>
#include <memory>
#include <SFML/Audio.hpp>
#include "SpaceObjectTypes.h"

//Check if entity reached target position or not
bool ReachedTargetPosition(const sf::Vector2f& currentPosition, const sf::Vector2f& targetPosition, const sf::Vector2f& velocity)
{
    bool closeByY{ false };
    bool closeByX{ false };
    //Check by y axis
    if (velocity.y == 0.f)
        closeByY = true;
    else if (velocity.y < 0.f)
    {
        if (currentPosition.y < targetPosition.y)
            closeByY = true;
    }
    else
    {
        if (currentPosition.y > targetPosition.y)
            closeByY = true;
    }

    //Check by x axis
    if (velocity.x == 0.f)
        closeByX = true;
    else if (velocity.x < 0.f)
    {
        if (currentPosition.x < targetPosition.x)
            closeByX = true;
    }
    else
    {
        if (currentPosition.x > targetPosition.x)
            closeByX = true;
    }

    //Return result by y and x axis
    return closeByY && closeByX;
}

void SceneNodeVisitorMovement::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Get deltatime
    float dt = ECSGame::Instance().GetDeltaTime();
    //Check if pointer not null
    if (spEntity != nullptr)
    {
        //std::cout << dt << ") " << spEntity->GetName();
        //Check if entity has a movement component
        if (spEntity->HasComponent<MovementComponent>())
        {
            //std::cout << " has " << " movement com" << '\n';
            //If yes, then check which other component it has
            if (spEntity->HasComponent<CameraComponent>())
            {
                //If it has camera component then move camera, not entity
                std::shared_ptr<CameraComponent> spCameraCom = spEntity->FindComponent<CameraComponent>().lock();
                
                bool moveCamera = true;
                if (spCameraCom->cameraLocked) 
                {
                    std::weak_ptr<SceneNode> wpPlanetPic = spCameraCom->wpNodeLockedOn.lock()->FindChild("PlanetPicture");
                    if(wpPlanetPic.lock()!=nullptr)
                        spCameraCom->view.setCenter(wpPlanetPic.lock()->GetCombinedPosition());
                    else
                        spCameraCom->view.setCenter(spCameraCom->wpNodeLockedOn.lock()->GetCombinedPosition());
                    //std::cout << "Locked on: " << spCameraCom->wpNodeLockedOn.lock()->GetEntity().lock()->GetName() << '\n';
                
                    //Check that camera do not go out of bounds
                    sf::Vector2f camCenter = spCameraCom->view.getCenter();
                    float moveX{ 0.f };
                    float moveY{ 0.f };
                    if (camCenter.x + (spCameraCom->view.getSize().x / 2.f) >= spCameraCom->horizontalBorders.y)
                        moveX = -(camCenter.x + (spCameraCom->view.getSize().x / 2.f) - spCameraCom->horizontalBorders.y);
                    else if (camCenter.x - (spCameraCom->view.getSize().x / 2.f) <= spCameraCom->horizontalBorders.x)
                        moveX = -(camCenter.x + moveX - (spCameraCom->view.getSize().x / 2.f) - spCameraCom->horizontalBorders.x);

                    if (camCenter.y + (spCameraCom->view.getSize().y / 2.f) >= spCameraCom->verticalBorders.y)
                        moveY = -(camCenter.y + moveY + (spCameraCom->view.getSize().y / 2.f) - spCameraCom->verticalBorders.y);
                    else if (camCenter.y - (spCameraCom->view.getSize().y / 2.f) <= spCameraCom->verticalBorders.x)
                        moveY = -(camCenter.y + moveY - (spCameraCom->view.getSize().y / 2.f) - spCameraCom->verticalBorders.x);

                    spCameraCom->view.move({ moveX, moveY });
                    moveCamera = false;
                }

                if (spCameraCom->moveCamera && moveCamera)
                {
                    //Check if we can move camera or not
                    //Get movement component
                    std::shared_ptr<MovementComponent> spMovementCom = spEntity->FindComponent<MovementComponent>().lock();

                    float speedMultiplier = spCameraCom->currentZoom * spCameraCom->speedChange;
                    //Move camera
                    sf::Vector2f previousPos = spCameraCom->view.getCenter();
                    sf::Vector2f newPos = previousPos;
                    float moveX = spMovementCom->velocity.x * speedMultiplier * movementSystem.direction.x * dt;
                    float moveY = spMovementCom->velocity.y * speedMultiplier * movementSystem.direction.y * dt;
                    //sf::Vector2f rightBottomCorner{ previousPos.x + (spCameraCom->view.getSize().x / 2.f) , previousPos.y + (spCameraCom->view.getSize().y / 2.f) };
                    if (previousPos.x + moveX + (spCameraCom->view.getSize().x / 2.f) <= spCameraCom->horizontalBorders.y && moveX > 0)
                        newPos.x += moveX;
                    else if (previousPos.x + moveX - (spCameraCom->view.getSize().x / 2.f) >= spCameraCom->horizontalBorders.x && moveX < 0)
                        newPos.x += moveX;

                    if (previousPos.y + moveY + (spCameraCom->view.getSize().y / 2.f) <= spCameraCom->verticalBorders.y && moveY > 0)
                        newPos.y += moveY;
                    else if (previousPos.y + moveY - (spCameraCom->view.getSize().y / 2.f) >= spCameraCom->verticalBorders.x && moveY < 0)
                        newPos.y += moveY;

                    spCameraCom->view.setCenter(newPos);
                    //spCameraCom->view.setCenter({ gel::clamp((spMovementCom->velocity.x * speedMultiplier * movementSystem.direction.x * dt) + spCameraCom->view.getCenter().x, spCameraCom->horizontalBorders.x,spCameraCom->horizontalBorders.y), gel::clamp((spMovementCom->velocity.y * speedMultiplier * movementSystem.direction.y * dt) + spCameraCom->view.getCenter().y, spCameraCom->verticalBorders.x,spCameraCom->verticalBorders.y) });
                }
            }
            else if (spEntity->HasComponent<UIPartComponent>())
            {
                //Get components
                std::shared_ptr<UIPartComponent> spTextCom = spEntity->FindComponent<UIPartComponent>().lock();
                std::shared_ptr<MovementComponent> spMovementCom = spEntity->FindComponent<MovementComponent>().lock();

                //Check if it have special movement logic
                if (spTextCom->moveIt) 
                {
                    sf::Transformable transformable = spEntity->GetTransformable();
                    sf::Vector2f position = transformable.getPosition();

                    //Move UIComponent
                    position.x += spMovementCom->velocity.x * dt;
                    position.y += spMovementCom->velocity.y * dt;
                    //Check if it reached target position
                    if (ReachedTargetPosition(position, spTextCom->targetPosition,spMovementCom->velocity))
                    {
                        //If it reached target position, then destroy it if needed
                        //or just stop moving this entity
                        if (spTextCom->destroyAtTarget)
                        {
                            //signals::onDeleteEntity(spEntity, );
                        }
                        else
                        {
                            position.x = spTextCom->targetPosition.x;
                            position.y = spTextCom->targetPosition.y;
                            spTextCom->moveIt = false;
                            spMovementCom->velocity = { 0.f,0.f };
                        }
                    }

                    //Apply movement
                    transformable.setPosition(position);
                    spEntity->SetTransformable(transformable);
                }
            }
            else 
            {
                //Get component
                std::shared_ptr<MovementComponent> spMovementCom = spEntity->FindComponent<MovementComponent>().lock();

                //Move it
                sf::Transformable transformable = spEntity->GetTransformable();
                sf::Vector2f position = transformable.getPosition();
                position.x += spMovementCom->velocity.x * dt;
                position.y += spMovementCom->velocity.y * dt;

                //Apply movement
                transformable.setPosition(position);
                spEntity->SetTransformable(transformable);
            }
        }
    }
}


//UI processing function
void SceneNodeVisitorUI::ProcessNode(SceneNode& node) 
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Get deltatime
    float dt = ECSGame::Instance().GetDeltaTime();
    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has UI component
        if (spEntity->HasComponent<UIPartComponent>())
        {
            //Get UI component
            std::shared_ptr<UIPartComponent> spEntityText = spEntity->FindComponent<UIPartComponent>().lock();

            //Check if this text blinks or not
            if (spEntityText->isBlinking) 
            {
                //Process blink time
                spEntityText->blinkTime += dt;
                //Check if it is flatline or not
                if(spEntityText->flatLine)
                {
                    if (spEntityText->blinkTime > spEntityText->flatLinePeriod)
                    {
                        //If flat period finishd than start change visibility
                        spEntityText->blinkTime -= spEntityText->flatLinePeriod;
                        spEntityText->flatLine = false;
                    }
                }
                else
                {
                    if (spEntityText->blinkTime > spEntityText->blinkingPeriod)
                    {
                        //If blinkPeriod finished then show same visibility for some
                        //period of time
                        spEntityText->decreasingVisibility = !spEntityText->decreasingVisibility;
                        spEntityText->blinkTime -= spEntityText->blinkingPeriod;
                        spEntityText->flatLine = true;
                    }
                    //Get component
                    std::shared_ptr<TextComponent> spEntityTextCom = spEntity->FindComponent<TextComponent>().lock();

                    //Get color values
                    sf::Color color = spEntityTextCom->text->getFillColor();
                    sf::Color outlineColor = spEntityTextCom->text->getOutlineColor();
                    uint8_t alphaValue{255};
                    //Check which state the text is and get correct alpha value
                    if (spEntityText->decreasingVisibility)
                        alphaValue = (uint8_t)gel::linearInterpolation(255, 90, spEntityText->blinkTime / spEntityText->blinkingPeriod);
                    else
                        alphaValue = (uint8_t)gel::linearInterpolation(90, 255, spEntityText->blinkTime / spEntityText->blinkingPeriod);

                    //Set new alphaValue
                    color.a = alphaValue;
                    outlineColor.a = alphaValue;
                    spEntityTextCom->text->setFillColor(color);
                    spEntityTextCom->text->setOutlineColor(outlineColor);
                }
            }

            //Check if entity has UIFollower component
            if (spEntity->HasComponent<UIFollowerComponent>())
            {
                std::shared_ptr<UIFollowerComponent> spEntityFollower = spEntity->FindComponent<UIFollowerComponent>().lock();

                bool hide = false;
                if (spEntityFollower->nodeToFollow.lock() == nullptr)
                    hide = true;
                else if (spEntityFollower->nodeToFollow.lock()->GetEntity().lock() == nullptr)
                    hide = true;
                else if (spEntityFollower->nodeToFollow.lock()->GetEntity().lock()->hidden)
                    hide = true;
                else if (spEntityFollower->hideAnyway)
                    hide = true;
                else if (spEntityFollower->hideIfZoomLargeEnough) 
                {
                    if (spCamCom->currentZoom > spEntityFollower->zoomLevelsAtWhichHideEntity.y)
                        hide = true;
                    else 
                        hide = false;
                }
                else if (spEntityFollower->hideIfZoomSmallEnough)
                {
                    if (spCamCom->currentZoom < spEntityFollower->zoomLevelsAtWhichHideEntity.x)
                        hide = true;
                    else
                        hide = false;
                }

                if (hide)
                    spEntity->hidden = true;
                else if(spEntityFollower->hideIfOutsideOfCamera && !IsWorldPosInsideOfCamera(spCamCom, spEntityFollower->nodeToFollow.lock()->GetCombinedPosition()))
                    spEntity->hidden = true;
                else
                {
                    spEntity->hidden = false;
                    sf::Vector2f positionToFollow = spEntityFollower->nodeToFollow.lock()->GetCombinedPosition();
                    sf::Vector2i convertedPosition = ConvertWorldPositionToWindow(spCamCom->view, positionToFollow);
                    spEntity->SetPosition({ (float)convertedPosition.x, (float)convertedPosition.y });
                }
            }

            //Check if entity has OrbitVisualizer component
            if (spEntity->HasComponent<OrbitVisualizerComponent>())
            {
                std::shared_ptr<OrbitVisualizerComponent> spOrbitVisualizer = spEntity->FindComponent<OrbitVisualizerComponent>().lock();
                spOrbitVisualizer->orbitShape.setRadius(static_cast<float>(spOrbitVisualizer->orbitSize*(static_cast<double>(spUICamCom->view.getSize().y) / static_cast<double>(spCamCom->view.getSize().y))));
                //std::cout << "Radius: " << spOrbitVisualizer->orbitShape.getRadius()<<'\n';
                spOrbitVisualizer->orbitShape.setOrigin(sf::Vector2f{ spOrbitVisualizer->orbitShape.getRadius(), spOrbitVisualizer->orbitShape.getRadius() });
            }
        }
    }
}



//System visibility processing function
void SceneNodeVisitorSystemVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has object system component
        if (spEntity->HasComponent<ObjectSystemComponent>())
        {
            if (IsWorldPosInsideOfCamera(spCamCom, spEntity->GetPosition()))
                spEntity->hidden = false;
            else
                spEntity->hidden = true;
        }
    }
}



void SceneNodeVisitorMoveObjectsInSystem::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //std::cout << spEntity->GetName()<<'\n';
        //Check if entity has star component
        if (spEntity->HasComponent<StarComponent>())
        {
            //Get star component
            std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();

            //Move star
            double rotation = (spStarCom->rotationalVelocity * static_cast<double>(ECSGame::Instance().GetDaysPast())) + spStarCom->initialRotationPosition;
            //std::cout << "Double: " << std::sin(rotation) * spStarCom->orbitRadius << '\n';
            spEntity->SetPosition(sf::Vector2f(static_cast<float>(std::sin(rotation) * spStarCom->orbitRadius), static_cast<float>(std::cos(rotation) * spStarCom->orbitRadius)));
            //std::cout <<"Time: " << ECSGame::Instance().GetDaysPast() << "; Double: " << std::sin(rotation) * spStarCom->orbitRadius << "; Float: " << spEntity->GetPosition().x << '\n';
        }//Check if entity has planet component
        else if (spEntity->HasComponent<PlanetComponent>())
        {
            //Get planet component
            std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();

            if (spPlanetCom->isMoon || !simulateOnlyMoons)
            {
                //Move planet
                //std::cout <<"Moved: " << spEntity->GetName() << '\n';
                double rotation = (spPlanetCom->rotationalVelocity * static_cast<double>(ECSGame::Instance().GetDaysPast())) + spPlanetCom->initialRotationPosition;
                spEntity->SetPosition(sf::Vector2f(static_cast<float>(std::sin(rotation) * spPlanetCom->orbitRadius), static_cast<float>(std::cos(rotation) * spPlanetCom->orbitRadius)));

                //if(spPlanetCom->isMoon && ECSGame::Instance().GetOverviewType()==OverviewType::Planet)
                //    std::cout <<"Time: " << ECSGame::Instance().GetDaysPast() << "; Double: " << std::sin(rotation) * spPlanetCom->orbitRadius << "; Float: " << spEntity->GetPosition().x << '\n';
            }
            //else
                //std::cout <<"Did not moved: " << spEntity->GetName() << '\n';
        }
    }

    //sf::Vect
}


//Buttons processing function
void SceneNodeVisitorButton::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has Button component
        if (spEntity->HasComponent<ButtonComponent>())
        {
            //Get Button component
            std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();

            if (spButton->enabled)
            {
                if (gel::IsPointInTheArea(mousePosition, spButton->buttonSize, spEntity->GetPosition()))
                {
                    if (!spButton->isHovered)
                    {
                        spButton->isHovered = true;
                        spButton->onButtonHovered(spEntity);
                    }
                }
                else
                {
                    if (spButton->isHovered) 
                    {
                        spButton->isHovered = false;
                        spButton->onButtonUnhovered(spEntity);
                    }
                }

                if (inputSystem.lmbPressed) 
                {
                    if (spButton->isHovered && !spButton->isPressed) 
                    {
                        spButton->isPressed= true;
                        spButton->onButtonPressed(spEntity);
                    }
                }
                else 
                {
                    if (spButton->isPressed)
                    {
                        spButton->isPressed = false;
                        spButton->onButtonReleased(spEntity);

                        if (spButton->isHovered)
                            spButton->onButtonClicked(spEntity);
                    }
                }
            }
        }
    }
}
