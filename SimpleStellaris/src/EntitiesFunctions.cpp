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
std::weak_ptr<Entity> CreateNewEntityAt(SceneNode* parentNodePtr, const std::string newEntityName)
{
	std::weak_ptr<Entity> wpEntity;

	//Check if node with that entity exists
	if (parentNodePtr != nullptr)
	{
		// Create the entity in the entity manager
		wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(newEntityName);
		// Add it to the scene
		parentNodePtr->AddChild(SceneNode(wpEntity));
	}

	return wpEntity;
}


void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath)
{
	recShape->shape = sf::RectangleShape(size);
	recShape->shape.setOrigin(size / 2.f);
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().LoadTexture(texturePath);
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	recShape->shape.setTexture(sTexture.get());
}

//IntRect means which part of the texture to draw
void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath, sf::IntRect intRect)
{
	recShape->shape = sf::RectangleShape(size);
	recShape->shape.setOrigin(size / 2.f);
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().LoadTexture(texturePath);
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	recShape->shape.setTexture(sTexture.get());
	recShape->shape.setTextureRect(intRect);
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
void InitializeCamera(const sf::Vector2u& windowSize)
{
	int cameraHeight = 300;
	float cameraVelocity = 40.f;//14.f
	float cameraZoomSpeed = -5.f;
	sf::Vector2f zoomBorders = {0.4f, 1.7f};

	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAtRoot("Camera").lock();
	//Add component
	spCamera->AddComponent(ComponentType::Camera);
	spCamera->AddComponent(ComponentType::Movement);
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraComponent(*spCamera);
	//set camera properties
	float windowSizeRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	//Set camera sizes
	spCameraCom->view.setSize(sf::Vector2f{(float)cameraHeight*windowSizeRatio,(float)cameraHeight});
	spCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
	//Set zoom properties
	spCameraCom->cameraSize = spCameraCom->view.getSize();
	spCameraCom->zoomingBorders = zoomBorders;
	spCameraCom->zoomingSpeed = cameraZoomSpeed;
	//Get movement com
	std::shared_ptr<MovementComponent> spMovementCom = GetMovementComponent(*spCamera);
	spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity};
}


std::shared_ptr<TileMapComponent> GenerateBackgroundTiles(SpaceMapConfigurations& mapConfig)
{
	sf::Vector2i tilesInTileset{ 4,5 };
	sf::Vector2i tilesSize{ 64,64 };

	//Create tileMap
	std::shared_ptr<Entity> spTileMap = CreateNewEntityAtRoot("Background").lock();
	// Add components
	spTileMap->AddComponent(ComponentType::TileMap);
	//Get component
	std::shared_ptr<TileMapComponent> spTileMapCom = GetTileMapComponent(*spTileMap);
	//set tilemap properties
	spTileMapCom->tileMap.tileSize = tilesSize;
	spTileMapCom->tileMap.marginSize = sf::Vector2i{ 0,0 };
	spTileMapCom->tileMap.paddingSize = sf::Vector2i{ 0,0 };
	spTileMapCom->tileMap.numTilesInTileset = tilesInTileset;
	spTileMapCom->tileMap.tilesTexturePath = "media/textures/SpaceBackground.png";
	spTileMapCom->tileMap.mapSize = mapConfig.backgroundSize;
	spTileMapCom->tileMap.loadTilesFromFile = false;
	spTileMapCom->tileMap.rotateTiles = true;
	//Iitialize all tiles
	//std::cout << "Before init tileMap\n";
	spTileMapCom->tileMap.Initialize(WorldGenerator::GenerateGridOfTiles(mapConfig.backgroundSize, sf::Vector2i{ 0, (tilesInTileset.x * tilesInTileset.y) - 1 }), WorldGenerator::GenerateGridOfRandomNumbers(mapConfig.backgroundSize, sf::Vector2i{ 0, 3 }));
	spTileMap->SetPosition(sf::Vector2f{(float)(tilesSize.x*mapConfig.backgroundSize.x/(-2.f)),(float)(tilesSize.y * mapConfig.backgroundSize.y / (-2.f)) });

	return spTileMapCom;
}


