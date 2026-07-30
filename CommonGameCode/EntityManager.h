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
	std::weak_ptr<Entity> NewEntity(const std::string& name = "New Entity");

	//Destroy this entity
	void DestroyEntity(std::weak_ptr<Entity> entity);

	//Find entity by name
	std::weak_ptr<Entity> FindEntity(const std::string& name) const;

private:
	//List of all entities
	std::vector<std::shared_ptr<Entity>> entities;
};