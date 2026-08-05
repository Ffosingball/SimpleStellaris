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
	if (key.code == sf::Keyboard::Key::Q)
	{
		;
	}
	else if (key.code == sf::Keyboard::Key::Escape)
	{
		//Close game
		ECSGame::Instance().CloseGame();
	}
}


void InputSystem::OnKeyReleased(sf::Event::KeyReleased key)
{
	//If key released then reset bool values
}


void InputSystem::OnMouseWheelScrolled(sf::Event::MouseWheelScrolled mw) 
{
	std::shared_ptr<CameraComponent> spCameraCom = GetCurrentlyActiveCamera();
	sf::Vector2f previousCameraSize = spCameraCom->view.getSize();
	//sf::Vector2f previousSize = spCameraCom->view.getSize();
	//Zoom camera
	spCameraCom->currentZoom = gel::clamp((spCameraCom->zoomingSpeed * ECSGame::Instance().GetDeltaTime() * mw.delta) + spCameraCom->currentZoom, spCameraCom->zoomingBorders.x, spCameraCom->zoomingBorders.y);
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


//I process movement and fire keys in every frame, because game reacts to the key press
//on the same frame as it was pressed, and it will react every fram until the key
//is released. If I would use events, they are not called every frame, which is bad
void InputSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	//Set direction to 0,0
	sf::Vector2f direction{ 0,0 };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		//Change direction to positive
		direction.y -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		//Change direction to negative
		direction.y += 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		direction.x -= 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		direction.x += 1.f;
	}

	//Deal with mouse movement
	sf::Vector2i mousePosition = ECSGame::Instance().GetMousePosition();

	mousePosText->text->setString("Window pos: " + std::to_string(mousePosition.x) + "; " + std::to_string(mousePosition.y));
	sf::Vector2f positionInWorld = ConvertWindowPositionToWorld(GetCurrentlyActiveCamera()->view, mousePosition);
	//sf::Vector2i positionInWindow = ConvertWorldPositionToWindow(GetCameraFromCameraEntity()->view, positionInWorld);
	worldPosText->text->setString("World pos: " + std::to_string(positionInWorld.x) + "; " + std::to_string(positionInWorld.y));

	std::vector<std::shared_ptr<SceneNode>> systemsNearBy = GetAllSystemsNearPosition(positionInWorld);

	std::string message{ "Systems nearby: " };
	float closestDistance = 999999.f;
	int closestSystemIndex = -1;
	int counter{ 0 };
	for (std::shared_ptr<SceneNode> spNode : systemsNearBy)
	{
		message += GetObjectSystemComponent(*spNode->GetEntity().lock())->systemName +" ("+ spNode->GetEntity().lock()->GetName() + "); ";
		if (gel::distanceBetween2Points(positionInWorld, spNode->GetEntity().lock()->GetPosition()) < closestDistance)
		{
			closestDistance = gel::distanceBetween2Points(positionInWorld, spNode->GetEntity().lock()->GetPosition());
			closestSystemIndex = counter;
			selectedSystemEntity->hidden = false;
		}
		counter++;
	}

	if (closestSystemIndex == -1)
		selectedSystemEntity->hidden = true;
	else
		selectedSystemIcon->entityToFollow = systemsNearBy[closestSystemIndex]->GetEntity();

	systemsNearByText->text->setString(message);

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
	systemName = "UISystem";
}

void UISystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	SceneNodeVisitorUI visitor(*this);
	scene->AcceptVisitor(visitor);
}




//MUSIC SYSTEM
void MusicSystem::Initialize()
{
	/*
	//Subscribe to some signals
	signals::onGameRestart.connect(&MusicSystem::OnGameRestart, this);
	signals::onPlayExplosionSound.connect(&MusicSystem::OnPlayExplosionSound, this);
	signals::onPlayerHurt.connect(&MusicSystem::OnPlayHurtSound, this);
	signals::onPlayShotSound.connect(&MusicSystem::OnPlayShotSound, this);
	signals::onPlayWarningSound.connect(&MusicSystem::OnPlayWarningSound, this);
	signals::onPlayPowerUpSound.connect(&MusicSystem::OnPlayPowerUpSound, this);
	signals::onPlayManyExplosionsSounds.connect(&MusicSystem::OnPlayManyExplosionsSounds, this);
	signals::onPlayWonSound.connect(&MusicSystem::OnPlayWonSound, this);
	//Initialize music
	mainMusic = ResourceManager::Instance().LoadMusic("media/music/time_for_adventure.mp3").lock();
	mainMusic->setLooping(true);
	mainMusic->play();
	//Load sound
	spExplosionSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/explosion.wav").lock());
	spHurtSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/hurt.wav").lock());
	spShotSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/tap.wav").lock());
	spWarningSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/jump.wav").lock());
	spPowerUpSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/power_up.wav").lock());
	spWonSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/coin.wav").lock());
	*/
	systemName = "MusicSystem";
}

void MusicSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	/*
	//Check if we played all explosion sound
	if (playExplosionSound > 0)
	{
		timeLeftToPlaySound -= deltaTime;
		//Check when we need to play it again
		if (timeLeftToPlaySound <= 0)
		{
			spExplosionSound->play();
			timeLeftToPlaySound += gel::Randf(0.1f, 0.5f);
			playExplosionSound--;
		}
	}

	//Check if we played all won sound
	if (playWonSound > 0)
	{
		timeLeftToPlayWonSound -= deltaTime;
		//Check when we need to play it again
		if (timeLeftToPlayWonSound <= 0)
		{
			spWonSound->play();
			timeLeftToPlayWonSound += 1.f;
			playWonSound--;
		}
	}*/
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
	if (scene != ECSGame::Instance().GetUIRoot())
	{
		SceneNodeVisitorSystemVisibility visitor;
		scene->AcceptVisitor(visitor);
	}
}
