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

enum class GameState;
enum class OverviewType;

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
	std::shared_ptr<SceneNode> GetSceneRoot() { return sceneRoot; }
	//Worst case: O(1)
	std::shared_ptr<SceneNode> GetUIRoot() { return uiRoot; }
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

	//Override all methods from the parent
	void Init(sf::RenderWindow& renderWindow) override;
	void Update(const float deltaTime, sf::RenderWindow& renderWindow) override;
	void Render(sf::RenderWindow& renderWindow) override;
	void HandleEvent(const std::optional<sf::Event>& event) override;
	virtual ~ECSGame() = default;

private:

	ECSGame() = default;

	EntityManager entityManager;
	std::shared_ptr<SceneNode> sceneRoot;
	std::shared_ptr<SceneNode> uiRoot;
	//List of all systems is here
	std::vector<std::shared_ptr<System>> systems;

	float deltaTime{ 0.f };
	float timeSinceStart{ 0.f };
	sf::Vector2u windowSize{ 0,0 };
	sf::Vector2i mousePosition{ 0,0 };
	int previousFPS{ 0 };
	int fps{ 0 };
	//Stores current gameState
	GameState gameState;
	OverviewType overviewType;
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
	sf::RenderWindow* renderWindowPtr;
};