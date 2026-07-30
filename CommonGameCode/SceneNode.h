#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <memory>
#include "SceneNodeVisitor.h"

//Scene node which will be used to create a scene graph
class SceneNode 
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
	const SceneNode* GetParent() const { return parent; }

	//Add child to this node
	void AddChild(const SceneNode& child);

	void AcceptVisitor(SceneNodeVisitor& visitor);

	//Remove child
	void RemoveByEntity(std::weak_ptr<Entity> e);

	//Find and return child
	SceneNode* FindChild(const Entity& e);

private:

	//Upadate parent pointer, of all children
	void UpdateParentRecursive();

	//Pointer to the entity
	std::weak_ptr<Entity> entity;
	//Pointer to the parent
	const SceneNode* parent = nullptr;
	//List of children
	std::vector<SceneNode> children;
};