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
	virtual ComponentType GetComponentType() const = 0;

	virtual ~Component() = default;
};

//Function which creates components
std::shared_ptr<Component> ComponentFactory(ComponentType ct);

//Function which returns component name in string
std::string PrintComponentName(ComponentType ct);