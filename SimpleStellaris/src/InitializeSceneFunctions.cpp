#include "EntitiesFunctions.h"
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SceneNode.h"
#include <memory>
#include "Components.h"
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "GameState.h"
//#include "ParticlesConfigurations.h"
#include "SpaceObjectTypes.h"
#include "WorldGenerator.h"
#include "SceneNodeVisitors.h"
#include "CompilerInstructions.h"

//Initialize SpaceWorldScene
void InitializeSpaceWorldScene(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode) 
{
	//Create nodes, in which i will sort new entities which will be created during the game
	std::weak_ptr<Entity> wpCameras = ECSGame::Instance().GetEntityManager().NewEntity("Cameras");
	sceneNode->AddChild(std::make_shared<SceneNode>(wpCameras));

	std::weak_ptr<Entity> wpObjOrb = ECSGame::Instance().GetEntityManager().NewEntity("ObjectOrbits");
	uiNode->AddChild(std::make_shared<SceneNode>(wpObjOrb));

	std::weak_ptr<Entity> wpSysIc = ECSGame::Instance().GetEntityManager().NewEntity("SystemIcons");
	uiNode->AddChild(std::make_shared<SceneNode>(wpSysIc));

	//Initialize all cameras
	InitializeAllCameras(ECSGame::Instance().GetWindowSize());
	//Setup mouseIcon
	InitializeMouseIcon();
	//Initialize game ui
	CreateUI();
	//Create debug text
	CreateDebugText();

	uiNode->ChangeChildOrder(uiNode->FindChild("MouseIcon").lock(), (int)uiNode->GetAllChildren().size() - 1);

	//Generate world in a separate thread
	std::thread generateWorldAsync(CreateSpaceObjects);
	generateWorldAsync.detach();
}