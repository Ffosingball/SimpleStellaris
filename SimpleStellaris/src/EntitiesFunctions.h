#pragma once
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SceneNode.h"
#include <memory>
#include "Components.h"
#include "Component.h"
#include <SFML/Graphics.hpp>
#include "GameState.h"

//Put here functions for creation and managing entities!

//Helper functions
//Worst case: O(1)
std::weak_ptr<Entity> CreateNewEntityAtRoot(const std::string name);
//Worst case: O(1)
std::weak_ptr<Entity> CreateNewEntityAtUIRoot(const std::string name);
//Worst case: O(2*N) where N is number of entities in game
std::weak_ptr<Entity> CreateNewEntityAt(const std::string nodeName, const std::string newEntityName, const std::shared_ptr<SceneNode> rootNode);
//Worst case: O(1)
std::weak_ptr<Entity> CreateNewEntityAt(const std::shared_ptr<SceneNode> parentNodePtr, const std::string newEntityName);
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCurrentlyActiveCamera();
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromUICameraEntity();
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromBackgroundCameraEntity();
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromSystemCameraEntity();
//Worst case: O(N+M) where N is number of scene nodes in game and M number of components in camera
std::shared_ptr<CameraComponent> GetCameraFromSpaceCameraEntity();
//Worst case: O(1)
void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath);
//Worst case: O(1)
sf::Vector2f ConvertWindowPositionToWorld(sf::View& cameraView, sf::Vector2i position);
//Worst case: O(1)
sf::Vector2i ConvertWorldPositionToWindow(sf::View& cameraView, sf::Vector2f position);
//Worst case: O(1)
int GetKeyForSystemsPosition(sf::Vector2i gridPosition);
//Worst case: O(N+M) where N is number of entities in game and M number of components in spaceMap
std::vector<std::shared_ptr<SceneNode>> GetAllSystemsNearPosition(sf::Vector2f position);
//Worst case: O(N+M) where N is number of entities in game and M number of components in spaceMap
std::shared_ptr<SystemPropertiesComponent> GetSystemPropertiesFromSpaceMap();
//Worst case: O(1)
bool IsWorldPosInsideOfCamera(std::shared_ptr<CameraComponent> spCamCom, sf::Vector2f worldPos);
//Worst case: O(N) where N is number of months in a year
std::string GetDateFromDays(int daysPast);


//Initializing functions
//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
void InitializeMouseIcon();
//Worst case: O(2N+4M) where N is number of systems + stars to create and M number of tiles to create
void CreateSpaceObjects();
//Worst case: O((6N+3M)*K) where N is number of components in entity and M number of components
//available in game and K number of texts to create
void CreateDebugText();
//Worst case: O(4N+M) where N is number of components in entity and M number of components
//available in game
void CreateUI();
//Worst case: O(12N+4M) where N is number of components available in game and M number of components
//available in game
void InitializeAllCameras(const sf::Vector2u& windowSize);


//Reset functions


//UI functions
//Worst case: O(N+2M) where N is number of entities in game and M number of components in the text
void SetupMoveTextProperties(const std::string textName, std::shared_ptr<SceneNode> nodeWithName, const float* xPos, const float* yPos, const sf::Vector2f velocity, const bool destroyAtTarget);
//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> CreateGenericText(const std::string textName, const int fontSize);
//Worst case: O(2N+9M+3K) where N is number entities in game and M number of components in text
// and K in number of components available
void InitializeMovingText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const bool isBlinking, const bool isMoving, const float* targetX, const float* targetY, const sf::Vector2f velocity, const bool skipOriginReset);
//Worst case: O(6N+3M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position);
//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> InitializeTextAt(std::shared_ptr<SceneNode> spNode, const std::string name, const std::string text, const int fontSize, const sf::Vector2f position);
//Worst case: O(4N+3M+6K) where N is number of components in provided entity and M is
//number of components available in game and K number of components to add to the text
std::shared_ptr<Entity> CreateSystemText(std::shared_ptr<SceneNode> systemNode, std::shared_ptr<SceneNode> spNodeToFollow, std::string& entityName, bool hideIfZoomLarge);
//Worst case: O(4N+M) where N is number of components in entity and M number of components
//available in game
void CreateIconForSystemOverview(std::shared_ptr<SceneNode> nodeToFollow, std::shared_ptr<SceneNode> createIconIn, std::string iconTexture, std::string name);

//Animation functions


//Remove components functions