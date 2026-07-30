#pragma once
#include <SFML/Graphics.hpp>

class SceneNode;

//Template for the scene graph node visitor
class SceneNodeVisitor 
{
public:
	//This function will get node and process it
	virtual void ProcessNode(SceneNode& node) = 0;
};