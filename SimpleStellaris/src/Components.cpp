#include <memory>
#include "Component.h"
#include "Components.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "SpaceObjectTypes.h"


//This function translates componentType into string
//Worst case: O(N) where N is number of components available in game
std::string PrintComponentName(std::shared_ptr<Component> spComponent) 
{
	if (std::dynamic_pointer_cast<StarComponent>(spComponent))
		return "Star";
	else if (std::dynamic_pointer_cast<PlanetComponent>(spComponent))
		return "Planet";
	else if (std::dynamic_pointer_cast<ObjectSystemComponent>(spComponent))
		return "ObjectSystem";
	else if (std::dynamic_pointer_cast<TextComponent>(spComponent))
		return "Text";
	else if (std::dynamic_pointer_cast<CameraComponent>(spComponent))
		return "Camera";
	else if (std::dynamic_pointer_cast<UIPartComponent>(spComponent))
		return "UIPart";
	else if (std::dynamic_pointer_cast<NebulaComponent>(spComponent))
		return "Nebula";
	else if (std::dynamic_pointer_cast<TileMapComponent>(spComponent))
		return "TileMap";
	else if (std::dynamic_pointer_cast<MovementComponent>(spComponent))
		return "Movement";
	else if (std::dynamic_pointer_cast<RectangleShapeComponent>(spComponent))
		return "RectangleShape";
	else if (std::dynamic_pointer_cast<SystemPropertiesComponent>(spComponent))
		return "SystemProperties";
	else if (std::dynamic_pointer_cast<UIFollowerComponent>(spComponent))
		return "UIFollower";
	else if (std::dynamic_pointer_cast<HabitablePlanetComponent>(spComponent))
		return "HabitablePlanet";
	else if (std::dynamic_pointer_cast<OrbitVisualizerComponent>(spComponent))
		return "OrbitVisualizer";
	else if (std::dynamic_pointer_cast<RingComponent>(spComponent))
		return "Ring";
	else
		return "---";
}