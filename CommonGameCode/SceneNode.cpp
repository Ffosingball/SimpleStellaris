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
	if (parent.lock() != nullptr)
		return parent.lock()->GetCombinedTransform() * ePtr->GetTransformable().getTransform();
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
	if (parent.lock() != nullptr)
		return parent.lock()->GetCombinedParentsNames() + " -> " + ePtr->GetName();
	else
		return ePtr->GetName();
}

//Get list of all children names
std::string SceneNode::GetAllChildrenNames() const
{
	std::string s{""};
	for (auto& ch : children)
	{
		s = s + "; " + ch->GetEntity().lock()->GetName();
	}

	return s;
}

//Add child to the list
void SceneNode::AddChild(const std::shared_ptr<SceneNode> child)
{
	children.emplace_back(child);
	child->parent = weak_from_this();
}

/*void SceneNode::UpdateParentRecursive()
{
	//Update pointer for each child, and call it for each child
	for (auto& ch : children)
	{
		ch.parent = this;
		ch.UpdateParentRecursive();
	}
}*/

void SceneNode::AcceptVisitor(SceneNodeVisitor& visitor) 
{
	//Allow visitor to process this node
	visitor.ProcessNode(*this);

	//Send visitors to all children
	for (std::shared_ptr<SceneNode> node : children)
		node->AcceptVisitor(visitor);
}

//Remove child
void SceneNode::RemoveByEntity(std::weak_ptr<Entity> e) 
{
	auto pEntity = e.lock().get();
	int iFound = -1;
	//Find the child
	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i]->GetEntity().lock().get() == pEntity)
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
		//UpdateParentRecursive();
	}
	else
	{
		//Else trye to find and remove this entity in the children
		for (auto& child : children)
			child->RemoveByEntity(e);
	}
}

//This function finds and returns scene node which contains this entity
std::weak_ptr<SceneNode> SceneNode::FindChild(const Entity& e)
{
	//Check if this node is a target entity
	if (&e == entity.lock().get())
		return weak_from_this();
	else
	{
		//Else, try to check all children
		for (auto& c : children)
		{
			auto ret = c->FindChild(e);
			//If it was found in the child, then return it
			if (ret.lock() != nullptr)
				return ret;
		}
	}
	//Else return null
	return {};
}

//This function finds and returns scene node which contains entity with provided name
std::weak_ptr<SceneNode> SceneNode::FindChild(const std::string& s)
{
	//Check if this node has a entity
	if (entity.lock() != nullptr)
	{
		//std::cout << "Find: " << s << "; Parent: " << parent << "; Check: "<<this<<"; " << entity.lock()->GetName() << "\n";
		//Check if this node is a target entity
		if (s == entity.lock()->GetName())
			return weak_from_this();
	}
	//else
	//	std::cout << "Find: " << s << "; Entity does not exist in this node: " << this << '\n';

	//Else, try to check all children
	for (auto& c : children)
	{
		auto ret = c->FindChild(s);
		//If it was found in the child, then return it
		if (ret.lock() != nullptr)
			return ret;
	}
	//Else return null
	return {};
}


void SceneNode::ChangeChildOrder(const std::shared_ptr<SceneNode> child, int position)
{
	if (position >= children.size())
		std::cout << "Position outside of vector range!\n";

	std::vector<std::shared_ptr<SceneNode>> newOrder(children.size());

	int newOrderPos = 0;
	int oldOrderPos = 0;
	while (newOrderPos < position) 
	{
		if (children[oldOrderPos] != child)
		{
			newOrder[newOrderPos] = children[oldOrderPos];
			newOrderPos++;
			oldOrderPos++;
		}
		else
			oldOrderPos++;
	}

	newOrder[newOrderPos] = child;
	newOrderPos++;

	while (newOrderPos < children.size())
	{
		if (children[oldOrderPos] != child)
		{
			newOrder[newOrderPos] = children[oldOrderPos];
			newOrderPos++;
			oldOrderPos++;
		}
		else
			oldOrderPos++;
	}

	children = newOrder;
}


void SceneNode::ChangeChildOrder(const std::shared_ptr<Entity> entity, int position)
{
	if (position >= children.size())
		std::cout << "Position outside of vector range!\n";

	std::vector<std::shared_ptr<SceneNode>> newOrder(children.size());

	int newOrderPos = 0;
	int oldOrderPos = 0;
	int childPos{-1};
	while (newOrderPos < position)
	{
		if (children[oldOrderPos]->GetEntity().lock() != entity)
		{
			newOrder[newOrderPos] = children[oldOrderPos];
			newOrderPos++;
			oldOrderPos++;
		}
		else
		{
			childPos = oldOrderPos;
			oldOrderPos++;
		}
	}

	newOrderPos++;

	while (newOrderPos < children.size())
	{
		if (children[oldOrderPos]->GetEntity().lock() != entity)
		{
			newOrder[newOrderPos] = children[oldOrderPos];
			newOrderPos++;
			oldOrderPos++;
		}
		else
		{
			childPos = oldOrderPos;
			oldOrderPos++;
		}
	}

	if (childPos == -1)
		childPos = children.size() - 1;

	newOrder[position] = children[childPos];
	children = newOrder;
}