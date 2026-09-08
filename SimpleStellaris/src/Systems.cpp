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
#include "ButtonsFunctions.h"


//INPUT SYSTEM
void InputSystem::Initialize() 
{
	//Subscribe functions to these signals
	signals::onKeyPressed.connect(&InputSystem::OnKeyPressed, this);
	signals::onKeyReleased.connect(&InputSystem::OnKeyReleased, this);
	signals::onMouseWheelScrolled.connect(&InputSystem::OnMouseWheelScrolled, this);
	signals::onMouseMoved.connect(&InputSystem::OnMouseMoved, this);
	signals::onMouseButtonPressed.connect(&InputSystem::OnMouseButtonPressed, this);
	signals::onMouseButtonReleased.connect(&InputSystem::OnMouseButtonReleased, this);
	signals::onJoystickMoved.connect(&InputSystem::OnJoystickMoved, this);
	signals::onJoystickButtonPressed.connect(&InputSystem::OnJoystickButtonPressed, this);
	signals::onJoystickButtonReleased.connect(&InputSystem::OnJoystickButtonReleased, this);
	signals::onChangeInputType.connect(&InputSystem::OnChangeInputType, this);
	signals::onTextEntered.connect(&InputSystem::OnTextEntered, this);
	signals::onInputBoxSelected.connect(&InputSystem::OnInputBoxSelected, this);

	previousFrameOverview = OverviewType::Space;
	systemName = "InputSystem";

	//Subscribe buttons
	ButtonSignals::OnResumeButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnExitButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnSlower3ButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnSlower2ButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnSlower1ButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnPlayingButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnStoppedButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnFaster3ButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnFaster2ButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnFaster1ButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnPreviousMusicButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnNextMusicButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnStopMusicButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnResumeMusicButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnMixMusicButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnExitToMainMenuButtonPressed.connect(&SetupPressedButtonTexture);
	ButtonSignals::OnStartGameButtonPressed.connect(&SetupPressedButtonTexture);

	ButtonSignals::OnResumeButtonPressed.connect(&InputSystem::ResumeButtonPressed, this);
	ButtonSignals::OnPlayingButtonPressed.connect(&InputSystem::PlayingButtonPressed, this);
	ButtonSignals::OnStoppedButtonPressed.connect(&InputSystem::StoppedButtonPressed, this);

	ButtonSignals::OnDistrictHovered.connect(&InputSystem::DistrictHovered, this);
	ButtonSignals::OnDistrictUnhovered.connect(&InputSystem::DistrictUnhovered, this);
	ButtonSignals::OnExitButtonPressed.connect(&ButtonSignals::ExitButtonPressed);
	ButtonSignals::OnButtonHovered.connect(&ButtonSignals::ButtonHovered);
	ButtonSignals::OnButtonUnhovered.connect(&ButtonSignals::ButtonUnhovered);
	ButtonSignals::OnButtonReleased.connect(&ButtonSignals::ButtonReleased);
	ButtonSignals::OnButtonClicked.connect(&ButtonSignals::ButtonClicked);
	ButtonSignals::OnExitToMainMenuButtonPressed.connect(&ButtonSignals::ExitToMainMenuButtonPressed);
	ButtonSignals::OnStartGameButtonPressed.connect(&ButtonSignals::StartGameButtonPressed);
	ButtonSignals::OnInputBoxPressed.connect(&ButtonSignals::InputBoxPressed);
	ButtonSignals::OnInputBoxHovered.connect(&ButtonSignals::InputBoxHovered);
	ButtonSignals::OnInputBoxUnhovered.connect(&ButtonSignals::InputBoxUnhovered);
	signals::onInputBoxUnselected.connect(&ButtonSignals::InputBoxUnselected);
}


void InputSystem::OnSceneChanged() 
{
	std::shared_ptr<SceneNode> mouseNodeSP = ECSGame::Instance().GetUINode()->FindChild("MouseIcon").lock();
	mouseIconEntity = mouseNodeSP->GetEntity().lock();

	wpInputRootNode = ECSGame::Instance().GetUINode();

	previousFrameOverview = OverviewType::Space;

	if (ECSGame::Instance().GetRoot()->GetEntity().lock()->GetName() == "SpaceWorldScene")
	{
		std::shared_ptr<SceneNode> spDebugNode = ECSGame::Instance().GetUINode()->FindChild("DebugPanel").lock();
		debugPanel = spDebugNode->GetEntity();

		std::shared_ptr<SceneNode> wsiPtr = ECSGame::Instance().GetUINode()->FindChild("SelectedSystemIcon").lock();
		selectedSystemIcon = wsiPtr->GetEntity().lock()->FindComponent<UIFollowerComponent>().lock();
		selectedSystemEntity = wsiPtr->GetEntity().lock();

		planetDistrictsPanel = ECSGame::Instance().GetUINode()->FindChild("PlanetDistrictsPart").lock();

		wpEscapeScreenNode = ECSGame::Instance().GetUINode()->FindChild("EscapeMenuScreen");
		wpStoppedButton = ECSGame::Instance().GetUINode()->FindChild("LowerPart").lock()->FindChild("StoppedButton").lock()->GetEntity();
		wpPlayingButton = ECSGame::Instance().GetUINode()->FindChild("LowerPart").lock()->FindChild("PlayingButton").lock()->GetEntity();

		wpSpaceSceneStates = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>();

		spaceMapScene = true;
		infoPanelIsShown = false;
		UIHidden = false;
		districtViewOpened = false;
		currentDistrictShown = -1;
	}
	else
		spaceMapScene = false;
}


void InputSystem::LockCameraOnNode(std::weak_ptr<SceneNode> wpNodeToLockOn)
{
	CancelCameraLock();

	std::shared_ptr<CameraComponent> spCameraCom = GetCurrentlyActiveCamera();
	spCameraCom->cameraLocked = true;
	spCameraCom->wpNodeLockedOn = wpNodeToLockOn;

	std::shared_ptr<Entity> spEntity = wpNodeToLockOn.lock()->GetEntity().lock();
	if ((spEntity->HasComponent<StarComponent>() || spEntity->HasComponent<PlanetComponent>()) && !spEntity->HasComponent<ObjectSystemComponent>())
		signals::onPlaySelectedObjectSound(spEntity);

	signals::onPlayLockCameraSFX();
}


void InputSystem::CancelCameraLock()
{
	std::shared_ptr<CameraComponent> spCameraCom = GetCurrentlyActiveCamera();
	spCameraCom->cameraLocked = false;
	spCameraCom->wpNodeLockedOn = {};

	signals::onStopSelectedObjectSound();
	signals::onPlayUnlockCameraSFX();
}


void InputSystem::OpenPlanetDistrictsView() 
{
	if (wpDistrictsOfPlanetShown.lock() != wpMoonOrPlanetSelected.lock())
	{
		if (wpDistrictsShown.lock() != nullptr)
			signals::onDeleteSceneNode(wpDistrictsShown);

		wpDistrictsOfPlanetShown = wpMoonOrPlanetSelected;
		std::shared_ptr<PlanetComponent> spPlanetCom = wpMoonOrPlanetSelected.lock()->GetEntity().lock()->FindComponent<PlanetComponent>().lock();
		if (spPlanetCom->planetDistrictsSeed != -1)
		{
			std::shared_ptr<SceneNode> spDistrictsNode = WorldGenerator::Instance().GenerateDistricts(spPlanetCom->planetDistrictsSeed, wpMoonOrPlanetSelected.lock());
			planetDistrictsPanel.lock()->AddChild(spDistrictsNode);
			wpDistrictsShown = spDistrictsNode;
		}

		planetDistrictsPanel.lock()->GetEntity().lock()->hidden = false;
		districtViewOpened = true;

		wpSpaceSceneStates.lock()->wpSelectedPlanet = spPlanetCom;

		signals::onPlayOpenDistrictViewSFX();
	}
}


