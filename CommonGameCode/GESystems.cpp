#include "SceneNode.h"
#include <sigslot/signal.hpp>
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "ECSGame.h"
#include <iostream>

//This function destroys all entities of specific name in this node
//Worst case: O((4*N)*M) where N is number of entities in game and M number of entities
//with the same name
void DeleteAllEntitiesWithName(std::string name)
{
	//Check if entity with this name still exists
	while (ECSGame::Instance().GetEntityManager().FindEntity(name).lock() != nullptr)
	{
		std::weak_ptr wPtr = ECSGame::Instance().GetEntityManager().FindEntity(name);
		//Remove from scene
		ECSGame::Instance().GetSceneRoot()->RemoveByEntity(wPtr);
		//Remove from entity manager
		ECSGame::Instance().GetEntityManager().DestroyEntity(wPtr);
	}
}

//DELETION SYSTEM
//Worst case: O(1)
void DeleteSystem::Initialize()
{
	//Subscribe to some signals
	signals::onDeleteEntity.connect(&DeleteSystem::OnEntityToDelete, this);
	signals::onDeleteEntitiesByName.connect(&DeleteSystem::OnDeleteEntitiesByName, this);

	systemName = "DeleteSystem";
}

//Worst case: O((2*N*M)+(N*K*L)) where N is number of entities in game and M number of entities
//to delete and K number of entities with the same name and L number of different names
//to delete
void DeleteSystem::Update(std::shared_ptr<SceneNode> scene, float deltaTime)
{
	//Check if vector is empty
	while (entitiesToDelete.size()>0)
	{
		//Remove from scene
		ECSGame::Instance().GetSceneRoot()->RemoveByEntity(entitiesToDelete[0]);
		//Remove from entity manager
		ECSGame::Instance().GetEntityManager().DestroyEntity(entitiesToDelete[0]);
		//Remove from this vestor as well
		entitiesToDelete.erase(entitiesToDelete.begin());
	}

	//Check if vector is empty
	while (entitiesByNameToDelete.size() > 0)
	{
		//Remove all entities with this name
		DeleteAllEntitiesWithName(entitiesByNameToDelete[0]);
		//Remove from this vestor as well
		entitiesByNameToDelete.erase(entitiesByNameToDelete.begin());
	}
}

//Add entity to deletion vector
//Worst case: O(1)
void DeleteSystem::OnEntityToDelete(std::weak_ptr<Entity> wEntity)
{
	entitiesToDelete.emplace_back(wEntity);
}

//Add name of entities to deletion
 //Worst case: O(1)
void DeleteSystem::OnDeleteEntitiesByName(std::string name)
{
	entitiesByNameToDelete.emplace_back(name);
}