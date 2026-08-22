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
#include "WorldGenerator.h"
//#include "CommonGameCode.h"


//INPUT SYSTEM
void InputSystem::Initialize() 
{
	//Subscribe functions to these signals
	signals::onKeyPressed.connect(&InputSystem::OnKeyPressed, this);
	signals::onKeyReleased.connect(&InputSystem::OnKeyReleased, this);
	signals::onMouseWheelScrolled.connect(&InputSystem::OnMouseWheelScrolled, this);
	signals::onMouseMoved.connect(&InputSystem::OnMouseMoved, this);
	signals::onMouseButtonPressed.connect(&InputSystem::OnMouseButtonPressed, this);
	signals::onJoystickMoved.connect(&InputSystem::OnJoystickMoved, this);
	signals::onJoystickButtonPressed.connect(&InputSystem::OnJoystickButtonPressed, this);
	signals::onJoystickButtonReleased.connect(&InputSystem::OnJoystickButtonReleased, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("MouseCoordsText").lock();
	mousePosText = mctPtr->GetEntity().lock()->FindComponent<TextComponent>().lock();
	debugTextes.push_back(mctPtr->GetEntity());

	std::shared_ptr<SceneNode> wctPtr = ECSGame::Instance().GetUIRoot()->FindChild("WorldCoordsText").lock();
	worldPosText = wctPtr->GetEntity().lock()->FindComponent<TextComponent>().lock();
	debugTextes.push_back(wctPtr->GetEntity());

	std::shared_ptr<SceneNode> wsnPtr = ECSGame::Instance().GetUIRoot()->FindChild("SystemsNearByText").lock();
	systemsNearByText = wsnPtr->GetEntity().lock()->FindComponent<TextComponent>().lock();
	debugTextes.push_back(wsnPtr->GetEntity());

	std::shared_ptr<SceneNode> wfpsPtr = ECSGame::Instance().GetUIRoot()->FindChild("FPSText").lock();
	fpsText = wfpsPtr->GetEntity().lock()->FindComponent<TextComponent>().lock();
	debugTextes.push_back(wfpsPtr->GetEntity());

	std::shared_ptr<SceneNode> wsiPtr = ECSGame::Instance().GetUIRoot()->FindChild("SelectedSystemIcon").lock();
	selectedSystemIcon = wsiPtr->GetEntity().lock()->FindComponent<UIFollowerComponent>().lock();
	selectedSystemEntity = wsiPtr->GetEntity().lock();
	//debugTextes.push_back(wsiPtr->GetEntity());

	std::shared_ptr<SceneNode> mouseNodeSP = ECSGame::Instance().GetUIRoot()->FindChild("MouseIcon").lock();
	mouseIconEntity = mouseNodeSP->GetEntity().lock();

	std::shared_ptr<SceneNode> s2Ptr = ECSGame::Instance().GetUIRoot()->FindChild("DaysPastText").lock();
	debugTextes.push_back(s2Ptr->GetEntity());

	std::shared_ptr<SceneNode> s3Ptr = ECSGame::Instance().GetUIRoot()->FindChild("DateText").lock();
	debugTextes.push_back(s3Ptr->GetEntity());

	std::shared_ptr<SceneNode> s4Ptr = ECSGame::Instance().GetUIRoot()->FindChild("RenderText").lock();
	debugTextes.push_back(s4Ptr->GetEntity());

	for (std::weak_ptr<Entity> e : debugTextes) 
	{
		e.lock()->hidden = true;
	}
	showDebugText = false;

	systemName = "InputSystem";
}


void LockCameraOnNode(std::weak_ptr<SceneNode> wpNodeToLockOn)
{
	std::shared_ptr<CameraComponent> spCameraCom = GetCurrentlyActiveCamera();
	spCameraCom->cameraLocked = true;
	spCameraCom->wpNodeLockedOn = wpNodeToLockOn;
	//std::cout<< "Locked camera on: " << spCameraCom->wpNodeLockedOn.lock()->GetEntity().lock()->GetName() << '\n';
}


void CancelCameraLock()
{
	std::shared_ptr<CameraComponent> spCameraCom = GetCurrentlyActiveCamera();
	spCameraCom->cameraLocked = false;
	spCameraCom->wpNodeLockedOn = {};
}


void InputSystem::EnterSystemOverview() 
{
	//Cancel camera lock
	CancelCameraLock();

	ECSGame::Instance().SetOverviewType(OverviewType::System);

	SceneNodeVisitorChangeAllSystemVisibility visitor(true);
	ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);

	std::shared_ptr<SceneNode> spSelectedSystemNode = wpSelectedSystemNode.lock();
	std::shared_ptr<ObjectSystemComponent> spSysCom = spSelectedSystemNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
	spSelectedSystemNode->AddChild(spSysCom->spAllSystemObjectsNode);

	//std::cout << "-- Entering system view: "<<'\n';
	//wpSelectedSystemNode.lock()->OutputTree("  ");
	SceneNodeVisitorChangeSingleSystemVisibility visitor2(false, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
	spSelectedSystemNode->AcceptVisitor(visitor2);

	//Setup background camera
	std::shared_ptr<CameraComponent> sBackCameraCom = GetCameraFromBackgroundCameraEntity();
	std::shared_ptr<CameraComponent> sSpaceCameraCom = GetCameraFromSpaceCameraEntity();
	sBackCameraCom->view.setCenter(spSelectedSystemNode->GetEntity().lock()->GetPosition());
	sBackCameraCom->view.setSize(sSpaceCameraCom->cameraSize * sSpaceCameraCom->zoomingBorders.x);
	sSpaceCameraCom->moveCamera = false;

	//Setup system camera
	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
	sSystemCameraCom->view.setSize(sSystemCameraCom->cameraSize);
	sSystemCameraCom->currentZoom = 1.f;
	sSystemCameraCom->moveCamera = true;

	signals::onSystemOverviewSet(wpSelectedSystemNode.lock());
}


void InputSystem::EnterPlanetFromSystemOverview()
{
	//Cancel camera lock
	CancelCameraLock();

	ECSGame::Instance().SetOverviewType(OverviewType::Planet);

	SceneNodeVisitorChangeSingleSystemVisibility visitor(true, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
	wpSelectedSystemNode.lock()->AcceptVisitor(visitor);

	float earthDiameter = ECSGame::Instance().GetSceneRoot()->FindChild("SpaceMap").lock()->GetEntity().lock()->FindComponent<SystemPropertiesComponent>().lock()->mapConfig.earthDiameter;
	SceneNodeVisitorChangeSinglePlanetVisibility visitor2(false, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock(), earthDiameter);
	wpPlanetOrStarSelected.lock()->AcceptVisitor(visitor2);

	//Setup planet camera
	std::shared_ptr<CameraComponent> sPlanetCameraCom = GetCameraFromPlanetCameraEntity();
	sPlanetCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
	sPlanetCameraCom->view.setSize(sPlanetCameraCom->cameraSize);
	sPlanetCameraCom->currentZoom = 1.f;
	sPlanetCameraCom->moveCamera = true;

	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->moveCamera = false;

	signals::onPlanetOverviewSet(wpPlanetOrStarSelected.lock());
}


void InputSystem::ExitSystemOverview() 
{
	//Cancel camera lock
	CancelCameraLock();

	ECSGame::Instance().SetOverviewType(OverviewType::Space);

	SceneNodeVisitorChangeAllSystemVisibility visitor(false);
	ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);

	SceneNodeVisitorChangeSingleSystemVisibility visitor2(true, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
	wpSelectedSystemNode.lock()->AcceptVisitor(visitor2);

	std::shared_ptr<SceneNode> spSelectedSystemNode = wpSelectedSystemNode.lock();
	std::shared_ptr<ObjectSystemComponent> spSysCom = spSelectedSystemNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
	spSelectedSystemNode->RemoveByEntity(spSysCom->spAllSystemObjectsNode->GetEntity().lock());

	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->moveCamera = false;
	std::shared_ptr<CameraComponent> sSpaceCameraCom = GetCameraFromSpaceCameraEntity();
	sSpaceCameraCom->moveCamera = true;
}


void InputSystem::ExitPlanetToSystemOverview()
{
	//Cancel camera lock
	CancelCameraLock();

	ECSGame::Instance().SetOverviewType(OverviewType::System);

	float earthDiameter = ECSGame::Instance().GetSceneRoot()->FindChild("SpaceMap").lock()->GetEntity().lock()->FindComponent<SystemPropertiesComponent>().lock()->mapConfig.earthDiameter;
	SceneNodeVisitorChangeSinglePlanetVisibility visitor2(true, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock(), earthDiameter);
	wpPlanetOrStarSelected.lock()->AcceptVisitor(visitor2);

	SceneNodeVisitorChangeSingleSystemVisibility visitor(false, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
	wpSelectedSystemNode.lock()->AcceptVisitor(visitor);

	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->moveCamera = true;
	std::shared_ptr<CameraComponent> sPlanetCameraCom = GetCameraFromPlanetCameraEntity();
	sPlanetCameraCom->moveCamera = false;

	signals::onSystemOverviewSet(wpSelectedSystemNode.lock());
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
		if (ECSGame::Instance().GetOverviewType() == OverviewType::System)
		{
			ExitSystemOverview();
		}
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet)
		{
			ExitPlanetToSystemOverview();
		}
		else if(ECSGame::Instance().GetOverviewType() == OverviewType::Space)
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
	else if (key.code == sf::Keyboard::Key::F3)
	{
		showDebugText = !showDebugText;
		for (std::weak_ptr<Entity> e : debugTextes)
		{
			e.lock()->hidden = showDebugText;
		}
	}

	lastInputByJoystick = false;
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

	lastInputByJoystick = false;
}


void InputSystem::OnJoystickMoved(sf::Event::JoystickMoved joystickMoved)
{
	if (joystickMoved.axis == sf::Joystick::Axis::PovX) 
	{
		//std::cout << "PovX: " << joystickMoved.position << '\n';
		if(joystickMoved.position > minValForJoystick)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 10);
		else if (joystickMoved.position < -minValForJoystick)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 10);
		
		lastInputByJoystick = true;
	}

	if (joystickMoved.axis == sf::Joystick::Axis::PovY)
	{
		//std::cout << "PovY: " << joystickMoved.position << '\n';
		if (joystickMoved.position > minValForJoystick)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 1);
		else if (joystickMoved.position < -minValForJoystick)
			ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 1);
		
		lastInputByJoystick = true;
	}
}