void InputSystem::ClosePlanetDistrictsView()
{
	if (wpDistrictsShown.lock() != nullptr)
	{
		signals::onDeleteSceneNode(wpDistrictsShown);
		wpDistrictsShown = {};
		wpDistrictsOfPlanetShown = {};
	}

	planetDistrictsPanel.lock()->GetEntity().lock()->hidden = true;
	districtViewOpened = false;
	wpSpaceSceneStates.lock()->wpSelectedPlanet = {};

	signals::onPlayCloseDistrictViewSFX();
}


void InputSystem::EnterSystemOverview() 
{
	//Cancel camera lock
	CancelCameraLock();

	wpSpaceSceneStates.lock()->overviewType = OverviewType::System;
	ECSGame::Instance().GetSceneNode()->FindChild("SpaceMap").lock()->GetEntity().lock()->hidden = true;
	ECSGame::Instance().GetUINode()->FindChild("SystemNames").lock()->GetEntity().lock()->hidden = true;
	ECSGame::Instance().GetUINode()->FindChild("NebulasNames").lock()->GetEntity().lock()->hidden = true;

	std::shared_ptr<SceneNode> spSelectedSystemNode = wpSelectedSystemNode.lock();
	std::shared_ptr<ObjectSystemComponent> spSysCom = spSelectedSystemNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();

	SceneNodeVisitorChangeSingleSystemVisibility visitor2(false, ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock());
	spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor2);

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

	wpSpaceSceneStates.lock()->wpSelectedNodeIn = wpSelectedSystemNode;
	signals::onAddNodeToSimulate(spSysCom->spAllSystemObjectsNode);
	signals::onPlayEnterSelectedSystemSFX();
}


void InputSystem::EnterPlanetFromSystemOverview()
{
	//Cancel camera lock
	CancelCameraLock();

	wpSpaceSceneStates.lock()->overviewType = OverviewType::Planet;

	std::shared_ptr<SceneNode> spSelectedSystemNode = wpSelectedSystemNode.lock();
	std::shared_ptr<ObjectSystemComponent> spSysCom = spSelectedSystemNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
	SceneNodeVisitorChangeSingleSystemVisibility visitor(true, ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock());
	spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor);

	wpPlanetOrStarSelected.lock()->GetEntity().lock()->hidden = false;
	std::shared_ptr<Entity> spParentEntity = wpPlanetOrStarSelected.lock()->GetParent().lock()->GetEntity().lock();
	spParentEntity->hidden = false;
	if (spParentEntity->HasComponent<RectangleShapeComponent>())
	{
		std::shared_ptr<RectangleShapeComponent> spRectShape = spParentEntity->FindComponent<RectangleShapeComponent>().lock();
		sf::Color color = spRectShape->shape.getFillColor();
		color.a = 0;
		spRectShape->shape.setFillColor(color);
	}

	float earthDiameter = WorldGenerator::Instance().getSpaceMapConfig().earthDiameter;
	SceneNodeVisitorChangeSinglePlanetVisibility visitor2(false, ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock(), earthDiameter);
	wpPlanetOrStarSelected.lock()->AcceptVisitor(visitor2);

	//Setup planet camera
	std::shared_ptr<CameraComponent> sPlanetCameraCom = GetCameraFromPlanetCameraEntity();
	sPlanetCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
	sPlanetCameraCom->view.setSize(sPlanetCameraCom->cameraSize);
	sPlanetCameraCom->currentZoom = 1.f;
	sPlanetCameraCom->moveCamera = true;

	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->moveCamera = false;

	wpSpaceSceneStates.lock()->wpSelectedNodeIn = wpPlanetOrStarSelected;
	signals::onPlayEnterSelectedSystemSFX();
}


void InputSystem::ExitSystemOverview() 
{
	//Cancel camera lock
	CancelCameraLock();

	wpSpaceSceneStates.lock()->overviewType = OverviewType::Space;
	ECSGame::Instance().GetSceneNode()->FindChild("SpaceMap").lock()->GetEntity().lock()->hidden = false;
	ECSGame::Instance().GetUINode()->FindChild("SystemNames").lock()->GetEntity().lock()->hidden = false;
	ECSGame::Instance().GetUINode()->FindChild("NebulasNames").lock()->GetEntity().lock()->hidden = false;

	std::shared_ptr<SceneNode> spSelectedSystemNode = wpSelectedSystemNode.lock();
	std::shared_ptr<ObjectSystemComponent> spSysCom = spSelectedSystemNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
	SceneNodeVisitorChangeSingleSystemVisibility visitor(true, ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock());
	spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor);

	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->moveCamera = false;
	std::shared_ptr<CameraComponent> sSpaceCameraCom = GetCameraFromSpaceCameraEntity();
	sSpaceCameraCom->moveCamera = true;

	wpSpaceSceneStates.lock()->wpSelectedNodeIn = {};
	signals::onRemoveNodeToSimulate(spSysCom->spAllSystemObjectsNode);
	signals::onPlayExitSelectedSystemSFX();
}


void InputSystem::ExitPlanetToSystemOverview()
{
	//Cancel camera lock
	CancelCameraLock();

	wpSpaceSceneStates.lock()->overviewType = OverviewType::System;

	float earthDiameter = WorldGenerator::Instance().getSpaceMapConfig().earthDiameter;
	SceneNodeVisitorChangeSinglePlanetVisibility visitor2(true, ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock(), earthDiameter);
	wpPlanetOrStarSelected.lock()->AcceptVisitor(visitor2);

	std::shared_ptr<SceneNode> spSelectedSystemNode = wpSelectedSystemNode.lock();
	std::shared_ptr<ObjectSystemComponent> spSysCom = spSelectedSystemNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
	SceneNodeVisitorChangeSingleSystemVisibility visitor(false, ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock(), ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock());
	spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor);

	std::shared_ptr<CameraComponent> sSystemCameraCom = GetCameraFromSystemCameraEntity();
	sSystemCameraCom->moveCamera = true;
	std::shared_ptr<CameraComponent> sPlanetCameraCom = GetCameraFromPlanetCameraEntity();
	sPlanetCameraCom->moveCamera = false;

	wpSpaceSceneStates.lock()->wpSelectedNodeIn = wpSelectedSystemNode;
	signals::onPlayExitSelectedSystemSFX();
}


