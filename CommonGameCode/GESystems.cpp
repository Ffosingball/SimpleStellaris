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
	signals::onDeleteSceneNode.connect(&DeleteSystem::OnSceneNodeToDelete, this);

	systemName = "DeleteSystem";
}

//Worst case: O(N*M) where N is number of nodes in game and M number of entities to delete
void DeleteSystem::Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime)
{
	//Check if vector is empty
	while (nodesToDelete.size()>0)
	{
		//For each entity in the list get it and node in which it exist
		std::weak_ptr<SceneNode> nodeToDelete = nodesToDelete.back();
		nodesToDelete.pop_back();

		if (nodeToDelete.lock() != nullptr)
		{
			//Delete all entities in the child node, sceneNode themselves will be deleted automatically
			nodeToDelete.lock()->DeleteAllEntities();
			//Remove from scene
			if(nodeToDelete.lock()->GetParent().lock()!=nullptr)
				nodeToDelete.lock()->GetParent().lock()->RemoveNode(nodeToDelete);
		}
	}
}

//Add entity to deletion vector
//Worst case: O(1)
void DeleteSystem::OnSceneNodeToDelete(std::weak_ptr<SceneNode> wNodeWithChildToDelete)
{
	nodesToDelete.emplace_back(wNodeWithChildToDelete);
}