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
	signals::onJoystickMoved.connect(&InputSystem::OnJoystickMoved, this);
	signals::onJoystickButtonPressed.connect(&InputSystem::OnJoystickButtonPressed, this);
	signals::onJoystickButtonReleased.connect(&InputSystem::OnJoystickButtonReleased, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("MouseCoordsText").lock();
	mousePosText = GetTextComponent(*mctPtr->GetEntity().lock());
	debugTextes.push_back(mctPtr->GetEntity());

	std::shared_ptr<SceneNode> wctPtr = ECSGame::Instance().GetUIRoot()->FindChild("WorldCoordsText").lock();
	worldPosText = GetTextComponent(*wctPtr->GetEntity().lock());
	debugTextes.push_back(wctPtr->GetEntity());

	std::shared_ptr<SceneNode> wsnPtr = ECSGame::Instance().GetUIRoot()->FindChild("SystemsNearByText").lock();
	systemsNearByText = GetTextComponent(*wsnPtr->GetEntity().lock());
	debugTextes.push_back(wsnPtr->GetEntity());

	std::shared_ptr<SceneNode> wfpsPtr = ECSGame::Instance().GetUIRoot()->FindChild("FPSText").lock();
	fpsText = GetTextComponent(*wfpsPtr->GetEntity().lock());
	debugTextes.push_back(wfpsPtr->GetEntity());

	std::shared_ptr<SceneNode> wsiPtr = ECSGame::Instance().GetUIRoot()->FindChild("SelectedSystemIcon").lock();
	selectedSystemIcon = GetUIFollowerComponent(*wsiPtr->GetEntity().lock());
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


void InputSystem::EnterSystemOverview() 
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
	ECSGame::Instance().SetOverviewType(OverviewType::Planet);

	SceneNodeVisitorChangeSingleSystemVisibility visitor(true, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
	wpSelectedSystemNode.lock()->AcceptVisitor(visitor);

	SceneNodeVisitorChangeSinglePlanetVisibility visitor2(false, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
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
	ECSGame::Instance().SetOverviewType(OverviewType::Space);

	SceneNodeVisitorChangeAllSystemVisibility visitor(false);
	ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);

	SceneNodeVisitorChangeSingleSystemVisibility visitor2(true, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
	wpSelectedSystemNode.lock()->AcceptVisitor(visitor2);

	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->moveCamera = false;
	std::shared_ptr<CameraComponent> sSpaceCameraCom = GetCameraFromSpaceCameraEntity();
	sSpaceCameraCom->moveCamera = true;
}


void InputSystem::ExitPlanetToSystemOverview()
{
	ECSGame::Instance().SetOverviewType(OverviewType::System);

	SceneNodeVisitorChangeSinglePlanetVisibility visitor2(true, ECSGame::Instance().GetUIRoot()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUIRoot()->FindChild("ObjectOrbits").lock());
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
			if (wpPlanetOrStarSelected.lock()->GetEntity().lock()->HasComponent(ComponentType::Planet))
				EnterPlanetFromSystemOverview();
		}
		break;
	case 1:
		if(ECSGame::Instance().GetOverviewType() == OverviewType::System)
			ExitSystemOverview();
		else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet)
			ExitPlanetToSystemOverview();
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

	if (!lastInputByJoystick)
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
			if (wpPlanetOrStarSelected.lock()->GetEntity().lock()->HasComponent(ComponentType::Planet))
				EnterPlanetFromSystemOverview();
		}
	}

	lastInputByJoystick = false;
}