void ChangeUIVisibility(bool hide) 
{
	OverviewType overviewType = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock()->overviewType;
	if (overviewType == OverviewType::Space)
	{
		ECSGame::Instance().GetUINode()->FindChild("SystemNames").lock()->GetEntity().lock()->hidden = hide;
		ECSGame::Instance().GetUINode()->FindChild("NebulasNames").lock()->GetEntity().lock()->hidden = hide;
	}
	else if (hide == false) 
		ECSGame::Instance().GetUINode()->FindChild("SystemNames").lock()->GetEntity().lock()->hidden = true;
	
	if (overviewType == OverviewType::Planet)
	{
		ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock()->GetEntity().lock()->hidden = hide;
		ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock()->GetEntity().lock()->hidden = hide;
	}
	else if (overviewType == OverviewType::System)
	{
		ECSGame::Instance().GetUINode()->FindChild("ObjectOrbits").lock()->GetEntity().lock()->hidden = hide;
		std::shared_ptr<SceneNode> spNode = ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock();
		ChangeAllNodesVisibilityExceptStarIcons visitor2(hide);
		spNode->AcceptVisitor(visitor2);
		ECSGame::Instance().GetUINode()->FindChild("SystemIcons").lock()->GetEntity().lock()->hidden = false;
	}

	ECSGame::Instance().GetUINode()->FindChild("UpperPart").lock()->GetEntity().lock()->hidden = hide;
	ECSGame::Instance().GetUINode()->FindChild("LowerPart").lock()->GetEntity().lock()->hidden = hide;
	ECSGame::Instance().GetUINode()->FindChild("MusicPlayerPart").lock()->GetEntity().lock()->hidden = hide;
}


void InputSystem::PauseSimulation() 
{
	wpSpaceSceneStates.lock()->simulationState = GameState::Paused;
	signals::onPlayPauseSimulationSFX();
	wpPlayingButton.lock()->hidden = true;
	wpStoppedButton.lock()->hidden = false;
}


void InputSystem::ResumeSimulation()
{
	wpSpaceSceneStates.lock()->simulationState = GameState::Resumed;
	signals::onPlayResumeSimulationSFX();
	wpPlayingButton.lock()->hidden = false;
	wpStoppedButton.lock()->hidden = true;
}


void InputSystem::ChangeEscapeScreen() 
{
	if (!wpEscapeScreenNode.lock()->GetEntity().lock()->hidden)
	{
		wpEscapeScreenNode.lock()->GetEntity().lock()->hidden = true;
		wpInputRootNode = ECSGame::Instance().GetUINode();

		ECSGame::Instance().SetDeltaTimeMultiplier(1.f);
		OnChangeInputType(InputType::World);
		ECSGame::Instance().SetGameState(GameState::Resumed);
	}
	else 
	{
		wpEscapeScreenNode.lock()->GetEntity().lock()->hidden = false;
		wpInputRootNode = wpEscapeScreenNode;

		ECSGame::Instance().SetDeltaTimeMultiplier(0.f);
		OnChangeInputType(InputType::Menu);
		ECSGame::Instance().SetGameState(GameState::Paused);

		if (lastInputByJoystick) 
		{
			sf::Vector2f pos = ECSGame::Instance().GetUINode()->FindChild("EscapeMenuScreen").lock()->FindChild("ResumeButton").lock()->GetCombinedPosition();
			ECSGame::Instance().SetMousePosition(sf::Vector2i{(int)pos.x,(int)pos.y});
		}
	}

	signals::onPlayOpenEscapePanelSFX();
}



//Process keys they are pressed
void InputSystem::OnKeyPressed(sf::Event::KeyPressed key) 
{
	if (ECSGame::Instance().GetGameState() != GameState::Loading)
	{
		if (inputType == InputType::World)
		{
			if (key.code == sf::Keyboard::Key::Space)
			{
				if (wpSpaceSceneStates.lock()->simulationState == GameState::Resumed)
					PauseSimulation();
				else if (wpSpaceSceneStates.lock()->simulationState == GameState::Paused)
					ResumeSimulation();
			}
			else if (key.code == sf::Keyboard::Key::Q)
			{
				if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System)
					ExitSystemOverview();
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet && districtViewOpened)
					ClosePlanetDistrictsView();
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet)
					ExitPlanetToSystemOverview();
			}
			else if (key.code == sf::Keyboard::Key::Escape)
			{
				ChangeEscapeScreen();
			}
			else if (key.code == sf::Keyboard::Key::F3)
			{
				debugPanel.lock()->hidden = !debugPanel.lock()->hidden;
			}
			else if (key.code == sf::Keyboard::Key::Tab)
			{
				if (infoPanelIsShown)
					signals::onHideInfoPanel();
				else
					signals::onShowInfoPanel();
				infoPanelIsShown = !infoPanelIsShown;
			}
			else if (key.code == sf::Keyboard::Key::Z)
			{
				UIHidden = !UIHidden;
				ChangeUIVisibility(UIHidden);
			}
		}
		else if (inputType == InputType::Menu)
		{
			if (key.code == sf::Keyboard::Key::Escape)
			{
				ChangeEscapeScreen();
			}
		}
		else if (inputType == InputType::InputBox) 
		{
			std::shared_ptr<InputBoxComponent> spInputBox = wpInputBoxSelected.lock()->FindComponent<InputBoxComponent>().lock();

			if (key.code == sf::Keyboard::Key::Escape)
			{
				signals::onInputBoxUnselected(wpInputBoxSelected);
				inputType = previousInputType;
				wpInputBoxSelected = {};
				spInputBox->focused = false;
			}
			else if (key.code == sf::Keyboard::Key::Enter) 
			{
				signals::onInputBoxUnselected(wpInputBoxSelected);
				inputType = previousInputType;
				wpInputBoxSelected = {};
				spInputBox->focused = false;
			}
			else if (key.code == sf::Keyboard::Key::Backspace)
			{
				if (spInputBox->cursorPosition > 0)
				{
					spInputBox->text.erase(spInputBox->cursorPosition - 1, 1);
					spInputBox->cursorPosition--;
				}
			}
			else if (key.code == sf::Keyboard::Key::Delete)
			{
				if (spInputBox->cursorPosition < spInputBox->text.size())
					spInputBox->text.erase(spInputBox->cursorPosition + 1, 1);
			}
			else if (key.code == sf::Keyboard::Key::Left)
			{
				spInputBox->cursorPosition--;
				if (spInputBox->cursorPosition < 0)
					spInputBox->cursorPosition = 0;
			}
			else if (key.code == sf::Keyboard::Key::Right)
			{
				spInputBox->cursorPosition++;
				if (spInputBox->cursorPosition > spInputBox->text.size())
					spInputBox->cursorPosition = (int)spInputBox->text.size();
			}
		}

		lastInputByJoystick = false;
	}
}


void InputSystem::OnKeyReleased(sf::Event::KeyReleased key)
{
	//if (ECSGame::Instance().GetGameState() != GameState::Loading)
	//{
		//lastInputByJoystick = false;
	//}
}


void InputSystem::OnJoystickMoved(sf::Event::JoystickMoved joystickMoved)
{
	//if (ECSGame::Instance().GetGameState() != GameState::Loading)
	//{

	//}
}


