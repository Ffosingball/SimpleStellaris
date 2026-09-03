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

//Put here functions for creation and managing entities!


//This function creates an entity at the root node and return smart pointer to it
//String is not reference, because rvalues cannot be referenced, I use them a lot
//Worst case: O(1)
std::weak_ptr<Entity> CreateNewEntityAtSceneNode(const std::string name) 
{
	// Create the entity in the entity manager
	std::weak_ptr<Entity> wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(name);
	// Add it to the scene
	ECSGame::Instance().GetSceneNode()->AddChild(std::make_shared<SceneNode>(wpEntity));
	return wpEntity;
}



//Worst case: O(1)
std::weak_ptr<Entity> CreateNewEntityAtUINode(const std::string name)
{
	// Create the entity in the entity manager
	std::weak_ptr<Entity> wpEntity = ECSGame::Instance().GetEntityManager().NewEntity(name);
	// Add it to the scene
	ECSGame::Instance().GetUINode()->AddChild(std::make_shared<SceneNode>(wpEntity));
	return wpEntity;
}



//This function creates an entity at node with an entity with provided name
//and return smart pointer to it
//String is not reference, because rvalues cannot be referenced, I use them a lot
//Worst case: O(2*N) where N is number of entities in game
std::weak_ptr<Entity> CreateNewEntityAt(const std::string nodeName, const std::string newEntityName, const std::shared_ptr<SceneNode> rootNode)
{
	std::weak_ptr<Entity> wpEntity;
	//Get node with entity of that name
	std::weak_ptr<SceneNode> nodePtr = rootNode->FindChild(nodeName);
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
std::weak_ptr<Entity> CreateNewEntityAt(const std::shared_ptr<SceneNode> parentNodePtr, const std::string newEntityName)
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
void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string textureName)
{
	recShape->shape = sf::RectangleShape(size);
	recShape->shape.setOrigin(size / 2.f);
	sf::IntRect intRect;
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().GetTexture(textureName, intRect);
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	recShape->shape.setTexture(sTexture.get());
	recShape->shape.setTextureRect(intRect);
}



//Returns camera component from the camera entity
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCurrentlyActiveCamera() 
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneNode()->FindChild("Cameras");
	
	std::shared_ptr<Entity> sCamera;
	if (ECSGame::Instance().GetOverviewType() == OverviewType::Space)
		sCamera = wCamNode.lock()->FindChild("SpaceCamera").lock()->GetEntity().lock();
	else if (ECSGame::Instance().GetOverviewType() == OverviewType::System)
		sCamera = wCamNode.lock()->FindChild("SystemCamera").lock()->GetEntity().lock();
	else if (ECSGame::Instance().GetOverviewType() == OverviewType::Planet)
		sCamera = wCamNode.lock()->FindChild("PlanetCamera").lock()->GetEntity().lock();

	return sCamera->FindComponent<CameraComponent>().lock();
}



//Returns camera component from the camera entity
//Worst case: O(N+M) where N is number of entities in game and M number of components in spaceMap
std::shared_ptr<SystemPropertiesComponent> GetSystemPropertiesFromSpaceMap()
{
	std::weak_ptr<SceneNode> wSnode = ECSGame::Instance().GetSceneNode()->FindChild("SpaceMap");
	std::shared_ptr<Entity> sSmap = wSnode.lock()->GetEntity().lock();
	return sSmap->FindComponent<SystemPropertiesComponent>().lock();
}



//Returns camera component from the UIcamera entity
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromUICameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneNode()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("UICamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return sCamera->FindComponent<CameraComponent>().lock();
}



//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromBackgroundCameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneNode()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("BackgroundCamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return sCamera->FindComponent<CameraComponent>().lock();
}



//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromSystemCameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneNode()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("SystemCamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return sCamera->FindComponent<CameraComponent>().lock();
}



std::shared_ptr<CameraComponent> GetCameraFromPlanetCameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneNode()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("PlanetCamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return sCamera->FindComponent<CameraComponent>().lock();
}



