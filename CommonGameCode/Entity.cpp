#include "Entity.h"
#include <string>
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "System.h"
#include <iostream>
#include "CommonGameCode.h"


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