void InputSystem::OnJoystickButtonPressed(sf::Event::JoystickButtonPressed button)
{
	if (ECSGame::Instance().GetGameState() != GameState::Loading)
	{
		if (inputType == InputType::World)
		{
			switch (button.button)
			{
			case 0:
				if (ECSGame::Instance().IsMouseOverUI())
					OnChangeInputType(InputType::Menu);
				else
				{
					if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Space && wpSelectedSystemNode.lock() != nullptr)
						EnterSystemOverview();
					else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr)
					{
						if (wpPlanetOrStarSelected.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
							EnterPlanetFromSystemOverview();
					}
					else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet && wpMoonOrPlanetSelected.lock() != nullptr)
					{
						if (wpMoonOrPlanetSelected.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
							OpenPlanetDistrictsView();
					}
				}

				signals::onLMBpressed();
				break;
			case 1:
				if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System)
					ExitSystemOverview();
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet && districtViewOpened)
					ClosePlanetDistrictsView();
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet)
					ExitPlanetToSystemOverview();
				break;
			case 2:
				if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Space && wpSelectedSystemNode.lock() != nullptr)
				{
					LockCameraOnNode(wpSelectedSystemNode);
				}
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr)
				{
					LockCameraOnNode(wpPlanetOrStarSelected);
				}
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet && wpMoonOrPlanetSelected.lock() != nullptr)
				{
					LockCameraOnNode(wpMoonOrPlanetSelected);
				}
				else
					CancelCameraLock();
				break;
			case 3:
				if (infoPanelIsShown)
					signals::onHideInfoPanel();
				else
					signals::onShowInfoPanel();
				infoPanelIsShown = !infoPanelIsShown;
				break;
			case 4:
				UIHidden = !UIHidden;
				ChangeUIVisibility(UIHidden);
				break;
			case 5:
				if (wpSpaceSceneStates.lock()->simulationState == GameState::Resumed)
					PauseSimulation();
				else if (wpSpaceSceneStates.lock()->simulationState == GameState::Paused)
					ResumeSimulation();
				break;
			case 6:
				ChangeEscapeScreen();
				OnChangeInputType(InputType::Menu);
				break;
			}
		}
		else if (inputType == InputType::Menu)
		{
			switch (button.button)
			{
			case 0:
				signals::onLMBpressed();
				break;
			case 1:
				if (ECSGame::Instance().GetGameState() != GameState::Paused)
					OnChangeInputType(InputType::World);
				break;
			case 2:
				if (ECSGame::Instance().GetGameState() != GameState::Paused)
					OnChangeInputType(InputType::World);
				break;
			case 5:
				if (wpSpaceSceneStates.lock()->simulationState == GameState::Resumed)
					PauseSimulation();
				else if (wpSpaceSceneStates.lock()->simulationState == GameState::Paused)
					ResumeSimulation();
				break;
			case 6:
				OnChangeInputType(InputType::World);
				ChangeEscapeScreen();
				break;
			}
		}
		else if (inputType == InputType::InputBox)
		{
			std::shared_ptr<InputBoxComponent> spInputBox = wpInputBoxSelected.lock()->FindComponent<InputBoxComponent>().lock();

			switch (button.button)
			{
			case 0:
				signals::onInputBoxUnselected(wpInputBoxSelected);
				inputType = previousInputType;
				wpInputBoxSelected = {};
				spInputBox->focused = false;
				break;
			case 1:
				if (spInputBox->cursorPosition >= spInputBox->text.size())if (spInputBox->cursorPosition > 0)
				{
					spInputBox->text.erase(spInputBox->cursorPosition - 1, 1);
					spInputBox->cursorPosition--;
				}
				break;
			case 2:
				if (spInputBox->cursorPosition < spInputBox->text.size())
					spInputBox->text.erase(spInputBox->cursorPosition + 1, 1);
				break;
			}
		}

		lastInputByJoystick = true;
	}
}


void InputSystem::OnJoystickButtonReleased(sf::Event::JoystickButtonReleased button)
{
	//if (ECSGame::Instance().GetGameState() != GameState::Loading)
	//{
		switch (button.button)
		{
		case 0:
			signals::onLMBreleased();
			break;
		}
	//}
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
	if (ECSGame::Instance().GetGameState() != GameState::Loading)
	{
		if (ECSGame::Instance().GetRoot()->GetEntity().lock()->GetName() == "SpaceWorldScene")
		{
			lastInputByJoystick = false;
			ZoomCamera((int)mw.delta);
		}
	}
}


void InputSystem::OnMouseMoved(sf::Event::MouseMoved mouseMovement) 
{
	//Move mouse
	mouseIconEntity.lock()->SetPosition({ (float)mouseMovement.position.x, (float)mouseMovement.position.y});
	//mouseIconEntity.lock()->hidden = false;
	//lastInputByJoystick = false;
}


void InputSystem::OnChangeInputType(InputType inType) 
{
	if (inType != inputType)
	{
		inputType = inType;
	}
}


void InputSystem::OnMouseButtonPressed(sf::Event::MouseButtonPressed mouseButPressed)
{
	if (ECSGame::Instance().GetGameState() != GameState::Loading)
	{
		if (inputType == InputType::World)
		{
			if (wpSpaceSceneStates.lock()->simulationState == GameState::Resumed && inputType == InputType::Menu)
				OnChangeInputType(InputType::World);

			if (mouseButPressed.button == sf::Mouse::Button::Left)
			{
				if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Space && wpSelectedSystemNode.lock() != nullptr)
				{
					EnterSystemOverview();
				}
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr)
				{
					if (wpPlanetOrStarSelected.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
						EnterPlanetFromSystemOverview();
				}
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet && wpMoonOrPlanetSelected.lock() != nullptr)
				{
					if (wpMoonOrPlanetSelected.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
						OpenPlanetDistrictsView();
				}

				signals::onLMBpressed();
			}
			else if (mouseButPressed.button == sf::Mouse::Button::Right)
			{
				if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Space && wpSelectedSystemNode.lock() != nullptr)
				{
					LockCameraOnNode(wpSelectedSystemNode);
				}
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System && wpPlanetOrStarSelected.lock() != nullptr)
				{
					LockCameraOnNode(wpPlanetOrStarSelected);
				}
				else if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet && wpMoonOrPlanetSelected.lock() != nullptr)
				{
					LockCameraOnNode(wpMoonOrPlanetSelected);
				}
				else
					CancelCameraLock();
			}
		}
		else if(inputType == InputType::Menu)
		{
			if (mouseButPressed.button == sf::Mouse::Button::Left)
			{
				signals::onLMBpressed();
			}
		}
		else if (inputType == InputType::InputBox)
		{
			if (mouseButPressed.button == sf::Mouse::Button::Left)
			{
				signals::onInputBoxUnselected(wpInputBoxSelected);
				inputType = previousInputType;
				wpInputBoxSelected = {};
				wpInputBoxSelected.lock()->FindComponent<InputBoxComponent>().lock()->focused = false;
				
				signals::onLMBpressed();
			}
		}

		lastInputByJoystick = false;
	}
}


void InputSystem::OnMouseButtonReleased(sf::Event::MouseButtonReleased mouseButReleased)
{
	if (mouseButReleased.button == sf::Mouse::Button::Left)
	{
		signals::onLMBreleased();
	}
}


void InputSystem::OnTextEntered(sf::Event::TextEntered textEntered) 
{
	if (inputType == InputType::InputBox)
	{
		if (textEntered.unicode >= 32 && textEntered.unicode != 127)
		{
			// Normal character
			std::shared_ptr<InputBoxComponent> spInputBox = wpInputBoxSelected.lock()->FindComponent<InputBoxComponent>().lock();
			spInputBox->text.insert(spInputBox->cursorPosition, 1, static_cast<char>(textEntered.unicode));
			spInputBox->cursorPosition++;
		}
	}
}


