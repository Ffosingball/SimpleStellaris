#pragma once
#include <memory>
#include <string>
#include <vector>
#include <sigslot/signal.hpp>
#include "Entity.h"

class EntityManager 
{
public:
	//Getter
	const std::vector<std::shared_ptr<Entity>>& GetEntities() const 
	{
		return entities;
	}

	//Create new entity, if name wast provided then use default one
	//Worst case: O(1), but depends on which functions are subscribed to 
	//onEntityCreated signal
	std::weak_ptr<Entity> NewEntity(const std::string& name = "New Entity");

	//Destroy this entity
	//Worst case: O(N) where N is number of entities in game, but depends on 
	//which functions are subscribed to onEntityDestroyed signal
	void DestroyEntity(std::weak_ptr<Entity> entity);

	//Find entity by name
	//Worst case: O(N) where N is number of entities in game
	std::weak_ptr<Entity> FindEntity(const std::string& name) const;

	//Output list of all entities
	//Worst case: O(N) where N is number of entities in game
	void OutputAllEntitiesNames();

private:
	//List of all entities
	std::vector<std::shared_ptr<Entity>> entities;
};