//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromSpaceCameraEntity()
{
	std::weak_ptr<SceneNode> wCamNode = ECSGame::Instance().GetSceneNode()->FindChild("Cameras");
	std::weak_ptr<Entity> wCamera = wCamNode.lock()->FindChild("SpaceCamera").lock()->GetEntity().lock();
	std::shared_ptr<Entity> sCamera = wCamera.lock();
	return sCamera->FindComponent<CameraComponent>().lock();
}



//Worst case: O(1)
sf::Vector2f ConvertWindowPositionToWorld(sf::View& cameraView, sf::Vector2i position) 
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
sf::Vector2i ConvertWorldPositionToWindow(sf::View& cameraView, sf::Vector2f position) 
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



//Worst case: O(N) where N is number of months in a year
void GetDateFromDays(int daysPast, int& day, std::string& month, int& year) 
{
	year = daysPast / 365;
	daysPast %= 365;
	daysPast++;

	daysPast -= 31;
	if (daysPast <= 0) 
	{
		daysPast += 31;
		month = "January";
		day = daysPast;
		return;
	}

	daysPast -= 28;
	if (daysPast <= 0)
	{
		daysPast += 28;
		month = "February";
		day = daysPast;
		return;
	}

	daysPast -= 31;
	if (daysPast <= 0)
	{
		daysPast += 31;
		month = "March";
		day = daysPast;
		return;
	}

	daysPast -= 30;
	if (daysPast <= 0)
	{
		daysPast += 30;
		month = "April";
		day = daysPast;
		return;
	}

	daysPast -= 31;
	if (daysPast <= 0)
	{
		daysPast += 31;
		month = "May";
		day = daysPast;
		return;
	}

	daysPast -= 30;
	if (daysPast <= 0)
	{
		daysPast += 30;
		month = "June";
		day = daysPast;
		return;
	}

	daysPast -= 31;
	if (daysPast <= 0)
	{
		daysPast += 31;
		month = "July";
		day = daysPast;
		return;
	}

	daysPast -= 31;
	if (daysPast <= 0)
	{
		daysPast += 31;
		month = "August";
		day = daysPast;
		return;
	}

	daysPast -= 30;
	if (daysPast <= 0)
	{
		daysPast += 30;
		month = "September";
		day = daysPast;
		return;
	}

	daysPast -= 31;
	if (daysPast <= 0)
	{
		daysPast += 31;
		month = "October";
		day = daysPast;
		return;
	}

	daysPast -= 30;
	if (daysPast <= 0)
	{
		daysPast += 30;
		month = "November";
		day = daysPast;
		return;
	}

	daysPast -= 31;
	if (daysPast <= 0)
	{
		daysPast += 31;
		month = "December";
		day = daysPast;
		return;
	}
}



//Worst case: O(1)
bool IsWorldPosInsideOfCamera(std::shared_ptr<CameraComponent> spCamCom, sf::Vector2f worldPos)
{
	sf::Vector2f horizontalCameraBorders{ spCamCom->view.getCenter().x - (spCamCom->view.getSize().x / 2.f) - spCamCom->renderOutsideBoundsFor, spCamCom->view.getCenter().x + (spCamCom->view.getSize().x / 2.f) + spCamCom->renderOutsideBoundsFor };
	sf::Vector2f verticalCameraBorders{ spCamCom->view.getCenter().y - (spCamCom->view.getSize().y / 2.f) - spCamCom->renderOutsideBoundsFor, spCamCom->view.getCenter().y + (spCamCom->view.getSize().y / 2.f) + spCamCom->renderOutsideBoundsFor };

	return worldPos.x > horizontalCameraBorders.x && worldPos.x < horizontalCameraBorders.y && worldPos.y>verticalCameraBorders.x && worldPos.y < verticalCameraBorders.y;
}



