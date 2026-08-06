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

	//Worst case: O(N) where N is number of components in entity
	~Entity();

	//Getters and setters
	//Worst case: O(1)
	const std::string& GetName() const { return name; }
	//Worst case: O(1)
	const sf::Transformable& GetTransformable() const { return transformable; }
	//Worst case: O(1)
	void SetTransformable(const sf::Transformable& transformable)
	{ this->transformable=transformable; }

	//Find component by type
	//Worst case: O(N) where N is number of components in entity
	std::weak_ptr<Component> FindComponent(ComponentType ct) const;
	//Check if entity has this type of component
	//Worst case: O(N) where N is number of components in entity
	bool HasComponent(ComponentType ct) const;
	//Add new component
	//Worst case: O(N+M) where N is number of components in entity and M
	//number of components available in game
	std::weak_ptr<Component> AddComponent(ComponentType ct);
	//Remove component
	//Worst case: O(N) where N is number of components in entity
	void RemoveComponent(ComponentType ct);

	//Move entity
	//Worst case: O(1)
	void Move(sf::Vector2f distance);
	//Move entity within given borders
	//Worst case: O(1)
	void MoveWithin(sf::Vector2f distance, sf::Vector2f horizontalBoundaries, sf::Vector2f verticalBoundaries);
	//Set new entity position
	//Worst case: O(1)
	void SetPosition(sf::Vector2f position);
	//Get new entity position
	//Worst case: O(1)
	sf::Vector2f GetPosition();

	//Flag to tell if entity should be drawn or not
	bool hidden = false;
	//Flag to tell if entity should inherit position of the parent or not
	bool inheritParentPosition = true;

private:
	std::string name;//name of the entity
	sf::Transformable transformable;//Transform of the entity
	std::vector<std::shared_ptr<Component>> components;//all components of the entity

	//Find index of the component in the list
	//Worst case: O(N) where N is number of components in entity
	int FindComponentIndex(ComponentType ct) const;
};