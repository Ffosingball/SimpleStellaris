#include <memory>
#include "Systems.h"
#include <SFML/Graphics.hpp>
#include "EntitiesFunctions.h"
#include "SceneNodeVisitors.h"
#include <iostream>
#include "CommonGameCode.h"
#include <SFML/Audio.hpp>
#include "ResourceManager.h"
#include "SpaceObjectTypes.h"


//INPUT SYSTEM
void InputSystem::Initialize() 
{
	//Subscribe functions to these signals
	signals::onKeyPressed.connect(&InputSystem::OnKeyPressed, this);
	signals::onKeyReleased.connect(&InputSystem::OnKeyReleased, this);
	signals::onMouseWheelScrolled.connect(&InputSystem::OnMouseWheelScrolled, this);
	signals::onMouseMoved.connect(&InputSystem::OnMouseMoved, this);
	signals::onMouseButtonPressed.connect(&InputSystem::OnMouseButtonPressed, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("MouseCoordsText").lock();
	mousePosText = GetTextComponent(*mctPtr->GetEntity().lock());

	std::shared_ptr<SceneNode> wctPtr = ECSGame::Instance().GetUIRoot()->FindChild("WorldCoordsText").lock();
	worldPosText = GetTextComponent(*wctPtr->GetEntity().lock());

	std::shared_ptr<SceneNode> wsnPtr = ECSGame::Instance().GetUIRoot()->FindChild("SystemsNearByText").lock();
	systemsNearByText = GetTextComponent(*wsnPtr->GetEntity().lock());

	std::shared_ptr<SceneNode> wfpsPtr = ECSGame::Instance().GetUIRoot()->FindChild("FPSText").lock();
	fpsText = GetTextComponent(*wfpsPtr->GetEntity().lock());

	std::shared_ptr<SceneNode> wsiPtr = ECSGame::Instance().GetUIRoot()->FindChild("SelectedSystemIcon").lock();
	selectedSystemIcon = GetUIFollowerComponent(*wsiPtr->GetEntity().lock());
	selectedSystemEntity = wsiPtr->GetEntity().lock();

	std::shared_ptr<SceneNode> mouseNodeSP = ECSGame::Instance().GetUIRoot()->FindChild("MouseIcon").lock();
	mouseIconEntity = mouseNodeSP->GetEntity().lock();

	systemName = "InputSystem";
}

//Process keys they are pressed
void InputSystem::OnKeyPressed(sf::Event::KeyPressed key) 
{
	if (key.code == sf::Keyboard::Key::Space)
	{
		if (ECSGame::Instance().GetGameState() == GameState::Game)
			ECSGame::Instance().SetGameState(GameState::Pause);
		else if(ECSGame::Instance().GetGameState() == GameState::Pause)
			ECSGame::Instance().SetGameState(GameState::Game);
	}
	else if (key.code == sf::Keyboard::Key::Escape)
	{
		if (ECSGame::Instance().GetOverviewType() != OverviewType::Space)
		{
			ECSGame::Instance().SetOverviewType(OverviewType::Space);

			SceneNodeVisitorChangeAllSystemVisibility visitor(false);
			ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);

			//ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock()->OutputTree(" ");

			SceneNodeVisitorChangeSingleSystemVisibility visitor2(true, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
			wpSelectedSystemNode.lock()->AcceptVisitor(visitor2);
		}
		else
			ECSGame::Instance().CloseGame();
	}
	else if (key.code == sf::Keyboard::Key::Up)
	{
		if(shiftHold)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 100);
		else if (ctrlHold)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 10);
		else
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 1);
	}
	else if (key.code == sf::Keyboard::Key::Down)
	{
		if (shiftHold)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 100);
		else if (ctrlHold)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 10);
		else
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 1);
	}
	else if (key.code == sf::Keyboard::Key::LControl) 
	{
		ctrlHold = true;
	}
	else if (key.code == sf::Keyboard::Key::LShift)
	{
		shiftHold = true;
	}
}