void InputSystem::OnJoystickButtonPressed(sf::Event::JoystickButtonPressed button)
{
	switch (button.button) 
	{
	case 0:
		if (ECSGame::Instance().GetOverviewType() == OverviewType::Space && wpSelectedSystemNode.lock() != nullptr)
			EnterSystemOverview();
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr) 
		{
			if (wpPlanetOrStarSelected.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
				EnterPlanetFromSystemOverview();
		}
		break;
	case 1:
		if(ECSGame::Instance().GetOverviewType() == OverviewType::System)
			ExitSystemOverview();
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet)
			ExitPlanetToSystemOverview();
		break;
	case 2:
		if (ECSGame::Instance().GetOverviewType() == OverviewType::Space && wpSelectedSystemNode.lock() != nullptr)
		{
			LockCameraOnNode(wpSelectedSystemNode);
		}
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr)
		{
			LockCameraOnNode(wpPlanetOrStarSelected);
		}
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet && wpMoonOrPlanetSelected.lock() != nullptr)
		{
			LockCameraOnNode(wpMoonOrPlanetSelected);
		}
		else
			CancelCameraLock();
		break;
	case 5:
		if (ECSGame::Instance().GetGameState() == GameState::Game)
			ECSGame::Instance().SetGameState(GameState::Pause);
		else if (ECSGame::Instance().GetGameState() == GameState::Pause)
			ECSGame::Instance().SetGameState(GameState::Game);
		break;
	case 6:
		ECSGame::Instance().CloseGame();
		break;
	}

	lastInputByJoystick = true;
}


