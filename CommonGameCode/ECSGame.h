#pragma once
#include "SFMLApplication.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SceneNode.h"
#include <vector>
#include "System.h"
#include <memory>
#include "ECSGame.h"
#include <SFML/Graphics.hpp>
#include "CompilerInstructions.h"
#include "GameState.h"

//If you need to change a whole scene just call change scene method

//This is a base of the main game loop
class ECSGame : public gel::SFMLApplication
{
public:
	//Disallow copying
	ECSGame(const ECSGame&) = delete;
	ECSGame& operator=(const ECSGame&) = delete;

	//Create a single access to the object of this class
	//Worst case: O(1)
	static ECSGame& Instance() 
	{
		static ECSGame ecsGame;
		return ecsGame;
	}

	//Getters
	//Worst case: O(1)
	float GetDeltaTime() const { return deltaTime; }
	//Worst case: O(1)
	EntityManager& GetEntityManager() { return entityManager; }
	//Worst case: O(1)
	std::shared_ptr<SceneNode> GetRoot() { return root; }
	//Worst case: O(1)
	std::shared_ptr<SceneNode> GetUINode() { return uiNode; }
	//Worst case: O(1)
	std::shared_ptr<SceneNode> GetSceneNode() { return sceneNode; }
	//Worst case: O(1)
	GameState GetGameState() const { return gameState; }
	//Worst case: O(1)
	OverviewType GetOverviewType() const { return overviewType; }
	//Worst case: O(1)
	sf::Vector2u GetWindowSize() const { return windowSize; }
	//Worst case: O(1)
	float GetUISize() const { return windowRelationToDefault; }
	//Worst case: O(1)
	sf::Vector2i GetMousePosition() const { return mousePosition; }
	//Worst case: O(1)
	int GetFPS() const { return previousFPS; }
	//Worst case: O(1)
	float GetDaysPast() const { return daysPast; }
	//Worst case: O(1)
	float GetSimulationSpeed() const { return simulationSpeed; }
	//Worst case: O(1)
	float GetSimulationDeltaTime() const;
	//Worst case: O(1)
	bool IsMouseOverUI() const { return isMouseOverUI; }
	//Worst case: O(1)
	float GetUIDeltaTime() const { return uiDeltaTime; }

	//Getter and setter
	float GetDeltaTimeMultiplier() { return deltaTimeMultiplier; }
	void SetDeltaTimeMultiplier(float dtMultiplier) { deltaTimeMultiplier = dtMultiplier; }

	//Setter
	//Worst case: O(1)
	void SetGameState(GameState gState) { gameState=gState; }
	//Worst case: O(1)
	void SetOverviewType(OverviewType ovType) { overviewType = ovType; }
	//Worst case: O(1)
	void CloseGame() { closeGame = true; }
	//Worst case: O(1)
	void SetSimulationSpeed(float simSpeed);
	//Worst case: O(1)
	void SetMousePosition(sf::Vector2i newMousePos) const;
	//Worst case: O(1)
	void SetIsMouseOverUI(bool mouseOverUI) { isMouseOverUI = mouseOverUI; }
	//Worst case: O(1)
	void ChangeScene(std::shared_ptr<SceneNode> newSceneNode, std::shared_ptr<SceneNode> newUINode);

	//Override all methods from the parent
	void Init(sf::RenderWindow& renderWindow) override;
	void Update(const float deltaTime, sf::RenderWindow& renderWindow) override;
	void Render(sf::RenderWindow& renderWindow) override;
	void HandleEvent(const std::optional<sf::Event>& event) override;
	virtual ~ECSGame() = default;

private:

	ECSGame() = default;

	EntityManager entityManager;
	std::shared_ptr<SceneNode> root;
	std::shared_ptr<SceneNode> uiNode;
	std::shared_ptr<SceneNode> sceneNode;
	//List of all systems is here
	std::vector<std::shared_ptr<System>> systems;

	float deltaTime{ 0.f };
	//Time passed since start of the game
	float timeSinceStart{ 0.f };
	sf::Vector2u windowSize{ 0,0 };
	sf::Vector2i mousePosition{ 0,0 };
	//Previous will be used to output the fps
	int previousFPS{ 0 };
	//This is used to count fps of the current second
	int fps{ 0 };
	//Stores current gameState
	GameState gameState = GameState::None;
	//Stores current overview type
	OverviewType overviewType = OverviewType::None;
	//Boolean to check if window should be closed or not
	bool closeGame{false};
	//System which deletes all required entities at the end of the frame
	DeleteSystem deleteSystem;
	//Simulation time in days
	float daysPast{ 0.f };
	//Simulation speed
	float simulationSpeed{ 10.f };
	//This is needed to keep UI size the same on every monitor
	float windowRelationToDefault{ 1.f };
	//Pointer to the window
	sf::RenderWindow* renderWindowPtr = nullptr;
	//Flag if mouse over ui part or not!
	bool isMouseOverUI{false};

	float deltaTimeMultiplier{ 1.f };
	float uiDeltaTime{0.f};
};