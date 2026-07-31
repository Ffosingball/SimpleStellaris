#include "SFMLApplication.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SceneNode.h"
#include <vector>
#include "Systems.h"
#include "System.h"
#include <memory>
#include "ECSGame.h"
#include <SFML/Graphics.hpp>
#include "SceneNodeVisitors.h"
#include <iostream>
#include "EntitiesFunctions.h"
#include "SpaceObjectTypes.h"
//#include "GameState.h"


//Initialize game
void ECSGame::Init(sf::RenderWindow& renderWindow) 
{
	//I noticed, that random function generates same random numbers every time when I start
	//my game again, so to solve this issue I seed it with current time at the start of the
	//game
	std::srand(std::time(nullptr));
	//Get window size
	windowSize = renderWindow.getSize();
	
	//Create and sdd systems to the list
	systems.emplace_back(std::make_shared<InputSystem>());
	systems.emplace_back(std::make_shared<MovementSystem>());
	systems.emplace_back(std::make_shared<UISystem>());
	systems.emplace_back(std::make_shared<MusicSystem>());
	systems.emplace_back(std::make_shared<GameSystem>());

	//Initialize map first, so it will be always drawn first
	InitializeTileMapAndCamera(windowSize);
	//Initialize UI camera, to draw ui entities
	InitializeUICamera(windowSize);

	//Create nodes, in which i will sort new entities which will be created during the game
	std::weak_ptr<Entity> wpPlay = ECSGame::Instance().GetEntityManager().NewEntity("SpaceMap");
	ECSGame::Instance().GetSceneRoot().AddChild(SceneNode(wpPlay));
	std::weak_ptr<Entity> wpUI = ECSGame::Instance().GetEntityManager().NewEntity("UI");
	ECSGame::Instance().GetSceneRoot().AddChild(SceneNode(wpUI));

	//Create initial entities of ui

	//Initialize Systems
	for (std::shared_ptr<System> system : systems)
		system->Initialize();

	//Initialize object removal system
	deleteSystem.Initialize();

	//Set gameState
	gameState = GameState::Game;
}


void ECSGame::Update(const float deltaTime)
{
	//Change deltatime
	this->deltaTime = deltaTime;
	timeSinceStart += deltaTime;

	//std::cout << "Update\n";
	//Update all systems
	for (std::shared_ptr<System> system : systems)
		system->Update(sceneRoot, deltaTime);

	//Process entities removal
	deleteSystem.Update(sceneRoot, deltaTime);
}


void ECSGame::HandleEvent(const std::optional<sf::Event>& event) 
{
	//Check if any key is pressed
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
	{
		signals::onKeyPressed(*keyPressed);
	}

	//Check if key released
	if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		signals::onKeyReleased(*keyReleased);
	}

	//Check if mouse wheel scrolled
	if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
	{
		signals::onMouseWheelScrolled(*mouseWheelScrolled);
	}
}

//Render all entities
void ECSGame::Render(sf::RenderWindow& renderWindow)
{
	//Check if we need to close a game
	if (closeGame)
		renderWindow.close();

	//Set renderWindow to render in the camera
	std::shared_ptr<CameraComponent> sCameraCom = GetCameraFromCameraEntity();
	renderWindow.setView(sCameraCom->view);

	//Render all scene entities
	SceneNodeVisitorRender visitor(renderWindow);
	sceneRoot.AcceptVisitor(visitor);

	//Set renderWindow to render UI
	std::shared_ptr<CameraComponent> sUICameraCom = GetCameraFromUICameraEntity();
	renderWindow.setView(sUICameraCom->view);

	//Render all UI entities
	SceneNodeVisitorRenderUI visitor2(renderWindow);
	sceneRoot.AcceptVisitor(visitor2);
}