//Worst case: O(N+M) where N is number of entities in game and M number of components in spaceMap
std::vector<std::shared_ptr<SceneNode>> GetAllSystemsNearPosition(sf::Vector2f position, std::weak_ptr<SystemPropertiesComponent> wpSysPropCom)
{
	std::shared_ptr<SystemPropertiesComponent> spSysPropCom = wpSysPropCom.lock();
	if (spSysPropCom == nullptr)
		spSysPropCom = GetSystemPropertiesFromSpaceMap();

	int yPos = (int)((position.y - WorldGenerator::mapConfig.verticalPosBoundaries.x) / WorldGenerator::mapConfig.minDistanceBetweenSystems);
	int xPos = (int)((position.x - WorldGenerator::mapConfig.horizontalPosBoundaries.x) / WorldGenerator::mapConfig.minDistanceBetweenSystems);

	std::vector<std::shared_ptr<SceneNode>> systemsNearby;
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos + 1, yPos + 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos+1, yPos+1 })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos + 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos+1 })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos - 1, yPos + 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos-1, yPos+1 })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos + 1, yPos })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos+1, yPos })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos - 1, yPos })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos-1, yPos })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos + 1, yPos - 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos+1, yPos-1 })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos - 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos-1 })].lock());
	
	if (spSysPropCom->systemsPositions.find(GetKeyForSystemsPosition(sf::Vector2i{ xPos - 1, yPos - 1 })) != spSysPropCom->systemsPositions.end())
		systemsNearby.push_back(spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos-1, yPos-1 })].lock());

	return systemsNearby;
}



//Creates UI camera
//Worst case: O(2N+M) where N is number of components available in game and M number of components
//available in game
void InitializeUICamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "UICamera").lock();
	//Add component
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
	//set camera properties
	spCameraCom->view.setSize(static_cast<sf::Vector2f>(windowSize));
	spCameraCom->view.setCenter(static_cast<sf::Vector2f>(windowSize) / 2.f);
}



//Creates a camera
//Worst case: O(4N+M) where N is number of components available in game and M number of components
//available in game
void InitializeSpaceCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	int cameraHeight = 300;
	float cameraVelocity = 20.f;//14.f
	float cameraZoomSpeed = 5.f;
	float velocityChange = 8.f;
	sf::Vector2f zoomBorders = {0.3f, 1.6f};
	float outsideBordersMaxRenderDistance = 15.f;

	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "SpaceCamera").lock();
	//Add component
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
	std::shared_ptr<MovementComponent> spMovementCom = spCamera->AddComponent<MovementComponent>().lock();
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
	spCameraCom->renderOutsideBoundsFor = outsideBordersMaxRenderDistance;
	spCameraCom->moveCamera = true;
	//Get movement com
	spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity};
}



//Creates a system camera
//Worst case: O(4N+M) where N is number of components available in game and M number of components
//available in game
void InitializeSystemCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	int cameraHeight = 300;
	float cameraVelocity = 20.f;//14.f
	float cameraZoomSpeed = 15.f;
	float velocityChange = 8.f;
	sf::Vector2f zoomBorders = { 0.0001f, 8.f };
	float outsideBordersMaxRenderDistance = 15.f;

	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "SystemCamera").lock();
	//Add component
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
	std::shared_ptr<MovementComponent> spMovementCom = spCamera->AddComponent<MovementComponent>().lock();
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
	spCameraCom->renderOutsideBoundsFor = outsideBordersMaxRenderDistance;
	spCameraCom->moveCamera = false;
	//Get movement com
	spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity };
}



//Creates planet overview camera
void InitializePlanetCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	int cameraHeight = 1500;
	float cameraVelocity = 120.f;//14.f
	float cameraZoomSpeed = 20.f;
	float velocityChange = 8.f;
	sf::Vector2f zoomBorders = { 0.008f, 7.f };
	float outsideBordersMaxRenderDistance = 15.f;

	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "PlanetCamera").lock();
	//Add component
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
	std::shared_ptr<MovementComponent> spMovementCom = spCamera->AddComponent<MovementComponent>().lock();
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
	spCameraCom->renderOutsideBoundsFor = outsideBordersMaxRenderDistance;
	spCameraCom->moveCamera = false;
	//Get movement com
	spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity };
}



//Creates background camera
//Worst case: O(2N+M) where N is number of components available in game and M number of components
//available in game
void InitializeBackgroundCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
{
	//Create camera
	std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "BackgroundCamera").lock();
	//Add component
	//Get component
	std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
	//set camera properties
	spCameraCom->view.setSize(static_cast<sf::Vector2f>(windowSize));
	spCameraCom->view.setCenter(static_cast<sf::Vector2f>(windowSize) / 2.f);
}

