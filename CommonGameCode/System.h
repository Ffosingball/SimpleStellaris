#pragma once

#include "SceneNode.h"
#include <sigslot/signal.hpp>
#include "Entity.h"
#include "Component.h"

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
	virtual void Update(SceneNode& scene, float deltaTime) {}
};

//Deletes all entities at the end of the frame
class DeleteSystem :public System
{
public:
	std::vector<std::weak_ptr<Entity>> entitiesToDelete;
	std::vector<std::string> entitiesByNameToDelete;

	virtual ~DeleteSystem() = default;
	void Initialize() override;
	void Update(SceneNode& scene, float deltaTime) override;
private:
	//This functions will be called when appropriate signal will be signaled
	//they will ad name or entity to the vector, and then delete them at the end
	//of the frame
	void OnEntityToDelete(std::weak_ptr<Entity> wEntity);
	void OnDeleteEntitiesByName(std::string name);
};

namespace signals 
{
	inline sigslot::signal<std::weak_ptr<Entity>> onEntityCreated;
	inline sigslot::signal<std::weak_ptr<Entity>> onEntityDestroyed;
	inline sigslot::signal<const Entity&, ComponentType> onComponentAdded;
	inline sigslot::signal<const Entity&, ComponentType> onComponentRemove;
	inline sigslot::signal<std::weak_ptr<Entity>> onDeleteEntity;
	inline sigslot::signal<std::string> onDeleteEntitiesByName;
}