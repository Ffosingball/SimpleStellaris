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
	uiRoot = std::make_shared<SceneNode>();
	//I noticed, that random function generates same random numbers every time when I start
	//my game again, so to solve this issue I seed it with current time at the start of the
	//game
	std::srand(std::time(nullptr));
	//Get window size
	windowSize = renderWindow.getSize();

	//Load all resources
	ResourceManager::Instance().LoadAllResources();
	
	//Create and sdd systems to the list
	systems.emplace_back(std::make_shared<InputSystem>());
	systems.emplace_back(std::make_shared<MovementSystem>());
	systems.emplace_back(std::make_shared<UISystem>());
	systems.emplace_back(std::make_shared<MusicSystem>());
	systems.emplace_back(std::make_shared<GameSystem>());
	systems.emplace_back(std::make_shared<SimulationSystem>());

	WorldGenerator::Initialize((unsigned int)gel::Randf(1000000.f, 9999999.f));

	//Create nodes, in which i will sort new entities which will be created during the game
	std::weak_ptr<Entity> wpPlay = entityManager.NewEntity("SpaceMap");
	sceneRoot->AddChild(std::make_shared<SceneNode>(wpPlay));
	wpPlay.lock()->AddComponent(ComponentType::SystemProperties);

	std::weak_ptr<Entity> wpCameras = entityManager.NewEntity("Cameras");
	sceneRoot->AddChild(std::make_shared<SceneNode>(wpCameras));

	std::weak_ptr<Entity> wpSysN = entityManager.NewEntity("SystemNames");
	uiRoot->AddChild(std::make_shared<SceneNode>(wpSysN));
	
	//Initialize all cameras
	InitializeAllCameras(windowSize);
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
	gameState = GameState::Pause;
	overviewType = OverviewType::Space;

	//Output full sceneNode tree for debbuging
	//sceneRoot->OutputTree(" ");
}


void ECSGame::Update(const float deltaTime, sf::RenderWindow& renderWindow)
{
	//Change deltatime
	this->deltaTime = deltaTime;
	timeSinceStart += deltaTime;

	if (timeSinceStart > 1.f)
	{
		timeSinceStart -= 1.f;
		previousFPS = fps;
		fps = 0.f;
	}
	fps++;

	//Get mousePosition
	mousePosition = sf::Mouse::getPosition(renderWindow);

	//Update simulation time
	if (ECSGame::Instance().GetGameState() != GameState::Pause)
	{
		daysPast += deltaTime * simulationSpeed;
	}

	//for debbuging purposes
	//DEB: sf::Clock timer;

	//std::cout << "Update\n";
	//Update all systems
	for (std::shared_ptr<System> system : systems)
	{
		system->Update(sceneRoot, deltaTime);
		system->Update(uiRoot, deltaTime);
		//DEB: std::cout << "  --"<<system->GetSystemName()<<": " << timer.restart().asSeconds() << '\n';
	}

	//Process entities removal
	deleteSystem.Update(sceneRoot, deltaTime);
	deleteSystem.Update(uiRoot, deltaTime);
	//DEB: std::cout << "  --" << deleteSystem.GetSystemName() << ": " << timer.restart().asSeconds() << '\n';
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

	//Check if mouse wheel scrolled
	if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
	{
		signals::onMouseButtonPressed(*mouseButtonPressed);
	}
}

//Render all entities
void ECSGame::Render(sf::RenderWindow& renderWindow)
{
	//for debbuging purposes
	//DEB: sf::Clock timer;

	//Check if we need to close a game
	if (closeGame)
		renderWindow.close();

	//DEB: std::cout << "  --Check Game Closure: " << timer.restart().asSeconds() << '\n';

	std::shared_ptr<SceneNode> spBackgroundNode;
	if (overviewType == OverviewType::System) 
	{
		spBackgroundNode = sceneRoot->FindChild("Background").lock();

		//Set renderWindow to render in the camera
		std::shared_ptr<CameraComponent> sBackCameraCom = GetCameraFromBackgroundCameraEntity();
		renderWindow.setView(sBackCameraCom->view);

		//Render only background
		SceneNodeVisitorRender visitor(renderWindow);
		spBackgroundNode->AcceptVisitor(visitor);

		spBackgroundNode->GetEntity().lock()->hidden = true;
	}

	//Set renderWindow to render in the camera
	std::shared_ptr<CameraComponent> sCameraCom = GetCurrentlyActiveCamera();
	renderWindow.setView(sCameraCom->view);

	//Render all scene entities
	SceneNodeVisitorRender visitor(renderWindow);
	sceneRoot->AcceptVisitor(visitor);

	//DEB: visitor.OutputRenderStatistics();
	//DEB: std::cout << "  --Scene Rendering: " << timer.restart().asSeconds() << '\n';

	//Set renderWindow to render UI
	std::shared_ptr<CameraComponent> sUICameraCom = GetCameraFromUICameraEntity();
	renderWindow.setView(sUICameraCom->view);

	//Render all UI entities
	SceneNodeVisitorRenderUI visitor2(renderWindow);
	uiRoot->AcceptVisitor(visitor2);

	//DEB: visitor2.OutputRenderStatistics();
	//DEB: std::cout << "  --UI Rendering: " << timer.restart().asSeconds() << '\n';

	if (overviewType == OverviewType::System)
		spBackgroundNode->GetEntity().lock()->hidden = false;
}


void ECSGame::SetSimulationSpeed(float simSpeed) 
{
	if (simSpeed > 0.f)
		simulationSpeed = simSpeed;
}


float ECSGame::GetSimulationDeltaTime() const 
{
	if (gameState == GameState::Game)
		return deltaTime * simulationSpeed;
	else
		return 0.f;
}