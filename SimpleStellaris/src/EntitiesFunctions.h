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
std::weak_ptr<Entity> CreateNewEntityAtRoot(const std::string name);
std::weak_ptr<Entity> CreateNewEntityAt(const std::string nodeName, const std::string newEntityName);
std::weak_ptr<Entity> CreateNewEntityAt(std::shared_ptr<SceneNode> parentNodePtr, const std::string newEntityName);
std::shared_ptr<CameraComponent> GetCameraFromCameraEntity();
std::shared_ptr<CameraComponent> GetCameraFromUICameraEntity();
//void SetNewPosition(std::weak_ptr<Entity> entity, const sf::Vector2f position);
void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath);
void SetupRectangleShape(std::shared_ptr<RectangleShapeComponent> recShape, const sf::Vector2f size, const std::string texturePath, sf::IntRect intRect);
sf::Vector2f ConvertWindowPositionToWorld(sf::View cameraView, sf::Vector2i position);
sf::Vector2i ConvertWorldPositionToWindow(sf::View cameraView, sf::Vector2f position);
int GetKeyForSystemsPosition(sf::Vector2i gridPosition);
std::vector<std::shared_ptr<SceneNode>> GetAllSystemsNearPosition(sf::Vector2f position);
std::shared_ptr<SystemPropertiesComponent> GetSystemPropertiesFromSpaceMap();


//Initializing functions
void InitializeCamera(const sf::Vector2u& windowSize);
void InitializeUICamera(const sf::Vector2u& windowSize);
void CreateSpaceObjects();
void CreateDebugText();
void CreateUI();


//Reset functions


//UI functions
void SetupMoveTextProperties(const std::string textName, const float* xPos, const float* yPos, const sf::Vector2f velocity, const bool destroyAtTarget);
std::shared_ptr<Entity> CreateGenericText(const std::string textName, const int fontSize);
void InitializeMovingText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const bool isBlinking, const bool isMoving, const float* targetX, const float* targetY, const sf::Vector2f velocity, const bool skipOriginReset);
void InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position);

//Animation functions


//Remove components functions