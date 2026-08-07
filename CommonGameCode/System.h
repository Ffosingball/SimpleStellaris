#pragma once

#include "SceneNode.h"
#include <sigslot/signal.hpp>
#include "Entity.h"
#include "Component.h"
#include <memory>

//Template for the system
class System 
{
public:
	//If any of this functions will not be implemented in the child class
	//it won't be used
	virtual ~System() = default;
	//To initialize the system
	virtual void Initialize() {}
	//To end the system
	virtual void Destroy() {}
	//To update entities every frame
	virtual void Update(std::shared_ptr<SceneNode> scene, float deltaTime) {}

	std::string& const GetSystemName() { return systemName; }

protected:
	std::string systemName;
};

//Deletes all entities at the end of the frame
class DeleteSystem :public System
{
public:
	std::vector<std::weak_ptr<Entity>> entitiesToDelete;
	//Nodes which contain child nodes which should be deleted!
	std::vector<std::weak_ptr<SceneNode>> deleteEntityFrom;

	virtual ~DeleteSystem() = default;
	//Worst case: O(1)
	void Initialize() override;
	//Worst case: O(N*M) where N is number of nodes in game and M number of entities to delete
	void Update(std::shared_ptr<SceneNode> scene, float deltaTime) override;
private:
	//This functions will be called when appropriate signal will be signaled
	//they will ad name or entity to the vector, and then delete them at the end
	//of the frame
	//Worst case: O(1)
	void OnEntityToDelete(std::weak_ptr<Entity> wEntity, std::weak_ptr<SceneNode> wNodeWithChildToDelete);
};

namespace signals 
{
	inline sigslot::signal<std::weak_ptr<Entity>> onEntityCreated;
	inline sigslot::signal<std::weak_ptr<Entity>> onEntityDestroyed;
	inline sigslot::signal<const Entity&, ComponentType> onComponentAdded;
	inline sigslot::signal<const Entity&, ComponentType> onComponentRemove;
	inline sigslot::signal<std::weak_ptr<Entity>, std::weak_ptr<SceneNode>> onDeleteEntity;
}