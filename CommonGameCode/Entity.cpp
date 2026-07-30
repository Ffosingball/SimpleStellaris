#include "Entity.h"
#include <string>
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "System.h"
#include <iostream>


//Find component by its type
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
bool Entity::HasComponent(ComponentType ct) const 
{
	int index = FindComponentIndex(ct);

	if (index == -1)
		return false;
	else
		return true;
}

//Add new component
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

Entity::~Entity() 
{
	//On entity destruction remove all components
	while (components.size() > 0) 
	{
		//std::cout <<components.size()<<": "<< PrintComponentName(components[0]->GetComponentType())<<'\n';
		components.erase(components.begin());
	}
}