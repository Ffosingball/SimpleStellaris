#pragma once

#include "System.h"
//#include "Systems.h"
#include <SFML/Graphics.hpp>
#include "SceneNode.h"
#include <sigslot/signal.hpp>
#include "GameState.h"
#include <SFML/Audio.hpp>
#include "Components.h"

class MusicSystem;


//Input system processes inputs
class InputSystem :public System 
{
public:
	virtual ~InputSystem() = default;

	//Buttons functions
	void DistrictHovered(std::shared_ptr<Entity> spEntity);
	void DistrictUnhovered(std::shared_ptr<Entity> spEntity);
	void ButtonHovered(std::shared_ptr<Entity> spEntity);
	void ButtonUnhovered(std::shared_ptr<Entity> spEntity);
	void ButtonReleased(std::shared_ptr<Entity> spEntity);
	void ButtonClicked(std::shared_ptr<Entity> spEntity);
	void ResumeButtonPressed(std::shared_ptr<Entity> spEntity);
	void ExitButtonButtonPressed(std::shared_ptr<Entity> spEntity);

	std::shared_ptr<MusicSystem> musicSystem;

	//Check if left mouse button or A on joystick is presed which controls button 
	bool lmbPressed{ false };
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;
	//Signals functions
	void OnKeyPressed(sf::Event::KeyPressed key);
	void OnKeyReleased(sf::Event::KeyReleased key);
	void OnMouseWheelScrolled(sf::Event::MouseWheelScrolled mw);
	void OnMouseMoved(sf::Event::MouseMoved mouseMovement);
	void OnMouseButtonPressed(sf::Event::MouseButtonPressed mouseButPressed);
	void OnMouseButtonReleased(sf::Event::MouseButtonReleased mouseButReleased);
	void OnJoystickMoved(sf::Event::JoystickMoved joystickMoved);
	void OnJoystickButtonPressed(sf::Event::JoystickButtonPressed button);
	void OnJoystickButtonReleased(sf::Event::JoystickButtonReleased button);

	void ZoomCamera(int direction);
	void EnterSystemOverview();
	void ExitSystemOverview();
	void EnterPlanetFromSystemOverview();
	void ExitPlanetToSystemOverview();
	void LockCameraOnNode(std::weak_ptr<SceneNode> wpNodeToLockOn);
	void CancelCameraLock();
	void PauseSimulation();
	void ResumeSimulation();
	void OpenPlanetDistrictsView();
	void ClosePlanetDistrictsView();
	void ProcessFrontmostUIPart(std::weak_ptr<SceneNode> wpFrontmostNode, sf::Vector2f mousePosition);
	void ChangeEscapeScreen();

	std::shared_ptr<TextComponent> mousePosText{nullptr};
	std::shared_ptr<TextComponent> worldPosText{ nullptr };
	std::shared_ptr<TextComponent> systemsNearByText{ nullptr };
	std::shared_ptr<TextComponent> fpsText{ nullptr };
	std::shared_ptr<TextComponent> mouseOverUIText{ nullptr };
	std::vector<std::weak_ptr<Entity>> debugTextes;
	std::shared_ptr<TextComponent> districtTypeText{ nullptr };
	std::shared_ptr<TextComponent> planetNameText{ nullptr };

	std::shared_ptr<UIFollowerComponent> selectedSystemIcon{ nullptr };
	std::shared_ptr<Entity> selectedSystemEntity{ nullptr };
	std::shared_ptr<Entity> mouseIconEntity{ nullptr };
	std::weak_ptr<SceneNode> wpSelectedSystemNode;
	std::weak_ptr<SceneNode> wpPlanetOrStarSelected;
	std::weak_ptr<SceneNode> wpMoonOrPlanetSelected;
	std::shared_ptr<SceneNode> planetDistrictsPanel;
	std::weak_ptr<Entity> wpDistrictsShown;
	std::weak_ptr<SceneNode> wpEscapeScreenNode;

	bool ctrlHold{ false };
	bool shiftHold{ false };

	bool joystickConnected = false;
	bool lastInputByJoystick = false;
	float minValForJoystick = 5.f;
	float mouseSpeedFromJoystick = 14.f;
	//How many times zoom on joystick slower than on mouse
	float zoomSpeedJoystickSlowing= 0.2f;
	//sf::Vector2i lastMouseSpeed{ 0,0 };
	bool showDebugText = false;
	float zoomAtWhichStartSelectPlanets{0.8f};
	//In window pixels
	int distanceFromMouseToIconToBeSelected{60};
	bool infoPanelIsShown{ false };
	bool UIHidden{ false };
	bool districtViewOpened{ false };
	int currentDistrictShown = -1;

	OverviewType previousFrameOverview = OverviewType::None;
};

//Processes all movement
class MovementSystem :public System
{
public:
	//Camera direction
	sf::Vector2f direction;

	virtual ~MovementSystem() = default;
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;
	//Set direction to the new one
	void OnMoveCamera(sf::Vector2f direction) { this->direction = direction; }
};

