#pragma once
#include <memory>
#include <string>

//Template for the component
class Component 
{
public:
	//All component fields will be public

	virtual ~Component() = default;
};

//Function which returns component name in string
//Worst case: O(N) where N is number of components available in game
std::string PrintComponentName(std::shared_ptr<Component> spComponent);