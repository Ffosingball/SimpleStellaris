#pragma once

#include "System.h"
//#include "Systems.h"
#include <SFML/Graphics.hpp>
#include "SceneNode.h"
#include <sigslot/signal.hpp>
#include "GameState.h"
#include <SFML/Audio.hpp>
#include "Components.h"


//Input system processes inputs
class InputSystem :public System 
{
public:
	virtual ~InputSystem() = default;
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;
	//Signals functions
	void OnKeyPressed(sf::Event::KeyPressed key);
	void OnKeyReleased(sf::Event::KeyReleased key);
	void OnMouseWheelScrolled(sf::Event::MouseWheelScrolled mw);
	void OnMouseMoved(sf::Event::MouseMoved mouseMovement);
	void OnMouseButtonPressed(sf::Event::MouseButtonPressed mouseButPressed);
	void OnJoystickMoved(sf::Event::JoystickMoved joystickMoved);
	void OnJoystickButtonPressed(sf::Event::JoystickButtonPressed button);
	void OnJoystickButtonReleased(sf::Event::JoystickButtonReleased button);

	void ZoomCamera(int direction);
	void EnterSystemOverview();
	void ExitSystemOverview();
	void EnterPlanetFromSystemOverview();
	void ExitPlanetToSystemOverview();

	std::shared_ptr<TextComponent> mousePosText{nullptr};
	std::shared_ptr<TextComponent> worldPosText{ nullptr };
	std::shared_ptr<TextComponent> systemsNearByText{ nullptr };
	std::shared_ptr<TextComponent> fpsText{ nullptr };
	std::vector<std::weak_ptr<Entity>> debugTextes;

	std::shared_ptr<UIFollowerComponent> selectedSystemIcon{ nullptr };
	std::shared_ptr<Entity> selectedSystemEntity{ nullptr };
	std::shared_ptr<Entity> mouseIconEntity{ nullptr };
	std::weak_ptr<SceneNode> wpPlanetOrStarSelected;
	std::weak_ptr<SceneNode> wpMoonOrPlanetSelected;

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

	OverviewType previousFrameOverview = OverviewType::None;

	std::weak_ptr<SceneNode> wpSelectedSystemNode;
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
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;

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