#pragma once

#include <memory>
#include "Component.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "TileMap.h"
//#include "ParticleSystem.h"
#include "SpaceObjectTypes.h"

//List of all components
enum class ComponentType
{
	ObjectSystem,
	Planet,
	Star,
	Text,
	Camera,
	UIPart,
	TileMap,
	Nebula,
	Movement,
	RectangleShape,
	SpaceObject
};


//Component which tells that the object is a system of other object (for example
// star system or planetary system)
class ObjectSystemComponent : public Component 
{
public:
	SpaceSystemType systemType;
	//vector of pointers to the central object in the system
	std::string systemName;
	//Distances in astronomical units
	float distTo2ndStar{ 0.f };
	float distTo3rdStar{ 0.f };
	int starAfar{ 0 };//which star is afar in ternary system

	ComponentType GetComponentType() const override;
};

//Component of the rectangular object
class RectangleShapeComponent : public Component
{
public:

	sf::RectangleShape shape;

	ComponentType GetComponentType() const override;
};

//Component for free movement
class MovementComponent : public Component
{
public:

	sf::Vector2f velocity{ 0.f,0.f };

	ComponentType GetComponentType() const override;
};

//Component of the star
class StarComponent : public Component
{
public:
	StarType starType;
	std::string starName;

	ComponentType GetComponentType() const override;
};

//Component of the nebula
class NebulaComponent : public Component
{
public:
	float nebulaSize{ 0.f };
	std::string nebulaName;

	ComponentType GetComponentType() const override;
};

//Component of the planet
class PlanetComponent : public Component
{
public:
	StarType planetType;
	//In kilometres
	float planetSize{ 0.f };
	std::string planetName;
	//In astronomical units
	float distanceFromStar{ 0.f };
	float rotationSpeed{ 0.f };
	//float axleSpeed{0.f};

	ComponentType GetComponentType() const override;
};

//Component of the tilemap
class TileMapComponent : public Component
{
public:

	TileMap tileMap;

	ComponentType GetComponentType() const override;
};


//Component of the camera
class CameraComponent : public Component
{
public:

	sf::View view;
	bool moveCamera{ false };
	//Left-Right
	sf::Vector2f horizontalBorders{ 0.f, 0.f };
	//Top-Bottom
	sf::Vector2f verticalBorders{ 0.f, 0.f };
	//Zoom speed
	float zoomingSpeed{0.1f};
	//Current camera zoom
	float currentZoom{ 1.f };
	//Max and min zooming
	sf::Vector2f zoomingBorders{ 0.5f, 2.f };
	//Camera size at zoom 1,1
	sf::Vector2f cameraSize{ 0.f, 0.f };

	ComponentType GetComponentType() const override;
};


//Text component which has text
class TextComponent : public Component
{
public:
	//Stores text
	std::shared_ptr<sf::Text> text;

	ComponentType GetComponentType() const override;
};

//UI component which has info to move or blink entity
class UIPartComponent : public Component
{
public:
	//Properties to make text to blink
	bool isBlinking{ false };
	float blinkingPeriod{ 1.f };
	bool decreasingVisibility{ false };
	float blinkTime{ 0.f };
	bool flatLine{ false };
	float flatLinePeriod{ 0.5f };
	//Properties to move text
	bool moveIt{ false };
	bool destroyAtTarget{ false };
	sf::Vector2f targetPosition{ 0.f,0.f };

	ComponentType GetComponentType() const override;
};



//Shortcut functions to get specific component from the entity
std::shared_ptr<TextComponent> GetTextComponent(const Entity& entity);
std::shared_ptr<CameraComponent> GetCameraComponent(const Entity& entity);
std::shared_ptr<UIPartComponent> GetUIPartComponent(const Entity& entity);
std::shared_ptr<TileMapComponent> GetTileMapComponent(const Entity& entity);
std::shared_ptr<StarComponent> GetStarComponent(const Entity& entity);
std::shared_ptr<PlanetComponent> GetPlanetComponent(const Entity& entity);
std::shared_ptr<ObjectSystemComponent> GetObjectSystemComponent(const Entity& entity);
std::shared_ptr<NebulaComponent> GetNebulaComponent(const Entity& entity);
std::shared_ptr<MovementComponent> GetMovementComponent(const Entity& entity);
std::shared_ptr<RectangleShapeComponent> GetRectangleShapeComponent(const Entity& entity);