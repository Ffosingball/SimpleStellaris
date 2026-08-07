#include <memory>
#include "Component.h"
#include "Components.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "SpaceObjectTypes.h"

//Component to render tileMap
//Worst case: O(1)
ComponentType TileMapComponent::GetComponentType() const
{
	return ComponentType::TileMap;
}

//Component of the star
//Worst case: O(1)
ComponentType StarComponent::GetComponentType() const
{
	return ComponentType::Star;
}

//Component of the movement
//Worst case: O(1)
ComponentType MovementComponent::GetComponentType() const
{
	return ComponentType::Movement;
}

//Component of the planet
//Worst case: O(1)
ComponentType PlanetComponent::GetComponentType() const
{
	return ComponentType::Planet;
}

//Component of the objectSystem
//Worst case: O(1)
ComponentType ObjectSystemComponent::GetComponentType() const
{
	return ComponentType::ObjectSystem;
}

//Component of the nebula
//Worst case: O(1)
ComponentType NebulaComponent::GetComponentType() const
{
	return ComponentType::Nebula;
}

//Component for text
//Worst case: O(1)
ComponentType TextComponent::GetComponentType() const
{
	return ComponentType::Text;
}

//Component of camera
//Worst case: O(1)
ComponentType CameraComponent::GetComponentType() const
{
	return ComponentType::Camera;
}

//Component to render UI parts
//Worst case: O(1)
ComponentType UIPartComponent::GetComponentType() const
{
	return ComponentType::UIPart;
}

//Component to render entity
//Worst case: O(1)
ComponentType RectangleShapeComponent::GetComponentType() const
{
	return ComponentType::RectangleShape;
}

//Worst case: O(1)
ComponentType SystemPropertiesComponent::GetComponentType() const
{
	return ComponentType::SystemProperties;
}

//Worst case: O(1)
ComponentType UIFollowerComponent::GetComponentType() const
{
	return ComponentType::UIFollower;
}

//Worst case: O(1)
ComponentType HabitablePlanetComponent::GetComponentType() const
{
	return ComponentType::HabitablePlanet;
}


//This function creates any component and return shared pointer to it
//Worst case: O(N) where N is number of available components in game
std::shared_ptr<Component> ComponentFactory(ComponentType ct) 
{
	switch (ct)
	{
	case ComponentType::TileMap:
		return std::make_shared<TileMapComponent>();
	case ComponentType::Star:
		return std::make_shared<StarComponent>();
	case ComponentType::Planet:
		return std::make_shared<PlanetComponent>();
	case ComponentType::ObjectSystem:
		return std::make_shared<ObjectSystemComponent>();
	case ComponentType::Nebula:
		return std::make_shared<NebulaComponent>();
	case ComponentType::Text:
		return std::make_shared<TextComponent>();
	case ComponentType::Camera:
		return std::make_shared<CameraComponent>();
	case ComponentType::UIPart:
		return std::make_shared<UIPartComponent>();
	case ComponentType::Movement:
		return std::make_shared<MovementComponent>();
	case ComponentType::RectangleShape:
		return std::make_shared<RectangleShapeComponent>();
	case ComponentType::SystemProperties:
		return std::make_shared<SystemPropertiesComponent>();
	case ComponentType::UIFollower:
		return std::make_shared<UIFollowerComponent>();
	case ComponentType::HabitablePlanet:
		return std::make_shared<HabitablePlanetComponent>();
	}

	return {};
}

//This function translates componentType into string
//Worst case: O(N) where N is number of components available in game
std::string PrintComponentName(ComponentType ct) 
{
	switch (ct)
	{
	case ComponentType::Star:
		return "Star";
	case ComponentType::Planet:
		return "Planet";
	case ComponentType::ObjectSystem:
		return "ObjectSystem";
	case ComponentType::Text:
		return "Text";
	case ComponentType::Camera:
		return "Camera";
	case ComponentType::UIPart:
		return "UIPart";
	case ComponentType::Nebula:
		return "Nebula";
	case ComponentType::TileMap:
		return "TileMap";
	case ComponentType::Movement:
		return "Movement";
	case ComponentType::RectangleShape:
		return "RectangleShape";
	case ComponentType::SystemProperties:
		return "SystemProperties";
	case ComponentType::UIFollower:
		return "UIFollower";
	case ComponentType::HabitablePlanet:
		return "HabitablePlanet";
	}

	return "---";
}


//These functions just return specific component of the entity
//Worst case: O(N) where N is number of components in entity
std::shared_ptr<StarComponent> GetStarComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Star);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<StarComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<NebulaComponent> GetNebulaComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Nebula);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<NebulaComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<TileMapComponent> GetTileMapComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::TileMap);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<TileMapComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<PlanetComponent> GetPlanetComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Planet);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<PlanetComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<ObjectSystemComponent> GetObjectSystemComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::ObjectSystem);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<ObjectSystemComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<TextComponent> GetTextComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Text);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<TextComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<CameraComponent> GetCameraComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Camera);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<CameraComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<MovementComponent> GetMovementComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Movement);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<MovementComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<RectangleShapeComponent> GetRectangleShapeComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::RectangleShape);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<RectangleShapeComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<UIPartComponent> GetUIPartComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::UIPart);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<UIPartComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<SystemPropertiesComponent> GetSystemPropertiesComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::SystemProperties);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<SystemPropertiesComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<UIFollowerComponent> GetUIFollowerComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::UIFollower);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<UIFollowerComponent>(spComponentBase);
}

//Worst case: O(N) where N is number of components in entity
std::shared_ptr<HabitablePlanetComponent> GetHabitablePlanetComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::HabitablePlanet);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<HabitablePlanetComponent>(spComponentBase);
}