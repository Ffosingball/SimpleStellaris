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

enum class GameState;

//This is a base of the main game loop
class ECSGame : public gel::SFMLApplication
{
public:
	//Disallow copying
	ECSGame(const ECSGame&) = delete;
	ECSGame& operator=(const ECSGame&) = delete;

	//Create a single access to the object of this class
	static ECSGame& Instance() 
	{
		static ECSGame ecsGame;
		return ecsGame;
	}

	//Getters
	float GetDeltaTime() const { return deltaTime; }
	EntityManager& GetEntityManager() { return entityManager; }
	SceneNode& GetSceneRoot() { return sceneRoot; }
	GameState GetGameState() const { return gameState; }
	sf::Vector2u GetWindowSize() const { return windowSize; }

	//Setter
	void SetGameState(GameState gState) { gameState=gState; }
	void CloseGame() { closeGame = true; }

	//Override all methods from the parent
	void Init(sf::RenderWindow& renderWindow) override;
	void Update(const float deltaTime) override;
	void Render(sf::RenderWindow& renderWindow) override;
	void HandleEvent(const std::optional<sf::Event>& event) override;
	virtual ~ECSGame() = default;

private:

	ECSGame() = default;

	EntityManager entityManager;
	SceneNode sceneRoot;
	//List of all systems is here
	std::vector<std::shared_ptr<System>> systems;

	float deltaTime{ 0.f };
	float timeSinceStart{ 0.f };
	sf::Vector2u windowSize{ 0,0 };
	//Stores current gameState
	GameState gameState;
	//Boolean to check if window should be closed or not
	bool closeGame{false};
	//System which deletes all required entities at the end of the frame
	DeleteSystem deleteSystem;
};