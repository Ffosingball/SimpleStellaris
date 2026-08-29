#pragma once

#include <memory>
#include "Component.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "TileMap.h"
//#include "ParticleSystem.h"
#include "SpaceObjectTypes.h"
#include <unordered_map>
#include "SpaceMapConfiguration.h"
#include "SceneNode.h"
#include "GameState.h"



//Component which tells that the object is a system of other object (for example
// star system or planetary system)
class ObjectSystemComponent : public Component 
{
public:
	SpaceSystemType systemType = SpaceSystemType::None;
	//vector of pointers to the central object in the system
	std::string systemName;
	//0 - outside of nebula, 1 - at the center of it
	float distToTheNebula{0.f};
	std::shared_ptr<SceneNode> spAllSystemObjectsNode;
};



//Component of the rectangular object
class RectangleShapeComponent : public Component
{
public:

	sf::RectangleShape shape;
};



//Component which stores data about all systems
class SystemPropertiesComponent : public Component
{
public:

	SpaceMapConfigurations mapConfig;
	std::unordered_map<int, std::weak_ptr<SceneNode>> systemsPositions;
};



//Component which tells ui entity to follow entity in the world
class UIFollowerComponent : public Component
{
public:

	std::weak_ptr<SceneNode> nodeToFollow;
	bool hideIfZoomLargeEnough = false;
	sf::Vector2f zoomLevelsAtWhichHideEntity{0.1f, 1.1f};
	bool hideIfOutsideOfCamera = true;
	bool hideAnyway = false;
	bool hideIfZoomSmallEnough = false;
};



//Component which visualize orbit of the object
class OrbitVisualizerComponent : public Component
{
public:

	sf::CircleShape orbitShape;
	//In A.U.
	double orbitSize{1.0};
};



//Component for free movement
class MovementComponent : public Component
{
public:

	sf::Vector2f velocity{ 0.f,0.f };
};



//Component of the star
class StarComponent : public Component
{
public:
	StarType starType;
	std::string starName;

	//Relative to sun size and mass
	float starSize{ 1.f };
	float starMass{ 1.f };

	//Distances in astronomical units
	double orbitRadius{ 0.0 };
	//0 rad - up, 0.5PI rad - right, PI rad - down, 1.5PI rad - left
	double initialRotationPosition{ 0.0 };
	//In radians per day
	double rotationalVelocity{ 0.0 };

	//Pointers to text i=and icon of the star
	std::weak_ptr<Entity> wpStarNameText;
};


//Component of the neutron star
class NeutronStarComponent : public Component
{
public:
	int pulsarNum{ 0 };
};



//Component of the nebula
class NebulaComponent : public Component
{
public:
	//In light years
	float nebulaSize{ 0.f };
	std::string nebulaName;
	std::weak_ptr<UIFollowerComponent> wpTextFollower;
};



//Component of the planet
class PlanetComponent : public Component
{
public:
	PlanetType planetType;
	//Earth radiuses
	//For gas giants this is not applied
	float planetSize{ 0.f };
	std::string planetName;
	//Distances in astronomical units
	double orbitRadius{ 0.0 };
	//0 rad - up, 0.5PI rad - right, PI rad - down, 1.5PI rad - left
	double initialRotationPosition{ 0.0 };
	//In radians per day
	double rotationalVelocity{ 0.0 };
	std::string planetIconTextureName;
	bool isMoon{ false };

	std::shared_ptr<SceneNode> spPlanetDistrictsNode;
};



//Component of the ring
class RingComponent : public Component
{
public:
	int ringNumber{ -1 };
	float ringSize{ 0.0f };
	std::string ringIconTextureName;
};



class HabitablePlanetComponent : public Component
{
public:
	DistanceToStar distanceToStar;
};


class StarIconComponent : public Component {};
class NebulaTextComponent : public Component {};



//Component of the tilemap
class TileMapComponent : public Component
{
public:

	TileMap tileMap;
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
	//Speed change depending on camera zoom
	float speedChange{ 5.f };
	//How far outside of camera borders I can consider objects as within camera borders
	float renderOutsideBoundsFor{ 20.f };
	bool cameraLocked{ false };
	std::weak_ptr<SceneNode> wpNodeLockedOn;
};



//Text component which has text
class TextComponent : public Component
{
public:
	//Stores text
	std::shared_ptr<sf::Text> text;
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
};


//Button component
class ButtonComponent : public Component
{
public:
	sf::Vector2f buttonSize{ 0.f, 0.f };
	bool enabled{ true };
	//Called if mouse is over the button and it pressed lmb or A(joystick)
	void (*onButtonPressed)(std::shared_ptr<Entity> spEntityOfButton) = nullptr;
	//Called if mouse enters button area
	void (*onButtonHovered)(std::shared_ptr<Entity> spEntityOfButton) = nullptr;
	//Called if mouse leaves button area
	void (*onButtonUnhovered)(std::shared_ptr<Entity> spEntityOfButton) = nullptr;
	//Called when button is released
	void (*onButtonReleased)(std::shared_ptr<Entity> spEntityOfButton) = nullptr;
	//Called when button is released and mouse is over the button
	void (*onButtonClicked)(std::shared_ptr<Entity> spEntityOfButton) = nullptr;
	bool isPressed{ false };
	bool isHovered{ false };
};


//District component
class DistrictComponent : public Component 
{
public:
	PlanetDistrictType districtType;
};