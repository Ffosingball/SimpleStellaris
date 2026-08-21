#pragma once
#include <string>
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>
#include <vector>

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

	//Find component by its type
	//Worst case: O(N) where N is number of components in entity
	template<typename T>
	std::weak_ptr<T> FindComponent() const
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		for (int i = 0; i < components.size(); i++)
		{
			std::shared_ptr<T> spCom = std::dynamic_pointer_cast<T>(components[i]);
			if (spCom)
				return spCom;
		}

		return {};
	}

	//Check if entity has this type of component
	//Worst case: O(N) where N is number of components in entity
	template<typename T>
	bool HasComponent() const 
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		if (FindComponent<T>().lock()==nullptr)
			return false;
		else
			return true;
	}

	//Add new component
	//Worst case: O(N+M) where N is number of components in entity and M
	//number of components available in game
	template<typename T>
	std::weak_ptr<T> AddComponent() 
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		std::weak_ptr<T> wp = FindComponent<T>();

		//Check if component does not exist
		if (wp.lock()==nullptr)
		{
			//Then add it to the list
			std::shared_ptr<T> sp = std::make_shared<T>();
			components.emplace_back(sp);
			wp = sp;
			//signals::onComponentAdded(*this, ct);
			//And return it
			return wp;
		}
		else
			return wp;
	}

	//Remove component
	//Worst case: O(N) where N is number of components in entity
	template<typename T>
	void RemoveComponent() 
	{
		static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

		int index = -1;
		for (int i = 0; i < components.size(); i++)
		{
			//If component is found, then return its index
			std::shared_ptr<T> spCom = std::dynamic_pointer_cast<T>(components[i]);
			if (spCom)
			{
				index = i;
				break;
			}
		}

		//Check if component exist, then remove it
		if (index != -1)
		{
			//signals::onComponentRemove(*this, ct);
			components.erase(components.begin() + index);
		}
	}

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
};