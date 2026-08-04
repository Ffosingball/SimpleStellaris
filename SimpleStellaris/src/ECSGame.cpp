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
#include "WorldGenerator.h"
#include "CommonGameCode.h"


//Initialize game
void ECSGame::Init(sf::RenderWindow& renderWindow) 
{
	//Create scene root
	sceneRoot = std::make_shared<SceneNode>();
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

	WorldGenerator::Initialize((unsigned int)gel::Randf(1000000.f, 9999999.f));
	//Initialize map first, so it will be always drawn first
	InitializeCamera(windowSize);
	//Initialize UI camera, to draw ui entities
	InitializeUICamera(windowSize);

	//Create nodes, in which i will sort new entities which will be created during the game
	std::weak_ptr<Entity> wpPlay = ECSGame::Instance().GetEntityManager().NewEntity("SpaceMap");
	ECSGame::Instance().GetSceneRoot()->AddChild(std::make_shared<SceneNode>(wpPlay));
	wpPlay.lock()->AddComponent(ComponentType::SystemProperties);
	std::weak_ptr<Entity> wpUI = ECSGame::Instance().GetEntityManager().NewEntity("UI");
	ECSGame::Instance().GetSceneRoot()->AddChild(std::make_shared<SceneNode>(wpUI));
	//Setup mouseIcon
	InitializeMouseIcon();

	//Create debug text
	CreateDebugText();
	CreateUI();

	//Initialize Systems
	for (std::shared_ptr<System> system : systems)
		system->Initialize();

	//Initialize object removal system
	deleteSystem.Initialize();

	//Create initial entities
	CreateSpaceObjects();

	//Set gameState
	gameState = GameState::Game;
}


void ECSGame::Update(const float deltaTime, sf::RenderWindow& renderWindow)
{
	//Change deltatime
	this->deltaTime = deltaTime;
	timeSinceStart += deltaTime;

	//Get mousePosition
	mousePosition = sf::Mouse::getPosition(renderWindow);

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

	//Check if mouse wheel scrolled
	if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
	{
		signals::onMouseMoved(*mouseMoved);
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
	sceneRoot->AcceptVisitor(visitor);

	//Set renderWindow to render UI
	std::shared_ptr<CameraComponent> sUICameraCom = GetCameraFromUICameraEntity();
	renderWindow.setView(sUICameraCom->view);

	//Render all UI entities
	SceneNodeVisitorRenderUI visitor2(renderWindow);
	sceneRoot->AcceptVisitor(visitor2);
}