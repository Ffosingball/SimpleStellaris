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
	void Update(std::shared_ptr<SceneNode> scene, float deltaTime) override;
	//Signals functions
	void OnKeyPressed(sf::Event::KeyPressed key);
	void OnKeyReleased(sf::Event::KeyReleased key);
	void OnMouseWheelScrolled(sf::Event::MouseWheelScrolled mw);
	void OnMouseMoved(sf::Event::MouseMoved mouseMovement);
	void OnMouseButtonPressed(sf::Event::MouseButtonPressed mouseButPressed);

	std::shared_ptr<TextComponent> mousePosText{nullptr};
	std::shared_ptr<TextComponent> worldPosText{ nullptr };
	std::shared_ptr<TextComponent> systemsNearByText{ nullptr };
	std::shared_ptr<UIFollowerComponent> selectedSystemIcon{ nullptr };
	std::shared_ptr<Entity> selectedSystemEntity{ nullptr };
	std::shared_ptr<Entity> mouseIconEntity{ nullptr };
	std::shared_ptr<TextComponent> fpsText{ nullptr };

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
	void Update(std::shared_ptr<SceneNode> scene, float deltaTime) override;
	//Set direction to the new one
	void OnMoveCamera(sf::Vector2f direction) { this->direction = direction; }
};

//Processes UI
class UISystem :public System
{
public:
	DifficultyLevel level{DifficultyLevel::Medium};
	float zoomLevelAtWhichHideSystemNames{1.1f};
	virtual ~UISystem() = default;
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, float deltaTime) override;
};


//Manages Music
class MusicSystem :public System
{
public:
	//Pointer to sound and music
	//std::shared_ptr<sf::Music> mainMusic;
	//std::shared_ptr<sf::Sound> spExplosionSound;
	//std::shared_ptr<sf::Sound> spHurtSound;
	//std::shared_ptr<sf::Sound> spShotSound;
	//std::shared_ptr<sf::Sound> spWarningSound;
	//std::shared_ptr<sf::Sound> spPowerUpSound;
	//std::shared_ptr<sf::Sound> spWonSound;
	//Variables to play some sounds several times
	//int playExplosionSound{ 0 };
	//float timeLeftToPlaySound{ 0.f };
	//int playWonSound{ 0 };
	//float timeLeftToPlayWonSound{0.f};

	virtual ~MusicSystem() = default;
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, float deltaTime) override;
	//Signals functions
	//void OnGameRestart();
	//void OnPlayExplosionSound();
	//void OnPlayHurtSound(int n);
	//void OnPlayShotSound();
	//void OnPlayWarningSound();
	//void OnPlayPowerUpSound();
	//void OnPlayManyExplosionsSounds(int numOfTimesToPlay);
	//void OnPlayWonSound();
};



class GameSystem :public System
{
public:
	//Statistics variables

	virtual ~GameSystem() = default;
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, float deltaTime) override;
};


class SimulationSystem :public System
{
public:
	virtual ~SimulationSystem() = default;

	std::shared_ptr<TextComponent> daysPastText{ nullptr };
	std::shared_ptr<TextComponent> dateText{ nullptr };
private:
	void Initialize() override;
	void Update(std::shared_ptr<SceneNode> scene, float deltaTime) override;

	void OnSystemOverviewSet(std::shared_ptr<SceneNode> nodeToSimulate);

	std::shared_ptr<SceneNode> spNodeToSimulate;
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
}