//Processes UI
class UISystem :public System
{
public:
	DifficultyLevel level{DifficultyLevel::Medium};
	//float zoomLevelAtWhichHideSystemNames{1.1f};

	virtual ~UISystem() = default;
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;

	void OnSystemOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate);

	std::weak_ptr<TextComponent> nodesText;
	int numOfNodes{ 0 };
	int nodesRendered{ 0 };

	std::weak_ptr<TextComponent> monthText;
	std::weak_ptr<TextComponent> dayText;
	std::weak_ptr<TextComponent> yearText;
	std::weak_ptr<TextComponent> simStateText;
	std::weak_ptr<TextComponent> simSpeedText;
	std::weak_ptr<TextComponent> viewSizeText;
	std::weak_ptr<TextComponent> overviewText;

	std::weak_ptr<Entity> infoText0;
	std::weak_ptr<Entity> infoText1;
	std::weak_ptr<Entity> infoText2;
	std::weak_ptr<Entity> infoText3;
	std::weak_ptr<Entity> infoText4;
	std::weak_ptr<Entity> infoText5;
	std::weak_ptr<Entity> infoText6;
	std::weak_ptr<Entity> wpInfoPanel;

	std::weak_ptr<SceneNode> wpSystemNodeSelected;
	std::weak_ptr<SceneNode> wpObjectNodeToTellInfoAbout;

	void OnRenderingComplete(int numOfNodes, int nodesRendered);
	void OnUpdateInfoPanel(std::weak_ptr<SceneNode> wpObjectNode);
	void OnClearInfoPanel();
	void OnHideInfoPanel();
	void OnShowInfoPanel();
};


//Manages Music
class MusicSystem :public System
{
public:

	virtual ~MusicSystem() = default;

	void PlayEnterSelectedSystemSFX();
	void PlayExitSelectedSystemSFX();
	void PlayPauseSimulationSFX();
	void PlayResumeSimulationSFX();
	void PlayLockCameraSFX();
	void PlayUnlockCameraSFX();

	void PlaySelectedObjectSound(std::shared_ptr<Entity> spSelectedEntity);
	void StopSelectedObjectSound();
	void PlayNextMusic();

private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;

	void MixMusicList();

	std::shared_ptr<sf::Sound> spEnterSelectedSystemSound;
	std::shared_ptr<sf::Sound> spExitSelectedSystemSound;
	std::shared_ptr<sf::Sound> spPauseSimulationSound;
	std::shared_ptr<sf::Sound> spResumeSimulationSound;
	std::shared_ptr<sf::Sound> spLockCameraSound;
	std::shared_ptr<sf::Sound> spUnlockCameraSound;

	std::vector<std::weak_ptr<sf::Music>> listOfMusicToPlay;

	std::weak_ptr<sf::Music> wpSelectedObjectSound;

	//Volume is in range from 0 to 1
	float sfxVolume{ 0.7f };
	float musicVolume{ 1.f };
	float overallVolume{ 1.f };
	int currentMusicPlaying{ 0 };
	bool playMusic{ true };
	sf::Time goToNextMusicBefore = sf::seconds(1.f);
};



class GameSystem :public System
{
public:
	//Statistics variables

	virtual ~GameSystem() = default;
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;
};


class SimulationSystem :public System
{
public:
	virtual ~SimulationSystem() = default;

private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;

	void OnSystemOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate);
	//void OnPlanetOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate);

	std::weak_ptr<TextComponent> daysPastText;
	std::weak_ptr<TextComponent> dateText;

	std::weak_ptr<SceneNode> wpNodeToSimulate;
};


//List of all possible signals
namespace signals
{
	inline sigslot::signal<sf::Event::KeyPressed> onKeyPressed;
	inline sigslot::signal<sf::Event::KeyReleased> onKeyReleased;
	inline sigslot::signal<sf::Event::MouseWheelScrolled> onMouseWheelScrolled;
	inline sigslot::signal<sf::Event::MouseMoved> onMouseMoved;
	inline sigslot::signal<sf::Event::MouseButtonPressed> onMouseButtonPressed;
	inline sigslot::signal<sf::Event::MouseButtonReleased> onMouseButtonReleased;
	inline sigslot::signal<sf::Vector2f> onMoveCamera;
	inline sigslot::signal<std::shared_ptr<SceneNode>> onSystemOverviewSet;
	inline sigslot::signal<std::shared_ptr<SceneNode>> onPlanetOverviewSet;
	inline sigslot::signal<int, int> onRenderingComplete;
	inline sigslot::signal<sf::Event::JoystickMoved> onJoystickMoved;
	inline sigslot::signal<sf::Event::JoystickButtonPressed> onJoystickButtonPressed;
	inline sigslot::signal<sf::Event::JoystickButtonReleased> onJoystickButtonReleased;
	inline sigslot::signal<std::weak_ptr<SceneNode>> onUpdateInfoPanel;
	inline sigslot::signal<> onHideInfoPanel;
	inline sigslot::signal<> onShowInfoPanel;
	inline sigslot::signal<> onClearInfoPanel;
}