void InputSystem::OnInputBoxSelected(std::weak_ptr<Entity> wpInBoxSelected)
{
	wpInputBoxSelected = wpInBoxSelected;
	std::shared_ptr<InputBoxComponent> spInputBox = wpInputBoxSelected.lock()->FindComponent<InputBoxComponent>().lock();
	spInputBox->focused = true;
	spInputBox->timePassed = 0.f;
	spInputBox->cursorPosition = (int)spInputBox->text.size();
	previousInputType = inputType;
	inputType = InputType::InputBox;
}


//I process movement and fire keys in every frame, because game reacts to the key press
//on the same frame as it was pressed, and it will react every fram until the key
//is released. If I would use events, they are not called every frame, which is bad
void InputSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	timePassedSinceSelectedButton += ECSGame::Instance().GetUIDeltaTime();

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

	if (ECSGame::Instance().GetGameState() != GameState::Loading)
	{
		//Get continuous input
		if (inputType == InputType::World)
		{
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
					ZoomCamera((int)z);
				}
			}
		}
		else
		{
			if (joystickConnected && timePassedSinceSelectedButton >= selectNextButtonPeriod)
			{
				//Get Crest joysticks position
				float povX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);
				float povY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
				//Get right joystick movement
				float u = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
				float v = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

				//std::cout << "V: " << v<<'\n';
				if (abs(u) > minValForJoystick || abs(v) > minValForJoystick || abs(povX) > minValForJoystick || abs(povY) > minValForJoystick)
				{
					//Get Direction
					//Crest input has priority over right joystick
					int direction = -1;//0 top, 1 left, 2 bottom, 3 right
					if (abs(povX) > abs(povY) && abs(povX) > minValForJoystick)
					{
						if (povX < 0)
							direction = 3;
						else
							direction = 1;
					}
					else if (abs(povY) > minValForJoystick)
					{
						if (povY > 0)
							direction = 2;
						else
							direction = 0;
					}
					else if (abs(u) > abs(v) && abs(u) > minValForJoystick)
					{
						if (u < 0)
							direction = 3;
						else
							direction = 1;
					}
					else if (abs(v) > minValForJoystick)
					{
						if (v < 0)
							direction = 2;
						else
							direction = 0;
					}

					//Get Button to move to
					sf::Vector2i tempPos = ECSGame::Instance().GetMousePosition();
					VisitorGetClosestButtonAtDirection visitor(direction, sf::Vector2f{ (float)tempPos.x,(float)tempPos.y });
					wpInputRootNode.lock()->AcceptReverseVisitor(visitor);
					if (visitor.wpClosestButton.lock() != nullptr)
					{
						//Set new mouse position
						sf::Vector2f pos = visitor.wpClosestButton.lock()->GetCombinedPosition();
						ECSGame::Instance().SetMousePosition(sf::Vector2i{ (int)pos.x,(int)pos.y });
					}

					lastInputByJoystick = true;
					timePassedSinceSelectedButton = 0.f;
				}
			}
		}
	}

	if (lastInputByJoystick && inputType == InputType::Menu)
	{
		mouseIconEntity.lock()->hidden = true;
	}
	else
	{
		mouseIconEntity.lock()->hidden = false;
	}

	if (spaceMapScene)
	{
		//Deal with mouse movement
		sf::Vector2i mousePosition = ECSGame::Instance().GetMousePosition();
		std::shared_ptr<CameraComponent> spCamCom = GetCurrentlyActiveCamera();

		if (direction != sf::Vector2f{ 0,0 } && spCamCom->cameraLocked)
			CancelCameraLock();

		sf::Vector2f positionInWorld = ConvertWindowPositionToWorld(spCamCom->view, mousePosition);
		if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Space)
		{
			if (!ECSGame::Instance().IsMouseOverUI())
			{
				std::vector<std::shared_ptr<SceneNode>> systemsNearBy = GetAllSystemsNearPosition(positionInWorld);

				float closestDistance = 999999.f;
				int closestSystemIndex = -1;
				int counter{ 0 };
				for (std::shared_ptr<SceneNode> spNode : systemsNearBy)
				{
					std::shared_ptr<ObjectSystemComponent> spSysCom = spNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
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
					selectedSystemIcon.lock()->nodeToFollow = {};
					wpSelectedSystemNode = {};
					wpSpaceSceneStates.lock()->wpSelectedObjectByMouse = {};
				}
				else
				{
					selectedSystemIcon.lock()->nodeToFollow = systemsNearBy[closestSystemIndex];
					wpSpaceSceneStates.lock()->wpSelectedObjectByMouse = wpSelectedSystemNode;
				}
			}
			else
			{
				selectedSystemIcon.lock()->nodeToFollow = {};
				wpSelectedSystemNode = {};
				wpSpaceSceneStates.lock()->wpSelectedObjectByMouse = {};
			}
		}
		else if ((wpSpaceSceneStates.lock()->overviewType == OverviewType::System || wpSpaceSceneStates.lock()->overviewType == OverviewType::Planet))
		{
			if (!ECSGame::Instance().IsMouseOverUI())
			{
				bool selectPlanets = true;
				if ((spCamCom->currentZoom > zoomAtWhichStartSelectPlanets || UIHidden) && wpSpaceSceneStates.lock()->overviewType == OverviewType::System)
					selectPlanets = false;

				float maxDistance = ConvertWindowPositionToWorld(spCamCom->view, sf::Vector2i{ distanceFromMouseToIconToBeSelected, 0 }).x - ConvertWindowPositionToWorld(spCamCom->view, sf::Vector2i{ 0,0 }).x;
				//std::cout << "MaxDist: "<<maxDistance<<;

				SceneNodeVisitorGetClosestNodeToPosition visitor(positionInWorld, maxDistance, selectPlanets);
				if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System)
				{
					std::shared_ptr<SceneNode> spSelectedSystemNode = wpSelectedSystemNode.lock();
					std::shared_ptr<ObjectSystemComponent> spSysCom = spSelectedSystemNode->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock();
					visitor.currentOverview = OverviewType::System;
					spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor);
				}
				else
				{
					visitor.currentOverview = OverviewType::Planet;
					wpPlanetOrStarSelected.lock()->AcceptVisitor(visitor);
				}

				if (visitor.wpClosestNode.lock() != nullptr)
				{
					std::shared_ptr<Entity> spE;
					if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System)
					{
						wpPlanetOrStarSelected = visitor.wpClosestNode;
						selectedSystemIcon.lock()->nodeToFollow = wpPlanetOrStarSelected;
						spE = wpPlanetOrStarSelected.lock()->GetEntity().lock();
						wpSpaceSceneStates.lock()->wpSelectedObjectByMouse = wpPlanetOrStarSelected;
					}
					else
					{
						wpMoonOrPlanetSelected = visitor.wpClosestNode;
						spE = wpMoonOrPlanetSelected.lock()->GetEntity().lock();
						std::shared_ptr<PlanetComponent> spPlanetCom = spE->FindComponent<PlanetComponent>().lock();
						if (spPlanetCom->isMoon)
							selectedSystemIcon.lock()->nodeToFollow = wpMoonOrPlanetSelected;
						else
							selectedSystemIcon.lock()->nodeToFollow = wpMoonOrPlanetSelected.lock()->FindChild("PlanetPicture");

						wpSpaceSceneStates.lock()->wpSelectedObjectByMouse = wpMoonOrPlanetSelected;
					}
				}
				else
				{
					selectedSystemIcon.lock()->nodeToFollow = {};
					wpSpaceSceneStates.lock()->wpSelectedObjectByMouse = {};

					if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System)
						wpPlanetOrStarSelected = {};
					else
						wpMoonOrPlanetSelected = {};
				}
			}
			else
			{
				selectedSystemIcon.lock()->nodeToFollow = {};

				if (wpSpaceSceneStates.lock()->overviewType == OverviewType::System)
				{
					wpPlanetOrStarSelected = {};
					wpSpaceSceneStates.lock()->wpSelectedObjectByMouse = {};
				}
				//else
				//	wpMoonOrPlanetSelected = {};
			}
		}
		else
			selectedSystemIcon.lock()->nodeToFollow = {};

		if (previousFrameOverview != wpSpaceSceneStates.lock()->overviewType && ECSGame::Instance().GetGameState()!=GameState::Loading)
			ChangeUIVisibility(UIHidden);

		//Signal the direction to the movement system
		signals::onMoveCamera(direction);

		previousFrameOverview = wpSpaceSceneStates.lock()->overviewType;
	}
}



