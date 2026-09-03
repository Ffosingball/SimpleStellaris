#pragma once
#include <SFML/Graphics.hpp>

class SceneNode;

//Template for the scene graph node visitor
class SceneNodeVisitor 
{
public:
	//This function will get node and process it
	virtual void ProcessNode(SceneNode& node) = 0;

	bool GetProcessHiddenNode() const { return processHiddenNode; }

protected:
	//This variable tells whether hidden node should be processed or not
	bool processHiddenNode{ false };
};