void InputSystem::OnKeyReleased(sf::Event::KeyReleased key)
{
	//If key released then reset bool values
	if (key.code == sf::Keyboard::Key::LControl)
	{
		ctrlHold = false;
	}
	else if (key.code == sf::Keyboard::Key::LShift)
	{
		shiftHold = false;
	}
}


void InputSystem::OnMouseWheelScrolled(sf::Event::MouseWheelScrolled mw) 
{
	std::shared_ptr<CameraComponent> spCameraCom = GetCurrentlyActiveCamera();
	sf::Vector2f previousCameraSize = spCameraCom->view.getSize();
	//sf::Vector2f previousSize = spCameraCom->view.getSize();
	//std::cout << mw.delta << '\n';
	
	//Zoom camera
	if (mw.delta < 0)
	{
		spCameraCom->currentZoom = gel::clamp(spCameraCom->currentZoom*(1+(ECSGame::Instance().GetDeltaTime()* spCameraCom->zoomingSpeed)), spCameraCom->zoomingBorders.x, spCameraCom->zoomingBorders.y);
		//std::cout << "Current zoom INC: " << spCameraCom->currentZoom << '\n';
	}
	else
	{
		spCameraCom->currentZoom = gel::clamp(spCameraCom->currentZoom * (1 - (ECSGame::Instance().GetDeltaTime() * spCameraCom->zoomingSpeed)), spCameraCom->zoomingBorders.x, spCameraCom->zoomingBorders.y);
		//std::cout << "Current zoom DEC: " << spCameraCom->currentZoom << '\n';
	}
	spCameraCom->view.setSize(spCameraCom->cameraSize * spCameraCom->currentZoom);
	
	//Move camera so, it looks like camera zooms to the place where mouse is pointing
	sf::Vector2f mousePositionInWorld = ConvertWindowPositionToWorld(spCameraCom->view, ECSGame::Instance().GetMousePosition());

	float previousLeftXBorder = spCameraCom->view.getCenter().x - (previousCameraSize.x / 2.f);
	float previousTopYBorder = spCameraCom->view.getCenter().y - (previousCameraSize.y / 2.f);

	float relativeXPos = (mousePositionInWorld.x - previousLeftXBorder) / previousCameraSize.x;
	float relativeYPos = (mousePositionInWorld.y - previousTopYBorder) / previousCameraSize.y;

	float newLeftXBorder = spCameraCom->view.getCenter().x - (spCameraCom->view.getSize().x / 2.f);
	float newTopYBorder = spCameraCom->view.getCenter().y - (spCameraCom->view.getSize().y / 2.f);

	sf::Vector2f newMousePosInWorld { gel::linearInterpolation(newLeftXBorder, newLeftXBorder + spCameraCom->view.getSize().x ,relativeXPos) , gel::linearInterpolation(newTopYBorder, newTopYBorder + spCameraCom->view.getSize().y ,relativeYPos) };
	spCameraCom->view.move( mousePositionInWorld - newMousePosInWorld);

	//Check that camera do not go out of bounds
	sf::Vector2f camCenter = spCameraCom->view.getCenter();
	float moveX{0.f};
	float moveY{ 0.f };
	if (camCenter.x + (spCameraCom->view.getSize().x / 2.f) >= spCameraCom->horizontalBorders.y)
		moveX = -(camCenter.x + (spCameraCom->view.getSize().x / 2.f) - spCameraCom->horizontalBorders.y);
	else if (camCenter.x - (spCameraCom->view.getSize().x / 2.f) <= spCameraCom->horizontalBorders.x)
		moveX = -(camCenter.x + moveX - (spCameraCom->view.getSize().x / 2.f) - spCameraCom->horizontalBorders.x);

	if (camCenter.y + (spCameraCom->view.getSize().y / 2.f) >= spCameraCom->verticalBorders.y)
		moveY = -(camCenter.y + moveY + (spCameraCom->view.getSize().y / 2.f) - spCameraCom->verticalBorders.y);
	else if (camCenter.y - (spCameraCom->view.getSize().y / 2.f) <= spCameraCom->verticalBorders.x)
		moveY = -(camCenter.y + moveY - (spCameraCom->view.getSize().y / 2.f) - spCameraCom->verticalBorders.x);

	spCameraCom->view.move({moveX, moveY});
}