//I process movement and fire keys in every frame, because game reacts to the key press
//on the same frame as it was pressed, and it will react every fram until the key
//is released. If I would use events, they are not called every frame, which is bad
void InputSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
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
	else if (ECSGame::Instance().GetOverviewType() == OverviewType::System)
	{
		bool selectPlanets = false;
		if (spCamCom->currentZoom < zoomAtWhichStartSelectPlanets)
			selectPlanets = true;

		float maxDistance = ConvertWindowPositionToWorld(spCamCom->view, sf::Vector2i{ distanceFromMouseToIconToBeSelected, 0 }).x - ConvertWindowPositionToWorld(spCamCom->view, sf::Vector2i{ 0,0 }).x;
		//std::cout << "MaxDist: "<<maxDistance<<;

		SceneNodeVisitorGetClosestNodeToPosition visitor(positionInWorld, maxDistance, selectPlanets);
		wpSelectedSystemNode.lock()->AcceptVisitor(visitor);

		if (visitor.wpClosestNode.lock() != nullptr)
		{
			wpPlanetOrStarSelected = visitor.wpClosestNode;
			selectedSystemIcon->nodeToFollow = wpPlanetOrStarSelected;

			std::shared_ptr<Entity> spE = wpPlanetOrStarSelected.lock()->GetEntity().lock();
			if (spE->HasComponent(ComponentType::Star))
				systemsNearByText->text->setString(spE->GetName() + " (" + GetStarComponent(*spE)->starName + ")");
			else if (spE->HasComponent(ComponentType::Planet))
			{
				std::shared_ptr<PlanetComponent> spPlanet = GetPlanetComponent(*spE);
				systemsNearByText->text->setString(spE->GetName() + " (" + spPlanet->planetName + "; " + spPlanet->planetIconTextureName + "); size: " + std::to_string(spPlanet->planetSize));
			}
		}
		else 
		{
			selectedSystemIcon->nodeToFollow = {};
			wpPlanetOrStarSelected = {};
			systemsNearByText->text->setString(" ");
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


void MovementSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	//If game paused, then do nothing
	SceneNodeVisitorMovement visitor(*this);
	scene->AcceptVisitor(visitor);

	//direction = sf::Vector2{ 0.f,0.f };
}


//UI SYSTEM
void UISystem::Initialize() 
{
	//Subscribe to some signals
	signals::onRenderingComplete.connect(&UISystem::OnRenderingComplete, this);
	signals::onSystemOverviewSet.connect(&UISystem::OnSystemOverviewSet, this);
	signals::onPlanetOverviewSet.connect(&UISystem::OnSystemOverviewSet, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("RenderText").lock();
	nodesText = GetTextComponent(*mctPtr->GetEntity().lock());
	std::shared_ptr<SceneNode> sp2 = ECSGame::Instance().GetUIRoot()->FindChild("MonthText").lock();
	monthText = GetTextComponent(*sp2->GetEntity().lock());
	std::shared_ptr<SceneNode> sp3 = ECSGame::Instance().GetUIRoot()->FindChild("DayText").lock();
	dayText = GetTextComponent(*sp3->GetEntity().lock());
	std::shared_ptr<SceneNode> sp4 = ECSGame::Instance().GetUIRoot()->FindChild("YearText").lock();
	yearText = GetTextComponent(*sp4->GetEntity().lock());
	std::shared_ptr<SceneNode> sp5 = ECSGame::Instance().GetUIRoot()->FindChild("SimulationStateText").lock();
	simStateText = GetTextComponent(*sp5->GetEntity().lock());
	std::shared_ptr<SceneNode> sp6 = ECSGame::Instance().GetUIRoot()->FindChild("SimulationSpeedText").lock();
	simSpeedText = GetTextComponent(*sp6->GetEntity().lock());
	std::shared_ptr<SceneNode> sp7 = ECSGame::Instance().GetUIRoot()->FindChild("ViewSizeText").lock();
	viewSizeText = GetTextComponent(*sp7->GetEntity().lock());
	std::shared_ptr<SceneNode> sp8 = ECSGame::Instance().GetUIRoot()->FindChild("OverviewText").lock();
	overviewText = GetTextComponent(*sp8->GetEntity().lock());

	systemName = "UISystem";
}

void UISystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
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
		overviewText.lock()->text->setString(GetObjectSystemComponent(*wpSystemNodeSelected.lock()->GetEntity().lock())->systemName + " System");
	else if (currentOverview == OverviewType::Planet)
	{
		if (wpSystemNodeSelected.lock()->GetEntity().lock()->HasComponent(ComponentType::Planet))
			overviewText.lock()->text->setString(GetPlanetComponent(*wpSystemNodeSelected.lock()->GetEntity().lock())->planetName + " Planet");
		else
			overviewText.lock()->text->setString("Planet Overview");
	}
	gel::CentreText(*overviewText.lock()->text, sf::Vector2 { 0.f, 0.f });

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

void UISystem::OnSystemOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate)
{
	wpSystemNodeSelected = nodeToSimulate;
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
	signals::onPlanetOverviewSet.connect(&SimulationSystem::OnSystemOverviewSet, this);

	std::shared_ptr<SceneNode> mctPtr = ECSGame::Instance().GetUIRoot()->FindChild("DaysPastText").lock();
	daysPastText = GetTextComponent(*mctPtr->GetEntity().lock());

	std::shared_ptr<SceneNode> wctPtr = ECSGame::Instance().GetUIRoot()->FindChild("DateText").lock();
	dateText = GetTextComponent(*wctPtr->GetEntity().lock());
}

void SimulationSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
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