void InputSystem::OnJoystickButtonReleased(sf::Event::JoystickButtonReleased button)
{

}


void InputSystem::ZoomCamera(int direction) 
{
	std::shared_ptr<CameraComponent> spCameraCom = GetCurrentlyActiveCamera();
	sf::Vector2f previousCameraSize = spCameraCom->view.getSize();

	float multiplier = 1.f;
	if (lastInputByJoystick)
		multiplier = zoomSpeedJoystickSlowing;

	//Zoom camera
	if (direction < 0)
	{
		spCameraCom->currentZoom = gel::clamp(spCameraCom->currentZoom * (1 + (ECSGame::Instance().GetDeltaTime() * spCameraCom->zoomingSpeed * multiplier)), spCameraCom->zoomingBorders.x, spCameraCom->zoomingBorders.y);
		//std::cout << "Current zoom INC: " << spCameraCom->currentZoom << '\n';
	}
	else
	{
		spCameraCom->currentZoom = gel::clamp(spCameraCom->currentZoom * (1 - (ECSGame::Instance().GetDeltaTime() * spCameraCom->zoomingSpeed * multiplier)), spCameraCom->zoomingBorders.x, spCameraCom->zoomingBorders.y);
		//std::cout << "Current zoom DEC: " << spCameraCom->currentZoom << '\n';
	}
	spCameraCom->view.setSize(spCameraCom->cameraSize * spCameraCom->currentZoom);

	if (!lastInputByJoystick && !spCameraCom->cameraLocked)
	{
		//Move camera so, it looks like camera zooms to the place where mouse is pointing
		sf::Vector2f mousePositionInWorld = ConvertWindowPositionToWorld(spCameraCom->view, ECSGame::Instance().GetMousePosition());

		float previousLeftXBorder = spCameraCom->view.getCenter().x - (previousCameraSize.x / 2.f);
		float previousTopYBorder = spCameraCom->view.getCenter().y - (previousCameraSize.y / 2.f);

		float relativeXPos = (mousePositionInWorld.x - previousLeftXBorder) / previousCameraSize.x;
		float relativeYPos = (mousePositionInWorld.y - previousTopYBorder) / previousCameraSize.y;

		float newLeftXBorder = spCameraCom->view.getCenter().x - (spCameraCom->view.getSize().x / 2.f);
		float newTopYBorder = spCameraCom->view.getCenter().y - (spCameraCom->view.getSize().y / 2.f);

		sf::Vector2f newMousePosInWorld{ gel::linearInterpolation(newLeftXBorder, newLeftXBorder + spCameraCom->view.getSize().x ,relativeXPos) , gel::linearInterpolation(newTopYBorder, newTopYBorder + spCameraCom->view.getSize().y ,relativeYPos) };
		spCameraCom->view.move(mousePositionInWorld - newMousePosInWorld);
	}

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
}


