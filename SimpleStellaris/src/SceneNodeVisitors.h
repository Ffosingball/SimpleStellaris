#pragma once
#include "SceneNodeVisitor.h"
#include <memory>
#include "Systems.h"
#include "Components.h"
#include <string>
#include <SFML/Graphics.hpp>

//Processes entities with movement components
class SceneNodeVisitorMovement : public SceneNodeVisitor
{
public:
    SceneNodeVisitorMovement(MovementSystem& mSystem) : movementSystem{ mSystem } {}

    void ProcessNode(SceneNode& node) override;

private:
    //References to movement system
    MovementSystem& movementSystem;
};

//To render entities with rectangleShape, tileMap and particles components
class SceneNodeVisitorRender : public SceneNodeVisitor
{
public:
    SceneNodeVisitorRender(sf::RenderWindow& zRenderWindow) :renderWindow(zRenderWindow) {}

    void ProcessNode(SceneNode& node) override;

private:
    sf::RenderWindow& renderWindow;
};

//To process entities with UI components
class SceneNodeVisitorUI : public SceneNodeVisitor
{
public:
    SceneNodeVisitorUI(UISystem& system) : uiSystem(system) {}

    void ProcessNode(SceneNode& node) override;
private:
    UISystem& uiSystem;
};

//To render entities with UI component
class SceneNodeVisitorRenderUI : public SceneNodeVisitor
{
public:
    SceneNodeVisitorRenderUI(sf::RenderWindow& zRenderWindow) :renderWindow(zRenderWindow) {}

    void ProcessNode(SceneNode& node) override;

private:
    sf::RenderWindow& renderWindow;
};