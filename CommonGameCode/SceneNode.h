#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <memory>
#include "SceneNodeVisitor.h"

//Scene node which will be used to create a scene graph
class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
	//Scene node can be constructed with or without entity
	SceneNode() = default;
	SceneNode(const std::weak_ptr<Entity>& entity) :entity{ entity } {}

	//Getters
	sf::Transform GetCombinedTransform() const;
	std::string GetCombinedParentsNames() const;
	std::string GetAllChildrenNames() const;
	std::weak_ptr<Entity> GetEntity() const { return entity; }
	const std::weak_ptr<SceneNode> GetParent() const { return parent; }
	std::vector<std::shared_ptr<SceneNode>> GetAllChildren() const { return children; };
	std::shared_ptr<SceneNode> GetSharedPtrToItself() { return shared_from_this(); }

	//Add child to this node
	void AddChild(const std::shared_ptr<SceneNode> child);

	//Change child position, child should already exist here
	void ChangeChildOrder(const std::shared_ptr<SceneNode> child, int position);
	//Change child position by entity, child should already exist here
	void ChangeChildOrder(const std::shared_ptr<Entity> entity, int position);

	void AcceptVisitor(SceneNodeVisitor& visitor);

	//Remove child
	void RemoveByEntity(std::weak_ptr<Entity> e);

	//Find and return child
	std::weak_ptr<SceneNode> FindChild(const Entity& e);
	std::weak_ptr<SceneNode> FindChild(const std::string& s);

private:

	//Upadate parent pointer, of all children
	//void UpdateParentRecursive();

	//Pointer to the entity
	std::weak_ptr<Entity> entity;
	//Pointer to the parent
	std::weak_ptr<SceneNode> parent;
	//List of children
	std::vector<std::shared_ptr<SceneNode>> children;
};