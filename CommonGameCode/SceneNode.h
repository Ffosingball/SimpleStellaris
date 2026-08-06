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
	//Worst case: O(N) where N is number of parents to get to the rootNode
	sf::Transform GetCombinedTransform() const;
	//Worst case: O(N) where N is number of parents to get to the rootNode
	sf::Vector2f GetCombinedPosition() const;
	//Worst case: O(N) where N is number of parents to get to the rootNode
	std::string GetCombinedParentsNames() const;
	//Worst case: O(N) where N is number of children
	std::string GetAllChildrenNames() const;
	//Worst case: O(1)
	std::weak_ptr<Entity> GetEntity() const { return entity; }
	//Worst case: O(1)
	const std::weak_ptr<SceneNode> GetParent() const { return parent; }
	//Worst case: O(1)
	std::vector<std::shared_ptr<SceneNode>> GetAllChildren() const { return children; };
	//Worst case: O(1)
	std::shared_ptr<SceneNode> GetSharedPtrToItself() { return shared_from_this(); }

	//Add child to this node
	//Worst case: O(1)
	void AddChild(const std::shared_ptr<SceneNode> child);

	//Change child position, child should already exist here
	//Worst case: O(N) where N is number of children
	void ChangeChildOrder(const std::shared_ptr<SceneNode> child, int position);
	//Change child position by entity, child should already exist here
	//Worst case: O(N) where N is number of children
	void ChangeChildOrder(const std::shared_ptr<Entity> entity, int position);

	void AcceptVisitor(SceneNodeVisitor& visitor);

	//Remove child
	//Worst case: O(N) where N is number nodes in the game
	void RemoveByEntity(std::weak_ptr<Entity> e);

	//Find and return child
	//Worst case: O(N) where N is number nodes in the game
	std::weak_ptr<SceneNode> FindChild(const Entity& e);
	//Worst case: O(N) where N is number nodes in the game
	std::weak_ptr<SceneNode> FindChild(const std::string& s);
	//Worst case: O(N) where N is number of nodes in game
	void OutputTree(const std::string& s);

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