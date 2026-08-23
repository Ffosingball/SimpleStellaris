#include "SceneNode.h"
#include <sigslot/signal.hpp>
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "ECSGame.h"
#include <iostream>


//DELETION SYSTEM
//Worst case: O(1)
void DeleteSystem::Initialize()
{
	//Subscribe to some signals
	signals::onDeleteEntity.connect(&DeleteSystem::OnEntityToDelete, this);

	systemName = "DeleteSystem";
}

//Worst case: O(N*M) where N is number of nodes in game and M number of entities to delete
void DeleteSystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	//Check if vector is empty
	while (entitiesToDelete.size()>0)
	{
		//For each entity in the list get it and node in which it exist
		std::weak_ptr<Entity> entityToDelete = entitiesToDelete.back();
		entitiesToDelete.pop_back();
		std::weak_ptr<SceneNode> nodeToDeleteFrom = deleteEntityFrom.back();
		deleteEntityFrom.pop_back();

		if (entityToDelete.lock() != nullptr)
		{
			//Remove from scene
			nodeToDeleteFrom.lock()->RemoveByEntity(entityToDelete);
			//Remove from entity manager
			ECSGame::Instance().GetEntityManager().DestroyEntity(entityToDelete);
		}
	}
}

//Add entity to deletion vector
//Worst case: O(1)
void DeleteSystem::OnEntityToDelete(std::weak_ptr<Entity> wEntity, std::weak_ptr<SceneNode> wNodeWithChildToDelete)
{
	entitiesToDelete.emplace_back(wEntity);
	deleteEntityFrom.emplace_back(wNodeWithChildToDelete);
}