//Creates space objects
void CreateSpaceObjects() 
{
	SpaceMapConfigurations mapConfig;
	//Get spaceMap node
	SceneNode* spNode = ECSGame::Instance().GetSceneRoot().FindChild(*ECSGame::Instance().GetEntityManager().FindEntity("Background").lock());
	//Firstly generate background
	std::shared_ptr<TileMapComponent> spTileMapCom = GenerateBackgroundTiles(mapConfig);
	//Secondly generate systems and stars in it
	WorldGenerator::GenerateSpaceMap(spNode,mapConfig);
	//Thirdly put rectangleShape components for all objects
	TextureSetter txSetter;
	txSetter.mapConfig = mapConfig;
	txSetter.ptrSpaceMapNode = spNode;
	spNode->AcceptVisitor(txSetter);
	//Lastly set camera boundaries
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraFromCameraEntity();
	sf::Vector2f mapSize = static_cast<sf::Vector2f>(spTileMapCom->tileMap.getMapSize());
	spCameraCom->horizontalBorders = mapConfig.horizontalPosBoundaries;
	spCameraCom->verticalBorders = mapConfig.verticalPosBoundaries;
	spCameraCom->view.setCenter(sf::Vector2f{ 0.f, 0.f });

	//Check systems generated
	/*SceneNodeSpaceObjectsCounter visitor;
	spNode->AcceptVisitor(visitor);
	std::cout << '\n';
	int total{ 0 };
	std::cout << "Overall stars:\n";
	total += visitor.redSupGiantAmount;
	std::cout << "Total Red Supergiants: " << visitor.redSupGiantAmount << '\n';
	total += visitor.redGiantAmount;
	std::cout << "Total Red Giants: " << visitor.redGiantAmount << '\n';
	total += visitor.OclassAmount;
	std::cout << "Total Blue Supergiants (O): " << visitor.OclassAmount << '\n';
	total += visitor.BclassAmount;
	std::cout << "Total Blue Giants (B): " << visitor.BclassAmount << '\n';
	total += visitor.AclassAmount;
	std::cout << "Total Blueish Stars (A): " << visitor.AclassAmount << '\n';
	total += visitor.FclassAmount;
	std::cout << "Total White Stars (F): " << visitor.FclassAmount << '\n';
	total += visitor.GclassAmount;
	std::cout << "Total Yellow Stars (G): " << visitor.GclassAmount << '\n';
	total += visitor.KclassAmount;
	std::cout << "Total Orange Dwarfs (K): " << visitor.KclassAmount << '\n';
	total += visitor.MclassAmount;
	std::cout << "Total Red Dwarfs (M): " << visitor.MclassAmount << '\n';
	total += visitor.brownDwarfAmount;
	std::cout << "Total Brown Dwarfs: " << visitor.brownDwarfAmount << '\n';
	total += visitor.whiteDwarfAmount;
	std::cout << "Total White Dwarfs: " << visitor.whiteDwarfAmount << '\n';
	total += visitor.neutronAmount;
	std::cout << "Total Neutron Stars: " << visitor.neutronAmount << '\n';
	total += visitor.blackHoleAmount;
	std::cout << "Total Black Holes: " << visitor.blackHoleAmount << '\n';
	std::cout << '\n';
	std::cout << "Total stars: "<< total<<'\n';

	std::cout << '\n';
	total = 0;
	std::cout << "Overall systems:\n";
	total += visitor.singleSysAmount;
	std::cout << "Total Single Systems: " << visitor.singleSysAmount << '\n';
	total += visitor.binaryCloseSysAmount;
	std::cout << "Total Binary Close Systems: " << visitor.binaryCloseSysAmount << '\n';
	total += visitor.binaryAfarSysAmount;
	std::cout << "Total Binary Afar Systems: " << visitor.binaryAfarSysAmount << '\n';
	total += visitor.ternaryCloseSysAmount;
	std::cout << "Total Ternary Close Systems: " << visitor.ternaryCloseSysAmount << '\n';
	total += visitor.ternaryAfarSysAmount;
	std::cout << "Total Ternary Afar Systems: " << visitor.ternaryAfarSysAmount << '\n';
	total += visitor.ternaryTwoCloseOneAfarSysAmount;
	std::cout << "Total Ternary Two Close One Afar Systems: " << visitor.ternaryTwoCloseOneAfarSysAmount << '\n';
	std::cout << '\n';
	std::cout << "Total systems: " << total << '\n';*/
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