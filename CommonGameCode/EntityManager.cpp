#include "EntityManager.h"
#include <memory>
#include "System.h"
#include <string>
#include <vector>
#include <sigslot/signal.hpp>
#include "Entity.h"
#include <iostream>
#include "SceneNode.h"

//Create new entity
//Worst case: O(1), but depends on which functions are subscribed to 
//onEntityCreated signal
std::weak_ptr<Entity> EntityManager::NewEntity(const std::string& name) 
{
	//Create new entity
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(name);

	//Add it to the map
	entities[newEntity] = newEntity;
	signals::onEntityCreated(newEntity);

	//return it
	std::weak_ptr<Entity> newEntityW = newEntity;
	return newEntityW;
}

//Destroy entity by reference to it
//Worst case: O(1), but depends on which functions are subscribed to onEntityDestroyed signal
void EntityManager::DestroyEntity(std::weak_ptr<Entity> entity)
{
	signals::onEntityDestroyed(entity);

	std::shared_ptr<Entity> target = entity.lock();

	//Check if entity exist in the list
	if (entities.contains(target))
		entities.erase(target);
}

//Worst case: O(N) where N is number of entities in game
void EntityManager::OutputAllEntitiesNames() 
{
	for (auto& [key, value] : entities)
	{
		if (value == nullptr)
			std::cout << "Entity is nullptr\n";
		else
			std::cout << value <<"; name: "<<value->GetName()<<'\n';
	}
}