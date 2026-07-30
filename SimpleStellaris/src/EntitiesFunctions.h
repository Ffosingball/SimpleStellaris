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
std::weak_ptr<Entity> CreateNewEntityAt(std::weak_ptr<SceneNode> parentNodeWP, const std::string newEntityName);
std::shared_ptr<CameraComponent> GetCameraFromCameraEntity();
std::shared_ptr<CameraComponent> GetCameraFromUICameraEntity();
void SetNewPosition(std::weak_ptr<Entity> entity, const sf::Vector2f position);


//Initializing functions
void InitializeTileMapAndCamera(const sf::Vector2u& windowSize);
void InitializeUICamera(const sf::Vector2u& windowSize);


//Reset functions


//UI functions
void SetupMoveTextProperties(const std::string textName, const float* xPos, const float* yPos, const sf::Vector2f velocity, const bool destroyAtTarget);
std::shared_ptr<Entity> CreateGenericText(const std::string textName, const int fontSize);
void InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const bool isBlinking, const bool isMoving, const float* targetX, const float* targetY, const sf::Vector2f velocity, const bool skipOriginReset);

//Animation functions


//Remove components functions