void InputSystem::OnMouseWheelScrolled(sf::Event::MouseWheelScrolled mw) 
{
	lastInputByJoystick = false;
	ZoomCamera(mw.delta);
}


void InputSystem::OnMouseMoved(sf::Event::MouseMoved mouseMovement) 
{
	//Move mouse
	mouseIconEntity->SetPosition({ (float)mouseMovement.position.x, (float)mouseMovement.position.y});
	//lastInputByJoystick = false;
}



void InputSystem::OnMouseButtonPressed(sf::Event::MouseButtonPressed mouseButPressed)
{
	if (mouseButPressed.button == sf::Mouse::Button::Left) 
	{
		if (ECSGame::Instance().GetOverviewType() == OverviewType::Space && wpSelectedSystemNode.lock()!=nullptr) 
		{
			EnterSystemOverview();
		}
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr)
		{
			if (wpPlanetOrStarSelected.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
				EnterPlanetFromSystemOverview();
		}
	}
	else if (mouseButPressed.button == sf::Mouse::Button::Right)
	{
		if (ECSGame::Instance().GetOverviewType() == OverviewType::Space && wpSelectedSystemNode.lock() != nullptr)
		{
			LockCameraOnNode(wpSelectedSystemNode);
		}
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr)
		{
			LockCameraOnNode(wpPlanetOrStarSelected);
		}
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet && wpMoonOrPlanetSelected.lock() != nullptr)
		{
			LockCameraOnNode(wpMoonOrPlanetSelected);
		}
		else
			CancelCameraLock();
	}

	lastInputByJoystick = false;
}


