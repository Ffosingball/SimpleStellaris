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
//Worst case: O(1)
std::weak_ptr<Entity> CreateNewEntityAtRoot(const std::string name) 
{
	// Create the entity in the entity manager
	std::weak_ptr<Entity> wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(name);
	// Add it to the scene
	ECSGame::Instance().GetSceneRoot()->AddChild(std::make_shared<SceneNode>(wpEntity));
	return wpEntity;
}



//This function creates an entity at node with an entity with provided name
//and return smart pointer to it
//String is not reference, because rvalues cannot be referenced, I use them a lot
//Worst case: O(2*N) where N is number of entities in game
std::weak_ptr<Entity> CreateNewEntityAt(const std::string nodeName, const std::string newEntityName)
{
	std::weak_ptr<Entity> wpEntity;
	//Get node with entity of that name
	std::weak_ptr<Entity> wSpEntity = ECSGame::Instance().GetEntityManager().FindEntity(nodeName);
	std::shared_ptr<Entity> sSpEntity = wSpEntity.lock();
	std::weak_ptr<SceneNode> nodePtr = ECSGame::Instance().GetSceneRoot()->FindChild(*sSpEntity);
	//Check if node with that entity exists
	if (nodePtr.lock() != nullptr)
	{
		// Create the entity in the entity manager
		wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(newEntityName);
		// Add it to the scene
		nodePtr.lock()->AddChild(std::make_shared<SceneNode>(wpEntity));
	}

	return wpEntity;
}



//Overloading of the function but instead of name you provide the node itself
//Worst case: O(1)
std::weak_ptr<Entity> CreateNewEntityAt(std::shared_ptr<SceneNode> parentNodePtr, const std::string newEntityName)
{
	std::weak_ptr<Entity> wpEntity;

	//Check if node with that entity exists
	if (parentNodePtr != nullptr)
	{
		// Create the entity in the entity manager
		wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(newEntityName);
		// Add it to the scene
		parentNodePtr->AddChild(std::make_shared<SceneNode>(wpEntity));
	}

	return wpEntity;
}



//IntRect means which part of the texture to draw
//Worst case: O(1)
void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath, OverviewType overviewType, sf::IntRect intRect)
{
	recShape->shape = sf::RectangleShape(size);
	recShape->shape.setOrigin(size / 2.f);
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().LoadTexture(texturePath);
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	recShape->shape.setTexture(sTexture.get());
	recShape->shape.setTextureRect(intRect);
}



//Worst case: O(1)
void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath, OverviewType overviewType)
{
	recShape->shape = sf::RectangleShape(size);
	recShape->shape.setOrigin(size / 2.f);
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().LoadTexture(texturePath);
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	recShape->shape.setTexture(sTexture.get());
}



//Returns camera component from the camera entity
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCurrentlyActiveCamera() 
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneRoot()->FindChild("Cameras");
	
	std::shared_ptr<Entity> sCamera;
	if (ECSGame::Instance().GetOverviewType() == OverviewType::Space)
		sCamera = wCamNode.lock()->FindChild("SpaceCamera").lock()->GetEntity().lock();
	else
		sCamera = wCamNode.lock()->FindChild("SystemCamera").lock()->GetEntity().lock();

	return GetCameraComponent(*sCamera);
}



//Returns camera component from the camera entity
//Worst case: O(N+M) where N is number of entities in game and M number of components in spaceMap
std::shared_ptr<SystemPropertiesComponent> GetSystemPropertiesFromSpaceMap()
{
	std::weak_ptr<Entity> wSmap = ECSGame::Instance().GetEntityManager().FindEntity("SpaceMap");
	std::shared_ptr<Entity> sSmap = wSmap.lock();
	return GetSystemPropertiesComponent(*sSmap);
}



//Returns camera component from the UIcamera entity
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromUICameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneRoot()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("UICamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return GetCameraComponent(*sCamera);
}



//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromBackgroundCameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneRoot()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("BackgroundCamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return GetCameraComponent(*sCamera);
}



//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromSystemCameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneRoot()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("SystemCamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return GetCameraComponent(*sCamera);
}



//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromSpaceCameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneRoot()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("SpaceCamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return GetCameraComponent(*sCamera);
}



