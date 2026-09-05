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


void SetupPressedButtonTexture(std::shared_ptr<Entity> spEntity)
{
	std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
	std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
	spRecShape->shape.setTexture(spButton->hoveredPressedTexture.lock().get());
	spRecShape->shape.setTextureRect(spButton->hoveredPressedIntRect);
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

	SpaceMapConfigurations& mapConfig = WorldGenerator::Instance().getSpaceMapConfig();
	int yPos = (int)((position.y - mapConfig.verticalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems);
	int xPos = (int)((position.x - mapConfig.horizontalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems);

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


//Setup text properties, where it should move. Also either xPos or yPos
//should be not null!
//Worst case: O(N+2M) where N is number of entities in game and M number of components in the text
void SetupMoveTextProperties(const std::string textName, std::shared_ptr<SceneNode> nodeWithName, const float* xPos, const float* yPos, const sf::Vector2f velocity, const bool destroyAtTarget)
{
	//Get text by name
	std::weak_ptr<SceneNode> wGNode = nodeWithName->FindChild(textName);
	std::shared_ptr<Entity> sGText = wGNode.lock()->GetEntity().lock();
	//Get components
	std::shared_ptr<UIPartComponent> sGTextCom = sGText->FindComponent<UIPartComponent>().lock();
	std::shared_ptr<MovementComponent> sGMovCom = sGText->FindComponent<MovementComponent>().lock();

	//Set animation properties for gameText
	sGTextCom->moveIt = true;
	sGTextCom->destroyAtTarget = destroyAtTarget;
	//Set target position
	if (xPos == nullptr)
		sGTextCom->targetPosition = { sGText->GetTransformable().getPosition().x, *yPos };
	else if (yPos == nullptr)
		sGTextCom->targetPosition = { *xPos, sGText->GetTransformable().getPosition().y };
	else
		sGTextCom->targetPosition = { *xPos, *yPos };
	//Set velocity
	sGMovCom->velocity = velocity;
}



//This function creates a text
//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> CreateGenericText(const std::string textName, const int fontSize, const std::string fontName, sf::Color color, std::weak_ptr<SceneNode> spCreateAt)
{
	//create entity
	std::shared_ptr<Entity> spUI;
	if (spCreateAt.lock() != nullptr)
		spUI = CreateNewEntityAt(spCreateAt.lock(), textName).lock();
	else
		spUI = CreateNewEntityAtUINode(textName).lock();
	//Add component
	//spUI->AddComponent<UIPartComponent>();
	std::shared_ptr<TextComponent> spUICom = spUI->AddComponent<TextComponent>().lock();
	//Get font from the resource manager
	std::shared_ptr<sf::Font> fontPtr = ResourceManager::Instance().GetFont(fontName).lock();
	//Set text properties
	spUICom->text = std::make_shared<sf::Text>(*fontPtr);
	spUICom->text->setFillColor(color);
	//spUICom->text->setOutlineColor(sf::Color(100,100,100));
	//spUICom->text->setOutlineThickness(1.2f);
	spUICom->text->setCharacterSize(fontSize);

	return spUI;
}



//Creates text without moving animation
//Worst case: O(6N+3M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const std::string fontName, bool centerText, sf::Color color = sf::Color{ 255,255,255 }, std::weak_ptr<SceneNode> spCreateAt)
{
	//Check if text exist then use existing one, otherwise create new one
	std::shared_ptr<Entity> spUI = CreateGenericText(name, fontSize, fontName, color, spCreateAt);
	spUI->AddComponent<MovementComponent>();
	//Get component
	//std::shared_ptr<UIPartComponent> spUICom = spUI->FindComponent<UIPartComponent>().lock();
	std::shared_ptr<TextComponent> spTextCom = spUI->FindComponent<TextComponent>().lock();
	//set text
	spTextCom->text->setString(text);
	//Set text position
	if (centerText)
		gel::CentreText(*spTextCom->text, sf::Vector2{ 0.f,0.f });

	spUI->SetPosition(position);

	return spUI;
}



//Creates text with moving animation
//Worst case: O(2N+9M+3K) where N is number entities in game and M number of components in text
// and K in number of components available
void InitializeMovingText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const bool isBlinking = false, const bool isMoving = false, const float* targetX = nullptr, const float* targetY = nullptr, const sf::Vector2f velocity = { 0.f,0.f }, const bool skipOriginReset = false)
{
	//Create new text
	std::shared_ptr<Entity> spUI = CreateGenericText(name, fontSize, "wakaeueu", sf::Color{ 255,255,255 });
	//Get component
	//std::shared_ptr<UIPartComponent> spUICom = spUI->FindComponent<UIPartComponent>().lock();
	std::shared_ptr<TextComponent> spTextCom = spUI->FindComponent<TextComponent>().lock();
	//Add component
	std::shared_ptr<MovementComponent> spMovCom = spUI->AddComponent<MovementComponent>().lock();
	//set text
	spTextCom->text->setString(text);
	//Set text position
	spUI->SetPosition(position);
	//Check if text should blink
	if (isBlinking)
	{
		//Set blinking properties
		std::shared_ptr<UIPartComponent> spUICom = spUI->AddComponent<UIPartComponent>().lock();
		spUICom->isBlinking = true;
		spUICom->decreasingVisibility = false;
		spUICom->blinkTime = spUICom->blinkingPeriod;
		spUICom->flatLine = true;
	}
	//Check if text should move
	if (isMoving)
	{
		//Set moving animation properties
		SetupMoveTextProperties(name, ECSGame::Instance().GetUINode(), targetX, targetY, velocity, false);
	}
	//Reset text origin to center of the text if needed
	if (!skipOriginReset)
		gel::SetTextOrigin(*(spTextCom->text), position);
}



//Worst case: O(4N+3M+6K) where N is number of components in provided entity and M is
//number of components available in game and K number of components to add to the text
std::shared_ptr<Entity> CreateSystemText(std::shared_ptr<SceneNode> systemNode, std::shared_ptr<SceneNode> spNodeToFollow, std::string& entityName, bool hideIfZoomLarge)
{
	//static int counter = 0;
	float fontSize = 22;
	float nebulaFontSize = 40;

	float uiSize = ECSGame::Instance().GetUISize();
	std::string name{ "UNDEFINED" };
	std::shared_ptr<Entity> spEntityToFollow = spNodeToFollow->GetEntity().lock();
	if (spEntityToFollow->HasComponent<ObjectSystemComponent>())
		name = spEntityToFollow->FindComponent<ObjectSystemComponent>().lock()->systemName;
	else if (spEntityToFollow->HasComponent<StarComponent>())
		name = spEntityToFollow->FindComponent<StarComponent>().lock()->starName;
	else if (spEntityToFollow->HasComponent<PlanetComponent>())
		name = spEntityToFollow->FindComponent<PlanetComponent>().lock()->planetName;
	else if (spEntityToFollow->HasComponent<NebulaComponent>())
	{
		name = spEntityToFollow->FindComponent<NebulaComponent>().lock()->nebulaName;
		fontSize = nebulaFontSize;
	}
	else
		name = entityName;
	//Create text
	std::shared_ptr<Entity> spText = InitializeText(entityName, name, (int)(fontSize * uiSize), sf::Vector2f{ 0,0 }, "Pixel", false, sf::Color::White, systemNode);

	//Add component
	std::shared_ptr<UIFollowerComponent> spUIFollower = spText->AddComponent<UIFollowerComponent>().lock();
	spUIFollower->nodeToFollow = spNodeToFollow;
	spUIFollower->hideIfZoomLargeEnough = hideIfZoomLarge;

	std::shared_ptr<TextComponent> spUIText = spText->FindComponent<TextComponent>().lock();
	gel::CentreText(*spUIText->text, sf::Vector2f{ 0,fontSize * 2.f * uiSize });
	spUIText->text->setFillColor(sf::Color(229, 229, 229));
	spUIText->text->setOutlineColor(sf::Color(50, 50, 50));
	spUIText->text->setOutlineThickness(1.f * uiSize);

	//counter++;

	return spText;
}


//Creates icons for system overview
//Worst case: O(4N+M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> CreateIconForSystemOverview(std::shared_ptr<SceneNode> nodeToFollow, std::shared_ptr<SceneNode> createIconIn, std::string iconTexture, std::string name, bool hideIfZoomLarge, sf::Vector2f iconSize, bool hideIfZoomSmall, sf::Vector2f zoomLevelAtWhichHide)
{
	//if (iconTexture == "CenterOfMassIcon")
	//	std::cout << systemNode->GetCombinedParentsNames()<<'\n';
	float uiSize = ECSGame::Instance().GetUISize();

	//Create selection icon
	std::shared_ptr<Entity> spSSIcon = CreateNewEntityAt(createIconIn, name).lock();
	//Add component
	//spSSIcon->AddComponent<UIPartComponent>();
	std::shared_ptr<RectangleShapeComponent> spRectShape = spSSIcon->AddComponent<RectangleShapeComponent>().lock();
	SetupRectangleShape(spRectShape, iconSize * uiSize, iconTexture);
	std::shared_ptr<UIFollowerComponent> spUIFollower = spSSIcon->AddComponent<UIFollowerComponent>().lock();
	spUIFollower->nodeToFollow = nodeToFollow;
	spUIFollower->hideIfZoomLargeEnough = hideIfZoomLarge;
	spUIFollower->hideIfZoomSmallEnough = hideIfZoomSmall;
	spUIFollower->zoomLevelsAtWhichHideEntity = zoomLevelAtWhichHide;

	return spSSIcon;
}



void CreateOrbitFor(std::shared_ptr<SceneNode> spParentNode, std::string name, bool inheritParentPosition, double orbitRadius, std::weak_ptr<SceneNode> wpNodeToFollow, float outlineThikness, sf::Color outlineColor, bool hideIfZoomLarge)
{
	//Create orbit
	std::shared_ptr<Entity> spOrbitE = CreateNewEntityAt(spParentNode, name).lock();
	//Add component
	//spOrbitE->AddComponent<UIPartComponent>();
	spOrbitE->inheritParentPosition = inheritParentPosition;

	std::shared_ptr<OrbitVisualizerComponent> spOrbitVis = spOrbitE->AddComponent<OrbitVisualizerComponent>().lock();
	spOrbitVis->orbitShape.setPointCount(300);
	spOrbitVis->orbitShape.setOutlineColor(outlineColor);
	spOrbitVis->orbitShape.setOutlineThickness(outlineThikness);
	spOrbitVis->orbitShape.setFillColor(sf::Color(0, 0, 0, 0));
	spOrbitVis->orbitSize = orbitRadius * 0.9996;

	std::shared_ptr<UIFollowerComponent> spUIFollower = spOrbitE->AddComponent<UIFollowerComponent>().lock();
	spUIFollower->nodeToFollow = wpNodeToFollow;
	spUIFollower->hideIfZoomLargeEnough = hideIfZoomLarge;
	spUIFollower->hideIfOutsideOfCamera = false;
}