//Worst case: O(12N+4M) where N is number of components available in game and M number of components
//available in game
void InitializeAllCameras(const sf::Vector2u& windowSize)
{
	std::shared_ptr<SceneNode> spAllCam = ECSGame::Instance().GetSceneNode()->FindChild("Cameras").lock();
	
	InitializeSpaceCamera(spAllCam, windowSize);
	InitializeUICamera(spAllCam, windowSize);
	InitializeBackgroundCamera(spAllCam, windowSize);
	InitializeSystemCamera(spAllCam, windowSize);
	InitializePlanetCamera(spAllCam, windowSize);
}



//Worst case: O(N) where N is number of entities in provided vector
void outputChildrens(std::vector<std::shared_ptr<SceneNode>> v) 
{
	for (std::shared_ptr<SceneNode> n : v) 
	{
		std::cout << n->GetEntity().lock()->GetName() << '\n';
	}
}



//Worst case: O(4N) where N is number of tiles to generate
std::shared_ptr<TileMapComponent> GenerateBackgroundTiles(std::shared_ptr<Entity> spTileMap)
{
	sf::Vector2i tilesInTileset{ 4,5 };
	sf::Vector2i tilesSize{ 64,64 };

	//Get component
	std::shared_ptr<TileMapComponent> spTileMapCom = spTileMap->AddComponent<TileMapComponent>().lock();
	//set tilemap properties
	spTileMapCom->tileMap.tileSize = tilesSize;
	spTileMapCom->tileMap.marginSize = sf::Vector2i{ 0,0 };
	spTileMapCom->tileMap.paddingSize = sf::Vector2i{ 0,0 };
	spTileMapCom->tileMap.numTilesInTileset = tilesInTileset;
	spTileMapCom->tileMap.tilesTexturePath = "media/textures/SpaceBackground.png";
	spTileMapCom->tileMap.mapSize = WorldGenerator::mapConfig.backgroundSize;
	spTileMapCom->tileMap.loadTilesFromFile = false;
	spTileMapCom->tileMap.rotateTiles = true;
	//Iitialize all tiles
	spTileMapCom->tileMap.Initialize(WorldGenerator::GenerateGridOfTiles(WorldGenerator::mapConfig.backgroundSize, sf::Vector2i{ 0, (tilesInTileset.x * tilesInTileset.y) - 1 }), WorldGenerator::GenerateGridOfRandomNumbers(WorldGenerator::mapConfig.backgroundSize, sf::Vector2i{ 0, 3 }));
	spTileMap->SetPosition(sf::Vector2f{(float)(tilesSize.x* WorldGenerator::mapConfig.backgroundSize.x/(-2.f)),(float)(tilesSize.y * WorldGenerator::mapConfig.backgroundSize.y / (-2.f)) });

	return spTileMapCom;
}