//Worst case: O(1)
sf::Vector2f ConvertWindowPositionToWorld(sf::View cameraView, sf::Vector2i position) 
{
	sf::Vector2u windowSize = ECSGame::Instance().GetWindowSize();
	windowSize.x--;
	windowSize.y--;

	float relativeXPos = ((float)position.x) / ((float)windowSize.x);
	float relativeYPos = ((float)position.y) / ((float)windowSize.y);

	sf::Vector2f horizontalCameraBorders{cameraView.getCenter().x - (cameraView.getSize().x/2.f), cameraView.getCenter().x+ (cameraView.getSize().x / 2.f) };
	sf::Vector2f verticalCameraBorders{ cameraView.getCenter().y - (cameraView.getSize().y / 2.f), cameraView.getCenter().y + (cameraView.getSize().y / 2.f) };

	return {gel::linearInterpolation(horizontalCameraBorders.x, horizontalCameraBorders.y, relativeXPos), gel::linearInterpolation(verticalCameraBorders.x, verticalCameraBorders.y, relativeYPos) };
}



//Worst case: O(1)
sf::Vector2i ConvertWorldPositionToWindow(sf::View cameraView, sf::Vector2f position) 
{
	sf::Vector2u windowSize = ECSGame::Instance().GetWindowSize();
	windowSize.x--;
	windowSize.y--;

	float leftXBorder = cameraView.getCenter().x - (cameraView.getSize().x / 2.f);
	float topYBorder = cameraView.getCenter().y - (cameraView.getSize().y / 2.f);

	float relativeXPos = (position.x - leftXBorder) / cameraView.getSize().x;
	float relativeYPos = (position.y - topYBorder) / cameraView.getSize().y;

	return { (int)(windowSize.x*relativeXPos), (int)(windowSize.y * relativeYPos) };
}



//Worst case: O(1)
int GetKeyForSystemsPosition(sf::Vector2i gridPosition) 
{
	if (gridPosition.x < 0 || gridPosition.y < 0)
		return -1;

	return (gridPosition.x * 10000) + gridPosition.y;
}



//Worst case: O(N+M) where N is number of entities in game and M number of components in spaceMap
std::vector<std::shared_ptr<SceneNode>> GetAllSystemsNearPosition(sf::Vector2f position) 
{
	std::shared_ptr<SystemPropertiesComponent> spSysPropCom = GetSystemPropertiesFromSpaceMap();
	int yPos = (int)((position.y - spSysPropCom->mapConfig.verticalPosBoundaries.x) / spSysPropCom->mapConfig.minDistanceBetweenSystems);
	int xPos = (int)((position.x - spSysPropCom->mapConfig.horizontalPosBoundaries.x) / spSysPropCom->mapConfig.minDistanceBetweenSystems);

	std::vector<std::shared_ptr<SceneNode>> systemsNearby;
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos + 1, yPos + 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos+1, yPos+1 })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos + 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos+1 })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos - 1, yPos + 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos-1, yPos+1 })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos + 1, yPos })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos+1, yPos })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos - 1, yPos })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos-1, yPos })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos + 1, yPos - 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos+1, yPos-1 })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos - 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos-1 })]);
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos - 1, yPos - 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos-1, yPos-1 })]);

	return systemsNearby;
}



//Creates UI camera
//Worst case: O(N) where N is number of components available in game and M number of components
//in
void InitializeUICamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "UICamera").lock();
	//Add component
	spCamera->AddComponent(ComponentType::Camera);
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraComponent(*spCamera);
	//set camera properties
	spCameraCom->view.setSize(static_cast<sf::Vector2f>(windowSize));
	spCameraCom->view.setCenter(static_cast<sf::Vector2f>(windowSize) / 2.f);
}


//Creates a camera
void InitializeSpaceCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	int cameraHeight = 300;
	float cameraVelocity = 20.f;//14.f
	float cameraZoomSpeed = -8.f;
	float velocityChange = 8.f;
	sf::Vector2f zoomBorders = {0.4f, 1.7f};

	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "SpaceCamera").lock();
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
	spCameraCom->speedChange = velocityChange;
	//Get movement com
	std::shared_ptr<MovementComponent> spMovementCom = GetMovementComponent(*spCamera);
	spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity};
}

//Creates a system camera
void InitializeSystemCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	int cameraHeight = 300;
	float cameraVelocity = 20.f;//14.f
	float cameraZoomSpeed = -8.f;
	float velocityChange = 8.f;
	sf::Vector2f zoomBorders = { 0.001f, 10.f };

	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "SystemCamera").lock();
	//Add component
	spCamera->AddComponent(ComponentType::Camera);
	spCamera->AddComponent(ComponentType::Movement);
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraComponent(*spCamera);
	//set camera properties
	float windowSizeRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	//Set camera sizes
	spCameraCom->view.setSize(sf::Vector2f{ (float)cameraHeight * windowSizeRatio,(float)cameraHeight });
	spCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
	//Set zoom properties
	spCameraCom->cameraSize = spCameraCom->view.getSize();
	spCameraCom->zoomingBorders = zoomBorders;
	spCameraCom->zoomingSpeed = cameraZoomSpeed;
	spCameraCom->speedChange = velocityChange;
	//Get movement com
	std::shared_ptr<MovementComponent> spMovementCom = GetMovementComponent(*spCamera);
	spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity };
}


