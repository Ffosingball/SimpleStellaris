#include "EntityManager.h"
#include <memory>
#include "System.h"
#include <string>
#include <vector>
#include <sigslot/signal.hpp>
#include "Entity.h"

//Create new entity
std::weak_ptr<Entity> EntityManager::NewEntity(const std::string& name) 
{
	//Create new entity
	std::shared_ptr<Entity> newEntity = std::make_shared<Entity>(name);

	//Add it to the list
	entities.emplace_back(newEntity);
	signals::onEntityCreated(newEntity);

	//return it
	std::weak_ptr<Entity> newEntityW = newEntity;
	return newEntityW;
}

//Destroy entity by reference to it
void EntityManager::DestroyEntity(std::weak_ptr<Entity> entity)
{
	signals::onEntityDestroyed(entity);

	std::shared_ptr<Entity> target = entity.lock();

	//Check if entity exist in the list
	for (int i = 0; i < entities.size(); i++) 
	{
		if (entities[i].get() == target.get())
		{
			//If so, then remove it
			entities.erase(entities.begin()+i);
			return;
		}
	}
}

std::weak_ptr<Entity> EntityManager::FindEntity(const std::string& name) const 
{
	std::weak_ptr<Entity> target;

	//Traverese the list
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->GetName() == name)
		{
			//If names are equal then return this entity
			target = entities[i];
			return target;
		}
	}

	//If not found then return nullptr
	return target;
}