//Creates space objects
//Worst case: O(2N+4M) where N is number of systems + stars to create and M number of tiles to create
void CreateSpaceObjects() 
{
	float additionalSpaceForCameraBoundaries = 30.f;
	std::weak_ptr<Entity> wpPlay = ECSGame::Instance().GetEntityManager().NewEntity("SpaceMap");
	std::shared_ptr<SceneNode> spNode = std::make_shared<SceneNode>(wpPlay);
	wpPlay.lock()->AddComponent<SystemPropertiesComponent>();

	std::weak_ptr<Entity> wpSysN = ECSGame::Instance().GetEntityManager().NewEntity("SystemNames");
	std::shared_ptr<SceneNode> spSysNamesNode = std::make_shared<SceneNode>(wpSysN);
	std::weak_ptr<Entity> wpNebN = ECSGame::Instance().GetEntityManager().NewEntity("NebulasNames");
	std::shared_ptr<SceneNode> spNebNamesNode = std::make_shared<SceneNode>(wpNebN);

	std::weak_ptr<Entity> wpBackgroundE = ECSGame::Instance().GetEntityManager().NewEntity("Background");
	std::shared_ptr<SceneNode> spBackgroundNode = std::make_shared<SceneNode>(wpBackgroundE);
	std::weak_ptr<Entity> wpNebul = ECSGame::Instance().GetEntityManager().NewEntity("Nebulas");
	spBackgroundNode->AddChild(std::make_shared<SceneNode>(wpNebul));

	//Firstly generate background
	std::shared_ptr<TileMapComponent> spTileMapCom = GenerateBackgroundTiles(wpBackgroundE.lock());
	//Secondly generate nebulas
	WorldGenerator::GenerateNebulas(spBackgroundNode->FindChild("Nebulas").lock(), spNebNamesNode);
	//Thirdly generate systems and stars in it
	WorldGenerator::GenerateSpaceMap(spNode);
	//After put rectangleShape components for all objects
	TextureSetter txSetter(WorldGenerator::getSeed(), spSysNamesNode);
	txSetter.wpSpaceMapNode = spNode;
	spNode->AcceptVisitor(txSetter);
	//Lastly set camera boundaries
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraFromSpaceCameraEntity();
	sf::Vector2f mapSize = static_cast<sf::Vector2f>(spTileMapCom->tileMap.getMapSize());
	spCameraCom->horizontalBorders = { WorldGenerator::mapConfig.horizontalPosBoundaries.x - additionalSpaceForCameraBoundaries, WorldGenerator::mapConfig.horizontalPosBoundaries.y + additionalSpaceForCameraBoundaries };
	spCameraCom->verticalBorders = { WorldGenerator::mapConfig.verticalPosBoundaries.x - additionalSpaceForCameraBoundaries, WorldGenerator::mapConfig.verticalPosBoundaries.y + additionalSpaceForCameraBoundaries };
	spCameraCom->view.setCenter(sf::Vector2f{ 0.f, 0.f });

	std::shared_ptr<CameraComponent> spCameraCom2 = GetCameraFromSystemCameraEntity();
	spCameraCom2->horizontalBorders = { -WorldGenerator::mapConfig.afarStarsBoundaries.y*2.f, WorldGenerator::mapConfig.afarStarsBoundaries.y*2.f };
	spCameraCom2->verticalBorders = { -WorldGenerator::mapConfig.afarStarsBoundaries.y*2.f, WorldGenerator::mapConfig.afarStarsBoundaries.y*2.f };
	spCameraCom2->view.setCenter(sf::Vector2f{ 0.f, 0.f });

	std::shared_ptr<CameraComponent> spCameraCom3 = GetCameraFromPlanetCameraEntity();
	spCameraCom3->horizontalBorders = { -WorldGenerator::mapConfig.planetCameraMaxBoundary, WorldGenerator::mapConfig.planetCameraMaxBoundary };
	spCameraCom3->verticalBorders = { -WorldGenerator::mapConfig.planetCameraMaxBoundary, WorldGenerator::mapConfig.planetCameraMaxBoundary };
	spCameraCom3->view.setCenter(sf::Vector2f{ 0.f, 0.f });

#ifdef OUTPUT_WORLD_STATISTICS
	SceneNodeSpaceObjectsCounter visitor(mapConfig);
	ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);
	visitor.OutputAllData();
#endif

#ifdef OUTPUT_WORLD_MEMORY_USAGE
	SceneNodeSpaceObjectsMemorySize visitor;
	ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);
	visitor.OutputAllData();
#endif

	ECSGame::Instance().GetSceneNode()->AddChild(spNode);
	ECSGame::Instance().GetSceneNode()->AddChild(spBackgroundNode);
	ECSGame::Instance().GetUINode()->AddChild(spSysNamesNode);
	ECSGame::Instance().GetUINode()->AddChild(spNebNamesNode);

	ECSGame::Instance().GetSceneNode()->ChangeChildOrder(wpBackgroundE.lock(), 0);
	ECSGame::Instance().GetUINode()->ChangeChildOrder(wpSysN.lock(), 0);
	ECSGame::Instance().GetUINode()->ChangeChildOrder(wpNebN.lock(), 1);

	ECSGame::Instance().GetUINode()->FindChild("LoadingScreen").lock()->GetEntity().lock()->hidden = true;
	ECSGame::Instance().GetUINode()->FindChild("LoadingScreen").lock()->FindChild("LoadingText").lock()->GetEntity().lock()->hidden = true;

	WorldGenerator::worldGenerated = true;
}