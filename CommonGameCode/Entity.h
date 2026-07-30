#pragma once
#include <string>
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory>

//Template for the entity
class Entity 
{
public:
	//Construct entity with name
	Entity(const std::string& name) : name{ name } {}

	//Forbid copying entities
	Entity() = delete;
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	~Entity();

	//Getters and setters
	const std::string& GetName() const { return name; }
	const sf::Transformable& GetTransformable() const { return transformable; }
	void SetTransformable(const sf::Transformable& transformable)
	{ this->transformable=transformable; }

	//Find component by type
	std::weak_ptr<Component> FindComponent(ComponentType ct) const;

	//Check if entity has this type of component
	bool HasComponent(ComponentType ct) const;

	//Add new component
	std::weak_ptr<Component> AddComponent(ComponentType ct);

	//Remove component
	void RemoveComponent(ComponentType ct);

private:
	std::string name;//name of the entity
	sf::Transformable transformable;//Transform of the entity
	std::vector<std::shared_ptr<Component>> components;//all components of the entity

	//Find index of the component in the list
	int FindComponentIndex(ComponentType ct) const;
};