//Creates background camera
void InitializeBackgroundCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "BackgroundCamera").lock();
	//Add component
	spCamera->AddComponent(ComponentType::Camera);
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraComponent(*spCamera);
	//set camera properties
	spCameraCom->view.setSize(static_cast<sf::Vector2f>(windowSize));
	spCameraCom->view.setCenter(static_cast<sf::Vector2f>(windowSize) / 2.f);
}


void InitializeAllCameras(const sf::Vector2u& windowSize)
{
	std::shared_ptr<SceneNode> spAllCam = ECSGame::Instance().GetSceneRoot()->FindChild("Cameras").lock();
	
	InitializeSpaceCamera(spAllCam, windowSize);
	InitializeUICamera(spAllCam, windowSize);
	InitializeBackgroundCamera(spAllCam, windowSize);
	InitializeSystemCamera(spAllCam, windowSize);
}


void outputChildrens(std::vector<std::shared_ptr<SceneNode>> v) 
{
	for (std::shared_ptr<SceneNode> n : v) 
	{
		std::cout << n->GetEntity().lock()->GetName() << '\n';
	}
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
	spTileMapCom->drawAt = OverviewType::Always;
	//Iitialize all tiles
	//std::cout << "Before init tileMap\n";
	spTileMapCom->tileMap.Initialize(WorldGenerator::GenerateGridOfTiles(mapConfig.backgroundSize, sf::Vector2i{ 0, (tilesInTileset.x * tilesInTileset.y) - 1 }), WorldGenerator::GenerateGridOfRandomNumbers(mapConfig.backgroundSize, sf::Vector2i{ 0, 3 }));
	spTileMap->SetPosition(sf::Vector2f{(float)(tilesSize.x*mapConfig.backgroundSize.x/(-2.f)),(float)(tilesSize.y * mapConfig.backgroundSize.y / (-2.f)) });

	//std::cout << "Before: " << '\n';
	//outputChildrens(ECSGame::Instance().GetSceneRoot()->GetAllChildren());
	ECSGame::Instance().GetSceneRoot()->ChangeChildOrder(spTileMap,0);
	//std::cout << "After: " << '\n';
	//outputChildrens(ECSGame::Instance().GetSceneRoot()->GetAllChildren());

	return spTileMapCom;
}


//Creates space objects
void CreateSpaceObjects() 
{
	float additionalSpaceForCameraBoundaries = 20.f;

	//Get spaceMap node
	std::weak_ptr<SceneNode> wpNode = ECSGame::Instance().GetSceneRoot()->FindChild("SpaceMap");
	std::shared_ptr<SceneNode> spNode = wpNode.lock();
	SpaceMapConfigurations mapConfig = GetSystemPropertiesComponent(*spNode->GetEntity().lock())->mapConfig;
	//Firstly generate background
	std::shared_ptr<TileMapComponent> spTileMapCom = GenerateBackgroundTiles(mapConfig);
	//Secondly generate systems and stars in it
	WorldGenerator::GenerateSpaceMap(spNode,mapConfig);
	//Thirdly put rectangleShape components for all objects
	TextureSetter txSetter(WorldGenerator::getSeed());
	txSetter.mapConfig = mapConfig;
	txSetter.ptrSpaceMapNode = spNode;
	spNode->AcceptVisitor(txSetter);
	//Lastly set camera boundaries
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraFromSpaceCameraEntity();
	sf::Vector2f mapSize = static_cast<sf::Vector2f>(spTileMapCom->tileMap.getMapSize());
	spCameraCom->horizontalBorders = { mapConfig.horizontalPosBoundaries.x - additionalSpaceForCameraBoundaries, mapConfig.horizontalPosBoundaries.y + additionalSpaceForCameraBoundaries };
	spCameraCom->verticalBorders = { mapConfig.verticalPosBoundaries.x - additionalSpaceForCameraBoundaries, mapConfig.verticalPosBoundaries.y + additionalSpaceForCameraBoundaries };
	spCameraCom->view.setCenter(sf::Vector2f{ 0.f, 0.f });

	std::shared_ptr<CameraComponent> spCameraCom2 = GetCameraFromSystemCameraEntity();
	spCameraCom2->horizontalBorders = { -mapConfig.afarStarsBoundaries.y, mapConfig.afarStarsBoundaries.y };
	spCameraCom2->verticalBorders = { -mapConfig.afarStarsBoundaries.y, mapConfig.afarStarsBoundaries.y };
	spCameraCom2->view.setCenter(sf::Vector2f{ 0.f, 0.f });

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