//I process movement and fire keys in every frame, because game reacts to the key press
//on the same frame as it was pressed, and it will react every fram until the key
//is released. If I would use events, they are not called every frame, which is bad
void InputSystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	//Check if joystick connected or not
	if (sf::Joystick::isConnected(0) != joystickConnected) 
	{
		if (sf::Joystick::isConnected(0))
			std::cout << "Joystick connected!\n";
		else
			std::cout << "Joystick disconnected!\n";

		joystickConnected = sf::Joystick::isConnected(0);
	}

	//Set direction to 0,0
	sf::Vector2f direction{ 0,0 };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		//Change direction to positive
		direction.y -= 1.f;
		lastInputByJoystick = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		//Change direction to negative
		direction.y += 1.f;
		lastInputByJoystick = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		direction.x -= 1.f;
		lastInputByJoystick = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		direction.x += 1.f;
		lastInputByJoystick = false;
	}

	if (joystickConnected)
	{
		//Get LEFT joysticks position
		float x = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

		if (abs(x) > minValForJoystick || abs(y) > minValForJoystick)
		{
			direction = sf::Vector2f{ x / 50.f, y / 50.f };
			lastInputByJoystick = true;
		}

		//Get RIGHT joysticks position
		float u = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
		float v = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V);

		//std::cout << "V: " << v<<'\n';
		if (abs(u) > minValForJoystick || abs(v) > minValForJoystick)
		{
			sf::Vector2i previousMousePos = ECSGame::Instance().GetMousePosition();
			float mouseSpeed = 100.f / mouseSpeedFromJoystick;
			//std::cout <<"Move Y: " << (int)roundf(v / mouseSpeed) << '\n';
			//lastMouseSpeed = sf::Vector2i{ previousMousePos.x + (int)roundf(u / mouseSpeed), previousMousePos.y + (int)roundf(v / mouseSpeed) };
			ECSGame::Instance().SetMousePosition(sf::Vector2i{ previousMousePos.x + (int)roundf(u / mouseSpeed), previousMousePos.y + (int)roundf(v / mouseSpeed) });
			lastInputByJoystick = true;
		}

		//Get LT and RT
		float z = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
		if (abs(z) > minValForJoystick)
		{
			lastInputByJoystick = true;
			ZoomCamera(z);
		}
	}

	//Deal with mouse movement
	sf::Vector2i mousePosition = ECSGame::Instance().GetMousePosition();
	std::shared_ptr<CameraComponent> spCamCom = GetCurrentlyActiveCamera();

	mousePosText->text->setString("Window pos: " + std::to_string(mousePosition.x) + "; " + std::to_string(mousePosition.y));
	sf::Vector2f positionInWorld = ConvertWindowPositionToWorld(spCamCom->view, mousePosition);
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
			std::shared_ptr<ObjectSystemComponent> spSysCom = spNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
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
			signals::onHideInfoPanel();
		}
		else
		{
			selectedSystemIcon->nodeToFollow = systemsNearBy[closestSystemIndex];
			signals::onShowInfoPanel(wpSelectedSystemNode);
		}

		systemsNearByText->text->setString(message);
	}
	else if (ECSGame::Instance().GetOverviewType() == OverviewType::System || ECSGame::Instance().GetOverviewType() == OverviewType::Planet)
	{
		bool selectPlanets = true;
		if (spCamCom->currentZoom > zoomAtWhichStartSelectPlanets && ECSGame::Instance().GetOverviewType() == OverviewType::System)
			selectPlanets = false;

		float maxDistance = ConvertWindowPositionToWorld(spCamCom->view, sf::Vector2i{ distanceFromMouseToIconToBeSelected, 0 }).x - ConvertWindowPositionToWorld(spCamCom->view, sf::Vector2i{ 0,0 }).x;
		//std::cout << "MaxDist: "<<maxDistance<<;

		SceneNodeVisitorGetClosestNodeToPosition visitor(positionInWorld, maxDistance, selectPlanets);
		if (ECSGame::Instance().GetOverviewType() == OverviewType::System)
		{
			visitor.currentOverview = OverviewType::System;
			wpSelectedSystemNode.lock()->AcceptVisitor(visitor);
		}
		else
		{
			visitor.currentOverview = OverviewType::Planet;
			wpPlanetOrStarSelected.lock()->AcceptVisitor(visitor);
		}

		if (visitor.wpClosestNode.lock() != nullptr)
		{
			std::shared_ptr<Entity> spE;
			if (ECSGame::Instance().GetOverviewType() == OverviewType::System)
			{
				wpPlanetOrStarSelected = visitor.wpClosestNode;
				selectedSystemIcon->nodeToFollow = wpPlanetOrStarSelected;
				spE = wpPlanetOrStarSelected.lock()->GetEntity().lock();
				signals::onShowInfoPanel(wpPlanetOrStarSelected);
			}
			else 
			{
				wpMoonOrPlanetSelected = visitor.wpClosestNode;
				spE = wpMoonOrPlanetSelected.lock()->GetEntity().lock();
				std::shared_ptr<PlanetComponent> spPlanetCom = spE->FindComponent<PlanetComponent>().lock();
				if(spPlanetCom->isMoon)
					selectedSystemIcon->nodeToFollow = wpMoonOrPlanetSelected;
				else
					selectedSystemIcon->nodeToFollow = wpMoonOrPlanetSelected.lock()->FindChild("PlanetPicture");
				
				signals::onShowInfoPanel(wpMoonOrPlanetSelected);
			}

			if (spE->HasComponent<StarComponent>())
				systemsNearByText->text->setString(spE->GetName() + " (" + spE->FindComponent<StarComponent>().lock()->starName + ")");
			else if (spE->HasComponent<PlanetComponent>())
			{
				std::shared_ptr<PlanetComponent> spPlanet = spE->FindComponent<PlanetComponent>().lock();
				systemsNearByText->text->setString(spE->GetName() + " (" + spPlanet->planetName + "; " + spPlanet->planetIconTextureName + "); size: " + std::to_string(spPlanet->planetSize));
			}
		}
		else 
		{
			selectedSystemIcon->nodeToFollow = {};
			systemsNearByText->text->setString(" ");
			signals::onHideInfoPanel();

			if (ECSGame::Instance().GetOverviewType() == OverviewType::System)
				wpPlanetOrStarSelected = {};
			else
				wpMoonOrPlanetSelected = {};
		}
	}
	else
		selectedSystemIcon->nodeToFollow = {};

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


void MovementSystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	//If game paused, then do nothing
	SceneNodeVisitorMovement visitor(*this);
	scene->AcceptVisitor(visitor);
	ui->AcceptVisitor(visitor);

	//direction = sf::Vector2{ 0.f,0.f };
}


//UI SYSTEM
void UISystem::Initialize() 
{
	//Subscribe to some signals
	signals::onRenderingComplete.connect(&UISystem::OnRenderingComplete, this);
	signals::onSystemOverviewSet.connect(&UISystem::OnSystemOverviewSet, this);
	signals::onPlanetOverviewSet.connect(&UISystem::OnSystemOverviewSet, this);
	signals::onShowInfoPanel.connect(&UISystem::OnShowInfoPanel, this);
	signals::onHideInfoPanel.connect(&UISystem::OnHideInfoPanel, this);

	std::shared_ptr<SceneNode> spNode = ECSGame::Instance().GetUIRoot()->FindChild("RenderText").lock();
	nodesText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("LowerPart").lock()->FindChild("MonthText").lock();
	monthText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("LowerPart").lock()->FindChild("DayText").lock();
	dayText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("LowerPart").lock()->FindChild("YearText").lock();
	yearText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("LowerPart").lock()->FindChild("SimulationStateText").lock();
	simStateText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("LowerPart").lock()->FindChild("SimulationSpeedText").lock();
	simSpeedText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("LowerPart").lock()->FindChild("ViewSizeText").lock();
	viewSizeText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("UpperPart").lock()->FindChild("OverviewText").lock();
	overviewText = spNode->GetEntity().lock()->FindComponent<TextComponent>().lock();

	spNode = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->FindChild("InfoText0").lock();
	infoText0 = spNode->GetEntity();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->FindChild("InfoText1").lock();
	infoText1 = spNode->GetEntity();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->FindChild("InfoText2").lock();
	infoText2 = spNode->GetEntity();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->FindChild("InfoText3").lock();
	infoText3 = spNode->GetEntity();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->FindChild("InfoText4").lock();
	infoText4 = spNode->GetEntity();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->FindChild("InfoText5").lock();
	infoText5 = spNode->GetEntity();
	spNode = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->FindChild("InfoText6").lock();
	infoText6 = spNode->GetEntity();

	wpInfoPanel = ECSGame::Instance().GetUIRoot()->FindChild("InfoPart").lock()->GetEntity();

	systemName = "UISystem";
}

void UISystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	nodesText.lock()->text->setString("Total nodes: "+std::to_string(numOfNodes)+"; rendered: "+std::to_string(nodesRendered));
	
	int days = 0;
	std::string month;
	int years = 0;
	GetDateFromDays((int)ECSGame::Instance().GetDaysPast(), days, month, years);

	dayText.lock()->text->setString(std::to_string(days));
	gel::CentreText(*dayText.lock()->text, sf::Vector2 { 0.f, 0.f });

	monthText.lock()->text->setString(month);
	gel::CentreText(*monthText.lock()->text, sf::Vector2 { 0.f, 0.f });

	yearText.lock()->text->setString(std::to_string(years));
	gel::CentreText(*yearText.lock()->text, sf::Vector2 { 0.f, 0.f });

	if (ECSGame::Instance().GetGameState() == GameState::Pause)
	{
		simStateText.lock()->text->setString("PAUSED");
		simStateText.lock()->text->setFillColor(sf::Color::Red);
	}
	else
	{
		simStateText.lock()->text->setString("RESUMED");
		simStateText.lock()->text->setFillColor(sf::Color::White);
	}
	gel::CentreText(*simStateText.lock()->text, sf::Vector2 { 0.f, 0.f });
	
	simSpeedText.lock()->text->setString("Simulation speed "+std::to_string((int)ECSGame::Instance().GetSimulationSpeed())+" days/second");
	gel::CentreText(*simSpeedText.lock()->text, sf::Vector2 { 0.f, 0.f });

	std::shared_ptr<CameraComponent> spCamCom = GetCurrentlyActiveCamera();
	OverviewType currentOverview = ECSGame::Instance().GetOverviewType();
	float size = spCamCom->view.getSize().x;
	std::string part{ "." };
	if (currentOverview != OverviewType::Planet)
	{
		if (size > 100)
			part = " ";
		else if ((int)size <= 0 && (int)((size - (int)size) * 100) < 10)
			part = ".0" + std::to_string((int)((size - (int)size) * 100));
		else
			part = "." + std::to_string((int)((size - (int)size) * 100));
	}
	else 
	{
		part = " ";
	}
	//std::cout << (int)size << '\n';

	if (currentOverview == OverviewType::Space)
		viewSizeText.lock()->text->setString("Current camera size is " + std::to_string((int)size)+part + " light years");
	else if(currentOverview == OverviewType::System)
		viewSizeText.lock()->text->setString("Current camera size is " + std::to_string((int)size) + part + " astronomical units");
	else if (currentOverview == OverviewType::Planet)
		viewSizeText.lock()->text->setString("Current camera size is " + std::to_string((int)(size*1000.f)) + part + " kilometers");
	gel::CentreText(*viewSizeText.lock()->text, sf::Vector2 { 0.f, 0.f });

	if (currentOverview == OverviewType::Space)
		overviewText.lock()->text->setString("Space Overview");
	else if(currentOverview == OverviewType::System)
		overviewText.lock()->text->setString(wpSystemNodeSelected.lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemName + " System");
	else if (currentOverview == OverviewType::Planet)
	{
		if (wpSystemNodeSelected.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
			overviewText.lock()->text->setString(wpSystemNodeSelected.lock()->GetEntity().lock()->FindComponent<PlanetComponent>().lock()->planetName + " Planet");
		else
			overviewText.lock()->text->setString("Planet Overview");
	}
	gel::CentreText(*overviewText.lock()->text, sf::Vector2 { 0.f, 0.f });

	SceneNodeVisitorUI visitor(*this, GetCurrentlyActiveCamera(), GetCameraFromUICameraEntity());
	ui->AcceptVisitor(visitor);
}

void UISystem::OnRenderingComplete(int numOfNodes, int nodesRendered)
{
	this->numOfNodes = numOfNodes;
	this->nodesRendered = nodesRendered;
}

void UISystem::OnSystemOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate)
{
	wpSystemNodeSelected = nodeToSimulate;
}