//MOVEMENT SYSTEM
void MovementSystem::Initialize()
{
	//Subscribe to this signal, to get player direction every frame
	signals::onMoveCamera.connect(&MovementSystem::OnMoveCamera, this);
	//signals::onZoomCamera.connect(&MovementSystem::OnZoomCamera, this);
	systemName = "MovementSystem";
}


void MovementSystem::OnSceneChanged()
{

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
	signals::onHideInfoPanel.connect(&UISystem::OnHideInfoPanel, this);
	signals::onShowInfoPanel.connect(&UISystem::OnShowInfoPanel, this);
	signals::onLMBpressed.connect(&UISystem::OnLMBpressed, this);
	signals::onLMBreleased.connect(&UISystem::OnLMBreleased, this);

	systemName = "UISystem";
}

void UISystem::OnSceneChanged()
{
	if (ECSGame::Instance().GetRoot()->GetEntity().lock()->GetName() == "SpaceWorldScene")
	{
		wpInfoPanel = ECSGame::Instance().GetUINode()->FindChild("InfoPart").lock()->GetEntity();
	}
}

void UISystem::ProcessFrontmostUIPart(std::weak_ptr<SceneNode> wpFrontmostNode, sf::Vector2f mousePosition)
{
	//Check that pointer is valid
	if (wpFrontmostNode.lock() != nullptr)
	{
		//std::cout <<"Frontmost entity is: "<< wpFrontmostNode.lock()->GetEntity().lock()->GetName()<<"\n";
		ECSGame::Instance().SetIsMouseOverUI(true);
	}
	else
		ECSGame::Instance().SetIsMouseOverUI(false);
}

void UISystem::OnLMBpressed()
{
	lmbPressed = true;
	buttonPressed = true;
}

void UISystem::OnLMBreleased()
{
	buttonPressed = false;
}

void UISystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	//Now get the frontmost ui part over which mouse is 
	sf::Vector2i mousePosition = ECSGame::Instance().GetMousePosition();
	SceneNodeVisitorFrontmostMouseHit frontVisitor(sf::Vector2f{ mousePosition });
	ECSGame::Instance().GetUINode()->AcceptReverseVisitor(frontVisitor);
	ProcessFrontmostUIPart(frontVisitor.wpFrontmostNode, sf::Vector2f{ mousePosition });

	//Now process all buttons
	std::weak_ptr<Entity> wpFrontEntity;
	if (frontVisitor.wpFrontmostNode.lock() != nullptr)
		wpFrontEntity = frontVisitor.wpFrontmostNode.lock()->GetEntity();

	SceneNodeVisitorButton visitorBut(*this, sf::Vector2f(mousePosition), wpFrontEntity);
	ECSGame::Instance().GetUINode()->AcceptVisitor(visitorBut);

	SceneNodeVisitorUIProcessHidden visitorProcHid(*this, GetCurrentlyActiveCamera(), GetCameraFromUICameraEntity());
	ECSGame::Instance().GetUINode()->AcceptVisitor(visitorProcHid);

	SceneNodeVisitorUI visitorUI(*this, GetCurrentlyActiveCamera(), GetCameraFromUICameraEntity());
	ECSGame::Instance().GetUINode()->AcceptVisitor(visitorUI);

	lmbPressed = false;
}

void UISystem::OnShowInfoPanel()
{
	wpInfoPanel.lock()->hidden = false;
}

void UISystem::OnHideInfoPanel() 
{
	wpInfoPanel.lock()->hidden = true;
}




//MUSIC SYSTEM
void MusicSystem::MixMusicList() 
{
	listOfMusicToPlay[currentMusicPlaying].lock()->pause();
	std::vector<std::weak_ptr<sf::Music>> newListToPlay;

	int musicSelected = gel::RandInt(0, (int)listOfMusicToPlay.size()-1);
	newListToPlay.push_back(listOfMusicToPlay[musicSelected]);
	listOfMusicToPlay[musicSelected] = listOfMusicToPlay.back();
	listOfMusicToPlay.pop_back();

	while (listOfMusicToPlay.size() > 0) 
	{
		musicSelected = gel::RandInt(0, (int)listOfMusicToPlay.size());
		newListToPlay.push_back(listOfMusicToPlay[musicSelected]);
		listOfMusicToPlay[musicSelected] = listOfMusicToPlay.back();
		listOfMusicToPlay.pop_back();
	}

	listOfMusicToPlay = newListToPlay;
	currentMusicPlaying = 0;
	SetupMusic(listOfMusicToPlay[currentMusicPlaying].lock());
}

void MusicSystem::SetupMusic(std::shared_ptr<sf::Music> currentlyPlayingMusic)
{
	currentlyPlayingMusic->setVolume(overallVolume * musicVolume * 100);
	sf::Time offset = sf::seconds(0.f);
	currentlyPlayingMusic->setPlayingOffset(offset);
}

void MusicSystem::PlayNextMusic()
{
	listOfMusicToPlay[currentMusicPlaying].lock()->setPlayingOffset(listOfMusicToPlay[currentMusicPlaying].lock()->getDuration()- goToNextMusicBefore);
}

void MusicSystem::PlayPreviousMusic()
{
	listOfMusicToPlay[currentMusicPlaying].lock()->pause();
	currentMusicPlaying--;
	if (currentMusicPlaying < 0)
		currentMusicPlaying = (int)listOfMusicToPlay.size() - 1;

	SetupMusic(listOfMusicToPlay[currentMusicPlaying].lock());
}

