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
	virtual void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) {}

	const std::string& GetSystemName() { return systemName; }

protected:
	std::string systemName;
};

//Deletes all entities at the end of the frame
class DeleteSystem :public System
{
public:
	//Nodes which contain child nodes which should be deleted!
	std::vector<std::weak_ptr<SceneNode>> nodesToDelete;

	virtual ~DeleteSystem() = default;
	//Worst case: O(1)
	void Initialize() override;
	//Worst case: O(N*M) where N is number of nodes in provided node and M number of entities to delete
	void Update(std::shared_ptr<SceneNode> scene, std::shared_ptr<SceneNode> ui, float deltaTime) override;
private:
	//This functions will be called when appropriate signal will be signaled
	//they will ad name or entity to the vector, and then delete them at the end
	//of the frame
	//Worst case: O(1)
	void OnSceneNodeToDelete(std::weak_ptr<SceneNode> wNodeWithChildToDelete);
};

//This namespace will store all signals
namespace signals 
{
	inline sigslot::signal<std::weak_ptr<Entity>> onEntityCreated;
	inline sigslot::signal<std::weak_ptr<Entity>> onEntityDestroyed;
	inline sigslot::signal<std::weak_ptr<SceneNode>> onDeleteSceneNode;
}