double convertRotationalVelocityIntoPeriod(double rotationalVelocity) 
{
	return (gel::PI * 2) / rotationalVelocity;
}



void UISystem::OnShowInfoPanel(std::weak_ptr<SceneNode> wpObjectNode) 
{
	wpInfoPanel.lock()->hidden = false;
	infoText0.lock()->hidden = false;
	infoText1.lock()->hidden = false;
	infoText2.lock()->hidden = false;
	infoText3.lock()->hidden = false;
	infoText4.lock()->hidden = false;
	infoText5.lock()->hidden = false;
	infoText6.lock()->hidden = false;

	std::shared_ptr<sf::Text> spText0 = infoText0.lock()->FindComponent<TextComponent>().lock()->text;
	std::shared_ptr<sf::Text> spText1 = infoText1.lock()->FindComponent<TextComponent>().lock()->text;
	std::shared_ptr<sf::Text> spText2 = infoText2.lock()->FindComponent<TextComponent>().lock()->text;
	std::shared_ptr<sf::Text> spText3 = infoText3.lock()->FindComponent<TextComponent>().lock()->text;
	std::shared_ptr<sf::Text> spText4 = infoText4.lock()->FindComponent<TextComponent>().lock()->text;
	std::shared_ptr<sf::Text> spText5 = infoText5.lock()->FindComponent<TextComponent>().lock()->text;
	std::shared_ptr<sf::Text> spText6 = infoText6.lock()->FindComponent<TextComponent>().lock()->text;

	std::shared_ptr<SystemPropertiesComponent> spProp = GetSystemPropertiesFromSpaceMap();

	bool showPlanetInfo = false;
	std::shared_ptr<Entity> spEntity = wpObjectNode.lock()->GetEntity().lock();
	if (ECSGame::Instance().GetOverviewType() == OverviewType::Space) 
	{
		std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();

		spText0->setString("Name: "+spSysCom->systemName);
		spText1->setString("Type: " + GetSystemTypeName(spSysCom->systemType));
		
		VisitorCountPlanets visitor(false);
		spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor);
		spText2->setString("Planets: " + std::to_string(visitor.counter));

		VisitorCountHabitablePlanets visitor2;
		spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor2);
		spText3->setString("Habitable planets: " + std::to_string(visitor2.counter));

		spText4->setString(" ");
		spText5->setString(" ");
		spText6->setString(" ");
	}
	else if (ECSGame::Instance().GetOverviewType() == OverviewType::System)
	{
		if (spEntity->HasComponent<StarComponent>()) 
		{
			std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();
			
			if (spEntity->GetName() == "InsideSystem") 
			{
				float period = convertRotationalVelocityIntoPeriod(spStarCom->rotationalVelocity);
				if(period<365*2)
					spText0->setString("Orbital period: " + std::to_string((int)period)+" days");
				else
					spText0->setString("Orbital period: " + std::to_string((int)(period/365)) + " years");
				spText1->setString("Orbit radius: " + gel::roundNumberForOutput(spStarCom->orbitRadius, 2)+ " AU");
				
				spText2->setString(" ");
				spText3->setString(" ");
				spText4->setString(" ");
				spText5->setString(" ");
				spText6->setString(" ");
			}
			else 
			{
				spText0->setString("Name: " + spStarCom->starName);
				spText1->setString("Type: " + GetStarTypeName(spStarCom->starType));
				spText2->setString("Mass: " + gel::roundNumberForOutput(spStarCom->starMass,2)+" solar masses");
				spText3->setString("Radius: " + gel::roundNumberForOutput(spStarCom->starSize, 2)+" solar radiuses");

				std::weak_ptr<ObjectSystemComponent> wpSysCom = wpObjectNode.lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>();
				if(wpSysCom.lock()==nullptr)
					wpSysCom = wpObjectNode.lock()->GetParent().lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>();
				
				if (wpSysCom.lock()!=nullptr)
				{
					if (wpSysCom.lock()->systemType != SpaceSystemType::Single)
					{
						float period = convertRotationalVelocityIntoPeriod(spStarCom->rotationalVelocity);
						if (period < 365 * 2)
							spText4->setString("Orbital period: " + std::to_string((int)period) + " days");
						else
							spText4->setString("Orbital period: " + std::to_string((int)(period / 365)) + " years");
						spText5->setString("Orbit radius: " + gel::roundNumberForOutput(spStarCom->orbitRadius, 2) + " AU");
					}
					else
					{
						spText4->setString(" ");
						spText5->setString(" ");
					}

				}
				else 
				{
					spText4->setString(" ");
					spText5->setString(" ");
				}

				spText6->setString(" ");
			}
		}
		else if (spEntity->HasComponent<PlanetComponent>()) 
		{
			showPlanetInfo = true;
		}
	}
	else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet) 
	{
		if (spEntity->HasComponent<PlanetComponent>())
		{
			showPlanetInfo = true;
		}
	}
	else 
	{
		spText0->setString(" ");
		spText1->setString(" ");
		spText2->setString(" ");
		spText3->setString(" ");
		spText4->setString(" ");
		spText5->setString(" ");
		spText6->setString(" ");
	}

	if (showPlanetInfo) 
	{
		std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();

		spText0->setString("Name: " + spPlanetCom->planetName);
		spText1->setString("Type: " + GetProperPlanetTypeName(spPlanetCom->planetType));
		if(ECSGame::Instance().GetOverviewType()==OverviewType::System)
			spText2->setString("Orbit radius: " + gel::roundNumberForOutput(spPlanetCom->orbitRadius,2) + " AU");
		else
			spText2->setString("Orbit radius: " + std::to_string((int)(spPlanetCom->orbitRadius*1000.f)) + " km");
		float period = convertRotationalVelocityIntoPeriod(spPlanetCom->rotationalVelocity);
		if (period < 365 * 2)
			spText3->setString("Orbital period: " + gel::roundNumberForOutput(period,1) + " days");
		else
			spText3->setString("Orbital period: " + gel::roundNumberForOutput(period / 365,1) + " years");
		spText4->setString("Radius: " + std::to_string((int)(spPlanetCom->planetSize * 500.f * spProp->mapConfig.earthDiameter)) + " km");
		spText5->setString("Mass: " + gel::roundNumberForOutput(gel::sphereVolume(spPlanetCom->planetSize / 2.f) * 2.f, 2) + " earth masses");

		if (!spPlanetCom->isMoon)
		{
			VisitorCountPlanets visitor(true);
			wpObjectNode.lock()->AcceptVisitor(visitor);
			spText6->setString("Moons: " + std::to_string(visitor.counter - 1));
		}
		else 
		{
			spText6->setString(" ");
		}
	}

	gel::AlignTextToLeftSide(*spText0, sf::Vector2 { 0.f, 0.f });
	gel::AlignTextToLeftSide(*spText1, sf::Vector2{ 0.f, 0.f });
	gel::AlignTextToLeftSide(*spText2, sf::Vector2{ 0.f, 0.f });
	gel::AlignTextToLeftSide(*spText3, sf::Vector2{ 0.f, 0.f });
	gel::AlignTextToLeftSide(*spText4, sf::Vector2{ 0.f, 0.f });
	gel::AlignTextToLeftSide(*spText5, sf::Vector2{ 0.f, 0.f });
	gel::AlignTextToLeftSide(*spText6, sf::Vector2{ 0.f, 0.f });
}