void MusicSystem::Initialize()
{
	systemName = "MusicSystem";

	signals::onPlayCloseDistrictViewSFX.connect(&MusicSystem::PlayCloseDistrictViewSFX, this);
	signals::onPlayOpenDistrictViewSFX.connect(&MusicSystem::PlayOpenDistrictViewSFX, this);
	signals::onPlayEnterSelectedSystemSFX.connect(&MusicSystem::PlayEnterSelectedSystemSFX, this);
	signals::onPlayExitSelectedSystemSFX.connect(&MusicSystem::PlayExitSelectedSystemSFX, this);
	signals::onPlayLockCameraSFX.connect(&MusicSystem::PlayLockCameraSFX, this);
	signals::onPlayUnlockCameraSFX.connect(&MusicSystem::PlayUnlockCameraSFX, this);
	signals::onPlayOpenEscapePanelSFX.connect(&MusicSystem::PlayOpenEscapePanelSFX, this);
	signals::onPlayPauseSimulationSFX.connect(&MusicSystem::PlayPauseSimulationSFX, this);
	signals::onPlayResumeSimulationSFX.connect(&MusicSystem::PlayResumeSimulationSFX, this);
	signals::onPlaySelectedObjectSound.connect(&MusicSystem::PlaySelectedObjectSound, this);
	signals::onStopSelectedObjectSound.connect(&MusicSystem::StopSelectedObjectSound, this);

	spEnterSelectedSystemSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("EnterSFX").lock());
	spExitSelectedSystemSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("ExitSFX").lock());
	spResumeSimulationSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("ResumeSFX").lock());
	spPauseSimulationSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("PauseSFX").lock());
	spLockCameraSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("LockCameraSFX").lock());
	spUnlockCameraSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("UnlockCameraSFX").lock());
	spOpenEscapeScreenSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("OpenEscapeScreenSFX").lock());
	spButtonPressedSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("ButtonPressedSFX").lock());
	spDistrictViewOpenedSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("DistrictViewOpenedSFX").lock());
	spDistrictViewClosedSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().GetSoundBuffer("DistrictViewClosedSFX").lock());

	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient1"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient2"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient3"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient4"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient5"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient6"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient7"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient8"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient9"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient10"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient11"));
	listOfMusicToPlay.push_back(ResourceManager::Instance().GetMusic("Ambient12"));

	//Subscribe buttons
	ButtonSignals::OnResumeButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnExitButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnSlower3ButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnSlower2ButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnSlower1ButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	//ButtonSignals::OnPlayingButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	//ButtonSignals::OnStoppedButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnFaster3ButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnFaster2ButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnFaster1ButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnPreviousMusicButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnNextMusicButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnStopMusicButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnResumeMusicButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnMixMusicButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnExitToMainMenuButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);
	ButtonSignals::OnStartGameButtonPressed.connect(&MusicSystem::PlayPressedButtonSFX, this);

	ButtonSignals::OnPreviousMusicButtonPressed.connect(&MusicSystem::PreviousMusicButtonPressed, this);
	ButtonSignals::OnNextMusicButtonPressed.connect(&MusicSystem::NextMusicButtonPressed, this);
	ButtonSignals::OnStopMusicButtonPressed.connect(&MusicSystem::StopMusicButtonPressed, this);
	ButtonSignals::OnResumeMusicButtonPressed.connect(&MusicSystem::ResumeMusicButtonPressed, this);
	ButtonSignals::OnMixMusicButtonPressed.connect(&MusicSystem::MixMusicButtonPressed, this);

	MixMusicList();
}

void MusicSystem::OnSceneChanged()
{
	StopSelectedObjectSound();
}

void MusicSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	std::shared_ptr<sf::Music> currentlyPlayingMusic = listOfMusicToPlay[currentMusicPlaying].lock();

	if (playMusic && !musicStopped && (currentlyPlayingMusic->getStatus() == sf::SoundSource::Status::Paused || listOfMusicToPlay[currentMusicPlaying].lock()->getStatus() == sf::SoundSource::Status::Stopped))
	{
		currentlyPlayingMusic->play();
	}
	else if((!playMusic || musicStopped) && currentlyPlayingMusic->getStatus() == sf::SoundSource::Status::Playing)
		currentlyPlayingMusic->pause();

	if (currentlyPlayingMusic->getDuration()-goToNextMusicBefore <= currentlyPlayingMusic->getPlayingOffset()) 
	{
		currentlyPlayingMusic->pause();
		currentMusicPlaying++;
		if (currentMusicPlaying >= listOfMusicToPlay.size())
			currentMusicPlaying = 0;

		SetupMusic(listOfMusicToPlay[currentMusicPlaying].lock());
		//else
		//	std::cout <<currentMusicPlaying<<") " << listOfMusicToPlay[currentMusicPlaying].lock()->getDuration().asSeconds() << '\n';
	}
}

void MusicSystem::PlayEnterSelectedSystemSFX() 
{
	spEnterSelectedSystemSound->setVolume(overallVolume * sfxVolume * 100);
	spEnterSelectedSystemSound->play();
}

void MusicSystem::PlayExitSelectedSystemSFX()
{
	spExitSelectedSystemSound->setVolume(overallVolume * sfxVolume * 100);
	spExitSelectedSystemSound->play();
}

void MusicSystem::PlayPauseSimulationSFX()
{
	spPauseSimulationSound->setVolume(overallVolume * sfxVolume * 100);
	spPauseSimulationSound->play();
}

void MusicSystem::PlayResumeSimulationSFX()
{
	spResumeSimulationSound->setVolume(overallVolume * sfxVolume * 100);
	spResumeSimulationSound->play();
}

void MusicSystem::PlayLockCameraSFX()
{
	spLockCameraSound->setVolume(overallVolume * sfxVolume * 100);
	spLockCameraSound->play();
}

void MusicSystem::PlayUnlockCameraSFX()
{
	spUnlockCameraSound->setVolume(overallVolume * sfxVolume * 100);
	spUnlockCameraSound->play();
}

void MusicSystem::PlayOpenDistrictViewSFX()
{
	spDistrictViewOpenedSound->setVolume(overallVolume * sfxVolume * 100);
	spDistrictViewOpenedSound->play();
}

void MusicSystem::PlayCloseDistrictViewSFX()
{
	spDistrictViewClosedSound->setVolume(overallVolume * sfxVolume * 100);
	spDistrictViewClosedSound->play();
}

void MusicSystem::PlayPressedButtonSFX(std::shared_ptr<Entity>)
{
	spButtonPressedSound->setVolume(overallVolume * sfxVolume * 100);
	spButtonPressedSound->play();
}

void MusicSystem::PlayOpenEscapePanelSFX()
{
	spOpenEscapeScreenSound->setVolume(overallVolume * sfxVolume * 100);
	spOpenEscapeScreenSound->play();
}


