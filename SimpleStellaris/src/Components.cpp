#include <memory>
#include "Component.h"
#include "Components.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "SpaceObjectTypes.h"

//Component to render tileMap
ComponentType TileMapComponent::GetComponentType() const
{
	return ComponentType::TileMap;
}

//Component of the star
ComponentType StarComponent::GetComponentType() const
{
	return ComponentType::Star;
}

//Component of the movement
ComponentType MovementComponent::GetComponentType() const
{
	return ComponentType::Movement;
}

//Component of the planet
ComponentType PlanetComponent::GetComponentType() const
{
	return ComponentType::Planet;
}

//Component of the objectSystem
ComponentType ObjectSystemComponent::GetComponentType() const
{
	return ComponentType::ObjectSystem;
}

//Component of the nebula
ComponentType NebulaComponent::GetComponentType() const
{
	return ComponentType::Nebula;
}

//Component for text
ComponentType TextComponent::GetComponentType() const
{
	return ComponentType::Text;
}

//Component of camera
ComponentType CameraComponent::GetComponentType() const
{
	return ComponentType::Camera;
}

//Component to render UI parts
ComponentType UIPartComponent::GetComponentType() const
{
	return ComponentType::UIPart;
}

//Component to render entity
ComponentType RectangleShapeComponent::GetComponentType() const
{
	return ComponentType::RectangleShape;
}


//This function creates any component and return shared pointer to it
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
	}

	return {};
}

//This function translates componentType into string
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
	}

	return "---";
}


//These functions just return specific component of the entity
std::shared_ptr<StarComponent> GetStarComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Star);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<StarComponent>(spComponentBase);
}

std::shared_ptr<NebulaComponent> GetNebulaComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Nebula);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<NebulaComponent>(spComponentBase);
}

std::shared_ptr<TileMapComponent> GetTileMapComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::TileMap);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<TileMapComponent>(spComponentBase);
}

std::shared_ptr<PlanetComponent> GetPlanetComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Planet);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<PlanetComponent>(spComponentBase);
}

std::shared_ptr<ObjectSystemComponent> GetObjectSystemComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::ObjectSystem);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<ObjectSystemComponent>(spComponentBase);
}

std::shared_ptr<TextComponent> GetTextComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Text);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<TextComponent>(spComponentBase);
}

std::shared_ptr<CameraComponent> GetCameraComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Camera);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<CameraComponent>(spComponentBase);
}

std::shared_ptr<MovementComponent> GetMovementComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::Movement);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<MovementComponent>(spComponentBase);
}

std::shared_ptr<RectangleShapeComponent> GetRectangleShapeComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::RectangleShape);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<RectangleShapeComponent>(spComponentBase);
}

std::shared_ptr<UIPartComponent> GetUIPartComponent(const Entity& entity)
{
	std::weak_ptr<Component> wpComponentBase = entity.FindComponent(ComponentType::UIPart);
	std::shared_ptr<Component> spComponentBase = wpComponentBase.lock();
	return std::static_pointer_cast<UIPartComponent>(spComponentBase);
}