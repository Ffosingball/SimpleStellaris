#include "Entity.h"
#include <string>
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "System.h"
#include <iostream>
#include "CommonGameCode.h"


//Find component by its type
//Worst case: O(N) where N is number of components in entity
std::weak_ptr<Component> Entity::FindComponent(ComponentType ct) const
{
	int index = FindComponentIndex(ct);

	std::weak_ptr<Component> wp;

	//Check if component exist, if not return null
	if (index == -1)
		return wp;
	else
	{
		//If exist, then return it
		wp = components[index];
		return wp;
	}

}

//Check if entity has component of this type
//Worst case: O(N) where N is number of components in entity
bool Entity::HasComponent(ComponentType ct) const 
{
	int index = FindComponentIndex(ct);

	if (index == -1)
		return false;
	else
		return true;
}

//Add new component
//Worst case: O(N+M) where N is number of components in entity and M
//number of components available in game
std::weak_ptr<Component> Entity::AddComponent(ComponentType ct) 
{
	std::weak_ptr<Component> wp;

	int index = FindComponentIndex(ct);

	//Check if component does not exist
	if (index == -1) 
	{
		//Then add it to the list
		std::shared_ptr<Component> sp = ComponentFactory(ct);
		components.emplace_back(sp);
		wp = sp;

		signals::onComponentAdded(*this,ct);

		//And return it
		return wp;
	}
	else
	{
		//If component exist, then just return it
		wp = components[index];
		return wp;
	}
}

//Worst case: O(N) where N is number of components in entity
void Entity::RemoveComponent(ComponentType ct) 
{
	int index = FindComponentIndex(ct);

	//Check if component exist, then remove it
	if (index != -1) 
	{
		signals::onComponentRemove(*this, ct);

		components.erase(components.begin()+index);
	}
}

//Worst case: O(N) where N is number of components in entity
int Entity::FindComponentIndex(ComponentType ct) const 
{
	for (int i = 0; i < components.size(); i++) 
	{
		//If component is found, then return its index
		if (components[i]->GetComponentType() == ct)
			return i;
	}

	//Otherwise return -1
	return -1;
}

//Worst case: O(N) where N is number of components in entity
Entity::~Entity() 
{
	//On entity destruction remove all components
	while (components.size() > 0) 
	{
		//std::cout <<components.size()<<": "<< PrintComponentName(components[0]->GetComponentType())<<'\n';
		components.erase(components.begin());
	}
}

//Move entity
//Worst case: O(1)
void Entity::Move(sf::Vector2f distance) 
{
	sf::Vector2f position = transformable.getPosition();
	position += distance;
	transformable.setPosition(position);
}

//Move entity within given borders
//Worst case: O(1)
void Entity::MoveWithin(sf::Vector2f distance, sf::Vector2f horizontalBoundaries, sf::Vector2f verticalBoundaries) 
{
	sf::Vector2f position = transformable.getPosition();
	position = { gel::clamp(distance.x + position.x, horizontalBoundaries.x, horizontalBoundaries.y), gel::clamp(distance.y + position.y, verticalBoundaries.x, verticalBoundaries.y) };
	transformable.setPosition(position);
}

//Set new entity position
//Worst case: O(1)
void Entity::SetPosition(sf::Vector2f position) 
{
	transformable.setPosition(position);
}

//Get new entity position
//Worst case: O(1)
sf::Vector2f Entity::GetPosition() 
{
	return transformable.getPosition();
}