std::weak_ptr<sf::Music> GetSoundNameForSpaceObject(std::shared_ptr<Entity> spSelectedEntity)
{
	std::string soundName{ "Placeholder" };

	if (spSelectedEntity->HasComponent<StarComponent>()) 
	{
		std::shared_ptr<StarComponent> spStarCom = spSelectedEntity->FindComponent<StarComponent>().lock();

		switch (spStarCom->starType)
		{
		case StarType::BlackHole:
			soundName =  "Black Hole Sound";
			break;
		case StarType::NeutronStar:
			soundName = "Pulsar"+ std::to_string(spSelectedEntity->FindComponent<NeutronStarComponent>().lock()->pulsarNum) +" Sound";
			break;
		case StarType::WhiteDwarf:
			soundName = "White Dwarf Sound";
			break;
		case StarType::BrownDwarf:
			soundName = "Brown Dwarf Sound";
			break;
		case StarType::MredDwarf:
			soundName = "M Class Sound";
			break;
		case StarType::KorangeDwarf:
			soundName = "K Class Sound";
			break;
		case StarType::GsunLike:
			soundName = "G Class Sound";
			break;
		case StarType::Ftype:
			soundName = "F Class Sound";
			break;
		case StarType::Atype:
			soundName = "A Class Sound";
			break;
		case StarType::Btype:
			soundName = "B Class Sound";
			break;
		case StarType::Otype:
			soundName = "O Class Sound";
			break;
		case StarType::RedGiant:
			soundName = "Red Giant Sound";
			break;
		case StarType::RedSupergiant:
			soundName = "Red Supergiant Sound";
			break;
		}
	}
	else if (spSelectedEntity->HasComponent<PlanetComponent>())
	{
		std::shared_ptr<PlanetComponent> spPlanetCom = spSelectedEntity->FindComponent<PlanetComponent>().lock();
		std::weak_ptr<HabitablePlanetComponent> wpHabitablePlanet = spSelectedEntity->FindComponent<HabitablePlanetComponent>().lock();

		switch (spPlanetCom->planetType)
		{
		case PlanetType::BarrenDark:
			soundName = "Dark Barren Sound";
			break;
		case PlanetType::BarrenGrey:
			soundName = "Grey Barren Sound";
			break;
		case PlanetType::BarrenMarsLike:
			soundName = "Red Barren Sound";
			break;
		case PlanetType::VenusLike:
			soundName = "Venus Like Sound";
			break;
		case PlanetType::Oceanic:
			soundName = "Oceanic Sound";
			break;
		case PlanetType::EarthLike:
			if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
				soundName = "Earth Like Close Sound";
			else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
				soundName = "Earth Like Medium Sound";
			else
				soundName = "Earth Like Far Sound";
			break;
		case PlanetType::TitanLike:
			soundName = "Titan Like Sound";
			break;
		case PlanetType::Molten:
			soundName = "Molten Sound";
			break;
		case PlanetType::Icy:
			soundName = "Icy Sound";
			break;
		case PlanetType::Voulcanic:
			soundName = "Voulcanic Sound";
			break;
		case PlanetType::Desert:
			soundName = "Desert Sound";
			break;
		case PlanetType::HotJupiter:
		case PlanetType::HotNeptune:
			soundName = "Hot Gas Planet Sound";
			break;
		case PlanetType::JupiterLike:
			soundName = "Jupiter Like Sound";
			break;
		case PlanetType::SaturnLike:
			soundName = "Saturn Like Sound";
			break;
		case PlanetType::NeptuneLike:
			soundName = "Neptune Like Sound";
			break;
		case PlanetType::UranusLike:
			soundName = "Uranus Like Sound";
			break;
		}
	}

	return ResourceManager::Instance().GetMusic(soundName);
}


void MusicSystem::PlaySelectedObjectSound(std::shared_ptr<Entity> spSelectedEntity)
{
	wpSelectedObjectSound = GetSoundNameForSpaceObject(spSelectedEntity);
	wpSelectedObjectSound.lock()->setLooping(true);
	wpSelectedObjectSound.lock()->setVolume(overallVolume*musicVolume*100);
	sf::Time playAt = sf::seconds(gel::Randf(0.f, wpSelectedObjectSound.lock()->getDuration().asSeconds()));
	wpSelectedObjectSound.lock()->setPlayingOffset(playAt);
	wpSelectedObjectSound.lock()->play();
	playMusic = false;
}


void MusicSystem::StopSelectedObjectSound() 
{
	if(wpSelectedObjectSound.lock()!=nullptr)
		wpSelectedObjectSound.lock()->stop();
	playMusic = true;
}

void MusicSystem::StopMusic() 
{
	musicStopped = true;
}

void MusicSystem::ResumeMusic()
{
	musicStopped = false;
}



//GAME SYSTEM
void GameSystem::Initialize()
{
	//Subscribe to some signals
	systemName = "GameSystem";
}

void GameSystem::OnSceneChanged()
{
	if (ECSGame::Instance().GetRoot()->GetEntity().lock()->GetName() == "SpaceWorldScene")
	{
		wpSpaceSceneStates = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>();

		spaceMapScene = true;
	}
	else
		spaceMapScene = false;
}

//Update systems visibility
void GameSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	if (spaceMapScene)
	{
		if (wpSpaceSceneStates.lock()->overviewType == OverviewType::Space)
		{
			SceneNodeVisitorSystemVisibility visitor(GetCurrentlyActiveCamera());
			scene->AcceptVisitor(visitor);
		}
	}
}



//SIMULATION SYSTEM
void SimulationSystem::Initialize()
{
	systemName = "SimulationSystem";
	signals::onAddNodeToSimulate.connect(&SimulationSystem::OnAddNodeToSimulate, this);
	signals::onRemoveNodeToSimulate.connect(&SimulationSystem::OnRemoveNodeToSimulate, this);

	ButtonSignals::OnSlower3ButtonPressed.connect(&SimulationSystem::Slower3ButtonPressed, this);
	ButtonSignals::OnSlower2ButtonPressed.connect(&SimulationSystem::Slower2ButtonPressed, this);
	ButtonSignals::OnSlower1ButtonPressed.connect(&SimulationSystem::Slower1ButtonPressed, this);
	ButtonSignals::OnFaster3ButtonPressed.connect(&SimulationSystem::Faster3ButtonPressed, this);
	ButtonSignals::OnFaster2ButtonPressed.connect(&SimulationSystem::Faster2ButtonPressed, this);
	ButtonSignals::OnFaster1ButtonPressed.connect(&SimulationSystem::Faster1ButtonPressed, this);
}

void SimulationSystem::OnSceneChanged()
{
	if (ECSGame::Instance().GetRoot()->GetEntity().lock()->GetName() == "SpaceWorldScene")
	{
		std::weak_ptr<Entity> wpObjOrb = ECSGame::Instance().GetEntityManager().NewEntity("SimulationNode");
		ECSGame::Instance().GetSceneNode()->AddChild(std::make_shared<SceneNode>(wpObjOrb));
		wpSimulationNode = ECSGame::Instance().GetSceneNode()->FindChild("SimulationNode");
	
		wpSpaceSceneStates = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>();

		spaceMapScene = true;
	}
	else
		spaceMapScene = false;
}

void SimulationSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	if (spaceMapScene)
	{
		if (wpSpaceSceneStates.lock()->simulationState == GameState::Resumed)
		{
			wpSpaceSceneStates.lock()->daysPast += deltaTime * wpSpaceSceneStates.lock()->simulationSpeed;

			GetDateFromDays((int)wpSpaceSceneStates.lock()->daysPast, wpSpaceSceneStates.lock()->day, wpSpaceSceneStates.lock()->month, wpSpaceSceneStates.lock()->year);
		}

		SceneNodeVisitorMoveObjectsInSystem visitor(wpSpaceSceneStates.lock()->daysPast);
		wpSimulationNode.lock()->AcceptVisitor(visitor);
	}
}

void SimulationSystem::OnAddNodeToSimulate(std::shared_ptr<SceneNode> nodeToSimulate)
{
	wpSimulationNode.lock()->AddChild(nodeToSimulate);
}

void SimulationSystem::OnRemoveNodeToSimulate(std::shared_ptr<SceneNode> nodeToNotSimulate)
{
	wpSimulationNode.lock()->RemoveNode(nodeToNotSimulate);
}
