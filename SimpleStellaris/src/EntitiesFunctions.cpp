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

//Put here functions for creation and managing entities!


//This function creates an entity at the root node and return smart pointer to it
//String is not reference, because rvalues cannot be referenced, I use them a lot
std::weak_ptr<Entity> CreateNewEntityAtRoot(const std::string name) 
{
	// Create the entity in the entity manager
	std::weak_ptr<Entity> wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(name);
	// Add it to the scene
	ECSGame::Instance().GetSceneRoot().AddChild(SceneNode(wpEntity));
	return wpEntity;
}


//This function creates an entity at node with an entity with provided name
//and return smart pointer to it
//String is not reference, because rvalues cannot be referenced, I use them a lot
std::weak_ptr<Entity> CreateNewEntityAt(const std::string nodeName, const std::string newEntityName)
{
	std::weak_ptr<Entity> wpEntity;
	//Get node with entity of that name
	std::weak_ptr<Entity> wSpEntity = ECSGame::Instance().GetEntityManager().FindEntity(nodeName);
	std::shared_ptr<Entity> sSpEntity = wSpEntity.lock();
	SceneNode* nodePtr = ECSGame::Instance().GetSceneRoot().FindChild(*sSpEntity);
	//Check if node with that entity exists
	if (nodePtr != nullptr)
	{
		// Create the entity in the entity manager
		wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(newEntityName);
		// Add it to the scene
		nodePtr->AddChild(SceneNode(wpEntity));
	}

	return wpEntity;
}

//Overloading of the function but instead of name you provide the node itself
std::weak_ptr<Entity> CreateNewEntityAt(std::weak_ptr<SceneNode> parentNodeWP, const std::string newEntityName)
{
	std::weak_ptr<Entity> wpEntity;

	std::shared_ptr<SceneNode> nodePtr = parentNodeWP.lock();
	//Check if node with that entity exists
	if (nodePtr != nullptr)
	{
		std::weak_ptr<Entity> wpEntity;
		// Create the entity in the entity manager
		wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(newEntityName);
		// Add it to the scene
		nodePtr->AddChild(SceneNode(wpEntity));
	}

	return wpEntity;
}

//Returns camera component from the camera entity
std::shared_ptr<CameraComponent> GetCameraFromCameraEntity() 
{
	std::weak_ptr<Entity> wCamera = ECSGame::Instance().GetEntityManager().FindEntity("Camera");
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return GetCameraComponent(*sCamera);
}


//Returns camera component from the UIcamera entity
std::shared_ptr<CameraComponent> GetCameraFromUICameraEntity()
{
	std::weak_ptr<Entity> wCamera = ECSGame::Instance().GetEntityManager().FindEntity("UICamera");
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return GetCameraComponent(*sCamera);
}


//This function set new position of this entity
void SetNewPosition(std::weak_ptr<Entity> entity, const sf::Vector2f position)
{
	std::shared_ptr<Entity> sEntity = entity.lock();
	sf::Transformable sTransform = sEntity->GetTransformable();
	//Set position
	sTransform.setPosition(position);
	sEntity->SetTransformable(sTransform);
}


/*void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath)
{
	recShape->shape = sf::RectangleShape(size);
	recShape->shape.setOrigin(size / 2.f);
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().LoadTexture(texturePath);
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	recShape->shape.setTexture(sTexture.get());
}*/


//Creates a tilemap and camera
void InitializeTileMapAndCamera(const sf::Vector2u& windowSize)
{
	//Create tileMap
	std::shared_ptr<Entity> spTileMap = CreateNewEntityAtRoot("TileMap").lock();
	// Add components
	spTileMap->AddComponent(ComponentType::TileMap);
	//Get component
	std::shared_ptr<TileMapComponent> spTileMapCom = GetTileMapComponent(*spTileMap);
	//set tilemap properties
	spTileMapCom->tileMap.tileSize = sf::Vector2i{64,64};
	spTileMapCom->tileMap.marginSize = sf::Vector2i{ 0,0 };
	spTileMapCom->tileMap.paddingSize = sf::Vector2i{ 0,0 };
	spTileMapCom->tileMap.numTilesInTileset = sf::Vector2i{ 4,5 };
	spTileMapCom->tileMap.tilesTexturePath = "media/textures/SpaceBackground.png";
	spTileMapCom->tileMap.mapSize = sf::Vector2i{10,10};
	spTileMapCom->tileMap.seed = (unsigned int)gel::Randf(1000000.f, 9999999.f);
	spTileMapCom->tileMap.randomlySelectTiles = true;
	//Iitialize all tiles
	spTileMapCom->tileMap.Initialize();

	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAtRoot("Camera").lock();
	//Add component
	spCamera->AddComponent(ComponentType::Camera);
	spCamera->AddComponent(ComponentType::Movement);
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraComponent(*spCamera);
	//set camera properties
	sf::Vector2f mapSize = static_cast<sf::Vector2f>(spTileMapCom->tileMap.getMapSize());
	float windowSizeRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	//Set camera sizes
	spCameraCom->view.setSize(sf::Vector2f{mapSize.y*windowSizeRatio,mapSize.y});
	spCameraCom->view.setCenter(sf::Vector2f{ mapSize.y * (windowSizeRatio/2.f)*1.f,mapSize.y/2.f });
	//Set boundaries
	spCameraCom->rightBorder = mapSize.x - ((mapSize.y * windowSizeRatio)/2.f);
	spCameraCom->leftBorder = spCameraCom->view.getCenter().x;
}


//Creates UI camera
void InitializeUICamera(const sf::Vector2u& windowSize)
{
	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAtRoot("UICamera").lock();
	//Add component
	spCamera->AddComponent(ComponentType::Camera);
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraComponent(*spCamera);
	//set camera properties
	spCameraCom->view.setSize(static_cast<sf::Vector2f>(windowSize));
	spCameraCom->view.setCenter(static_cast<sf::Vector2f>(windowSize)/2.f);
}