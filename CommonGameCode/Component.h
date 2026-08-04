#pragma once
#include <memory>
#include <string>

enum class ComponentType;

//Template for the component
class Component 
{
public:
	//All component fields will be public
	//Returns type of the component
	//Worst case: O(1)
	virtual ComponentType GetComponentType() const = 0;

	virtual ~Component() = default;
};

//Function which creates components
//Worst case: O(N) where N is number of components available in game
std::shared_ptr<Component> ComponentFactory(ComponentType ct);

//Function which returns component name in string
//Worst case: O(N) where N is number of components available in game
std::string PrintComponentName(ComponentType ct);