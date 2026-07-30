#include "SceneNode.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "SceneNodeVisitor.h"
#include <iostream>

//Get sum of all transformations from the root to this node
sf::Transform SceneNode::GetCombinedTransform() const 
{
	std::shared_ptr<Entity> ePtr = entity.lock();

	//If node does not have an entity then, return nothing
	if (ePtr == nullptr)
		return {};

	//Check if this node has a parent, then get transform from them and add to
	//the transform of this node, else just return transform of this node
	if (parent != nullptr)
		return parent->GetCombinedTransform() * ePtr->GetTransformable().getTransform();
	else
		return ePtr->GetTransformable().getTransform();
}

//Get full location of the node in the tree
std::string SceneNode::GetCombinedParentsNames() const
{
	std::shared_ptr<Entity> ePtr = entity.lock();

	//If node does not have an entity then, return nothing
	if (ePtr == nullptr)
		return "--None--";

	//Check if this node has a parent, then get transform from them and add to
	//the transform of this node, else just return transform of this node
	if (parent != nullptr)
		return parent->GetCombinedParentsNames()+" -> "+ePtr->GetName();
	else
		return ePtr->GetName();
}

//Get list of all children names
std::string SceneNode::GetAllChildrenNames() const
{
	std::string s{""};
	for (auto& ch : children)
	{
		s = s + "; " + ch.GetEntity().lock()->GetName();
	}

	return s;
}

//Add child to the list
void SceneNode::AddChild(const SceneNode& child) 
{
	children.emplace_back(child);
	//Vector has been changed, so we HAVE TO update POINTERS!!!
	UpdateParentRecursive();
}

void SceneNode::UpdateParentRecursive() 
{
	//Update pointer for each child, and call it for each child
	for (auto& ch : children)
	{
		ch.parent = this;
		ch.UpdateParentRecursive();
	}
}

void SceneNode::AcceptVisitor(SceneNodeVisitor& visitor) 
{
	//Allow visitor to process this node
	visitor.ProcessNode(*this);

	//Send visitors to all children
	for (SceneNode node : children)
		node.AcceptVisitor(visitor);
}

//Remove child
void SceneNode::RemoveByEntity(std::weak_ptr<Entity> e) 
{
	auto pEntity = e.lock().get();
	int iFound = -1;
	//Find the child
	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i].GetEntity().lock().get() == pEntity)
		{
			//If found, then update iFound
			iFound = i;
			break;
		}
	}
	if (iFound >= 0)
	{
		//If child is found, then remove it from the list
		children.erase(children.begin() + iFound);
		//Vector has been changed, so we HAVE TO update POINTERS!!!
		UpdateParentRecursive();
	}
	else
	{
		//Else trye to find and remove this entity in the children
		for (auto& child : children)
			child.RemoveByEntity(e);
	}
}

//This function finds and returns scene node which contains at this entity
SceneNode* SceneNode::FindChild(const Entity& e) 
{
	//Check if this node is a target entity
	if (&e == entity.lock().get())
		return this;
	else
	{
		//Else, try to check all children
		for (auto& c : children)
		{
			auto ret = c.FindChild(e);
			//If it was found in the child, then return it
			if (ret != nullptr)
				return ret;
		}
	}
	//Else return null
	return nullptr;
}