void InputSystem::OnMouseMoved(sf::Event::MouseMoved mouseMovement) 
{
	//Move mouse
	mouseIconEntity->SetPosition({ (float)mouseMovement.position.x, (float)mouseMovement.position.y});
}


void InputSystem::OnMouseButtonPressed(sf::Event::MouseButtonPressed mouseButPressed)
{
	if (mouseButPressed.button == sf::Mouse::Button::Left) 
	{
		if (ECSGame::Instance().GetOverviewType() == OverviewType::Space && wpSelectedSystemNode.lock()!=nullptr) 
		{
			ECSGame::Instance().SetOverviewType(OverviewType::System);

			SceneNodeVisitorChangeAllSystemVisibility visitor(true);
			ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);

			//std::cout << "-- Entering system view: "<<'\n';
			//wpSelectedSystemNode.lock()->OutputTree("  ");
			SceneNodeVisitorChangeSingleSystemVisibility visitor2(false, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
			wpSelectedSystemNode.lock()->AcceptVisitor(visitor2);

			//Setup background camera
			std::shared_ptr<CameraComponent> sBackCameraCom = GetCameraFromBackgroundCameraEntity();
			std::shared_ptr<CameraComponent> sSpaceCameraCom = GetCameraFromSpaceCameraEntity();
			sBackCameraCom->view.setCenter(wpSelectedSystemNode.lock()->GetEntity().lock()->GetPosition());
			sBackCameraCom->view.setSize(sSpaceCameraCom->cameraSize* sSpaceCameraCom->zoomingBorders.x);

			//Setup system camera
			std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
			sSystemCameraCom->view.setCenter(sf::Vector2f{0.f,0.f});
			sSystemCameraCom->view.setSize(sSystemCameraCom->cameraSize);
			sSystemCameraCom->currentZoom = 1.f;

			signals::onSystemOverviewSet(wpSelectedSystemNode.lock());
		}
	}
}


//I process movement and fire keys in every frame, because game reacts to the key press
//on the same frame as it was pressed, and it will react every fram until the key
//is released. If I would use events, they are not called every frame, which is bad
void InputSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	//Set direction to 0,0
	sf::Vector2f direction{ 0,0 };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		//Change direction to positive
		direction.y -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		//Change direction to negative
		direction.y += 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		direction.x -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		direction.x += 1.f;
	}

	//Deal with mouse movement
	sf::Vector2i mousePosition = ECSGame::Instance().GetMousePosition();

	mousePosText->text->setString("Window pos: " + std::to_string(mousePosition.x) + "; " + std::to_string(mousePosition.y));
	sf::Vector2f positionInWorld = ConvertWindowPositionToWorld(GetCurrentlyActiveCamera()->view, mousePosition);
	//sf::Vector2i positionInWindow = ConvertWorldPositionToWindow(GetCameraFromCameraEntity()->view, positionInWorld);
	worldPosText->text->setString("World pos: " + std::to_string(positionInWorld.x) + "; " + std::to_string(positionInWorld.y));

	if (ECSGame::Instance().GetOverviewType() == OverviewType::Space)
	{
		std::vector<std::shared_ptr<SceneNode>> systemsNearBy = GetAllSystemsNearPosition(positionInWorld);

		std::string message{ "Systems nearby: " };
		float closestDistance = 999999.f;
		int closestSystemIndex = -1;
		int counter{ 0 };
		for (std::shared_ptr<SceneNode> spNode : systemsNearBy)
		{
			std::shared_ptr<ObjectSystemComponent> spSysCom = GetObjectSystemComponent(*spNode->GetEntity().lock());
			message += spSysCom->systemName + " (" + spNode->GetEntity().lock()->GetName() + ") "+GetSpaceSystemTypeName(spSysCom->systemType);
			if (gel::distanceBetween2Points(positionInWorld, spNode->GetEntity().lock()->GetPosition()) < closestDistance)
			{
				closestDistance = gel::distanceBetween2Points(positionInWorld, spNode->GetEntity().lock()->GetPosition());
				closestSystemIndex = counter;
				wpSelectedSystemNode = spNode;
			}
			counter++;
		}

		if (closestSystemIndex == -1)
		{
			selectedSystemIcon->nodeToFollow = {};
			wpSelectedSystemNode = {};
		}
		else
			selectedSystemIcon->nodeToFollow = systemsNearBy[closestSystemIndex];

		systemsNearByText->text->setString(message);
	}
	else
		systemsNearByText->text->setString(" ");

	fpsText->text->setString(std::to_string(ECSGame::Instance().GetFPS())+" fps");

	//Signal the direction to the movement system
	signals::onMoveCamera(direction);
}