void UISystem::OnHideInfoPanel() 
{
	wpInfoPanel.lock()->hidden = true;
	infoText0.lock()->hidden = true;
	infoText1.lock()->hidden = true;
	infoText2.lock()->hidden = true;
	infoText3.lock()->hidden = true;
	infoText4.lock()->hidden = true;
	infoText5.lock()->hidden = true;
	infoText6.lock()->hidden = true;
}




//MUSIC SYSTEM
void MusicSystem::Initialize()
{
	
	systemName = "MusicSystem";
}

void MusicSystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	
}



//GAME SYSTEM
void GameSystem::Initialize()
{
	//Subscribe to some signals
	systemName = "GameSystem";
}



//Update systems visibility
void GameSystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	if (ECSGame::Instance().GetOverviewType()==OverviewType::Space)
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
	signals::onPlanetOverviewSet.connect(&SimulationSystem::OnSystemOverviewSet, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("DaysPastText").lock();
	daysPastText = mctPtr->GetEntity().lock()->FindComponent<TextComponent>().lock();

	std::shared_ptr<SceneNode> wctPtr = ECSGame::Instance().GetUIRoot()->FindChild("DateText").lock();
	dateText = wctPtr->GetEntity().lock()->FindComponent<TextComponent>().lock();
}

void SimulationSystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	daysPastText.lock()->text->setString("Simulation speed: " + std::to_string(ECSGame::Instance().GetSimulationSpeed()) + "; Days past: " + std::to_string((int)ECSGame::Instance().GetDaysPast()));
	int days = 0;
	std::string month;
	int years = 0;
	GetDateFromDays((int)ECSGame::Instance().GetDaysPast(), days, month, years);
	dateText.lock()->text->setString("Proper Date: " + std::to_string(days)+" "+month+std::to_string(years));

	if (ECSGame::Instance().GetOverviewType() == OverviewType::System) 
	{
		SceneNodeVisitorMoveObjectsInSystem visitor(false);
		wpNodeToSimulate.lock()->AcceptVisitor(visitor);
	}
	else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet)
	{
		SceneNodeVisitorMoveObjectsInSystem visitor(true);
		wpNodeToSimulate.lock()->AcceptVisitor(visitor);
	}
}

void SimulationSystem::OnSystemOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate)
{
	wpNodeToSimulate = nodeToSimulate;
}