//MOVEMENT SYSTEM
void MovementSystem::Initialize()
{
	//Subscribe to this signal, to get player direction every frame
	signals::onMoveCamera.connect(&MovementSystem::OnMoveCamera, this);
	//signals::onZoomCamera.connect(&MovementSystem::OnZoomCamera, this);
	systemName = "MovementSystem";
}


void MovementSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	//If game paused, then do nothing
	SceneNodeVisitorMovement visitor(*this);
	scene->AcceptVisitor(visitor);
}


//UI SYSTEM
void UISystem::Initialize() 
{
	//Subscribe to some signals
	signals::onRenderingComplete.connect(&UISystem::OnRenderingComplete, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("RenderText").lock();
	nodesText = GetTextComponent(*mctPtr->GetEntity().lock());

	systemName = "UISystem";
}

void UISystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	nodesText.lock()->text->setString("Total nodes: "+std::to_string(numOfNodes)+"; rendered: "+std::to_string(nodesRendered));

	if (scene == ECSGame::Instance().GetUIRoot())
	{
		SceneNodeVisitorUI visitor(*this, GetCurrentlyActiveCamera(), GetCameraFromUICameraEntity());
		scene->AcceptVisitor(visitor);
	}
}

void UISystem::OnRenderingComplete(int numOfNodes, int nodesRendered)
{
	this->numOfNodes = numOfNodes;
	this->nodesRendered = nodesRendered;
}




//MUSIC SYSTEM
void MusicSystem::Initialize()
{
	
	systemName = "MusicSystem";
}

void MusicSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	
}



//GAME SYSTEM
void GameSystem::Initialize()
{
	//Subscribe to some signals
	systemName = "GameSystem";
}



//Update systems visibility
void GameSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	if (scene == ECSGame::Instance().GetSceneRoot() && ECSGame::Instance().GetOverviewType()==OverviewType::Space)
	{
		SceneNodeVisitorSystemVisibility visitor(GetCurrentlyActiveCamera());
		scene->AcceptVisitor(visitor);
	}
}



//SIMULATION SYSTEM
void SimulationSystem::Initialize()
{
	systemName = "SimulationSystem";
	signals::onSystemOverviewSet.connect(&SimulationSystem::OnSystemOverviewSet, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("DaysPastText").lock();
	daysPastText = GetTextComponent(*mctPtr->GetEntity().lock());

	std::shared_ptr<SceneNode> wctPtr = ECSGame::Instance().GetUIRoot()->FindChild("DateText").lock();
	dateText = GetTextComponent(*wctPtr->GetEntity().lock());
}

void SimulationSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	daysPastText.lock()->text->setString("Simulation speed: " + std::to_string(ECSGame::Instance().GetSimulationSpeed()) + "; Days past: " + std::to_string((int)ECSGame::Instance().GetDaysPast()));
	dateText.lock()->text->setString("Proper Date: " + GetDateFromDays((int)ECSGame::Instance().GetDaysPast()));

	if (ECSGame::Instance().GetOverviewType() == OverviewType::System) 
	{
		SceneNodeVisitorMoveObjectsInSystem visitor;
		wpNodeToSimulate.lock()->AcceptVisitor(visitor);
	}
}

void SimulationSystem::OnSystemOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate)
{
	wpNodeToSimulate = nodeToSimulate;
}
