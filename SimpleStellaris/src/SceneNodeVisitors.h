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


//To count and print all space objects
class SceneNodeSpaceObjectsCounter : public SceneNodeVisitor
{
public:
    SceneNodeSpaceObjectsCounter() {}

    void ProcessNode(SceneNode& node) override;

    int redSupGiantAmount{0};
    int redGiantAmount{ 0 };
    int OclassAmount{ 0 };
    int BclassAmount{ 0 };
    int AclassAmount{ 0 };
    int FclassAmount{ 0 };
    int GclassAmount{ 0 };
    int KclassAmount{ 0 };
    int MclassAmount{ 0 };
    int brownDwarfAmount{ 0 };
    int whiteDwarfAmount{ 0 };
    int neutronAmount{ 0 };
    int blackHoleAmount{ 0 };

    int singleSysAmount{ 0 };
    int binaryCloseSysAmount{ 0 };
    int binaryAfarSysAmount{ 0 };
    int ternaryCloseSysAmount{ 0 };
    int ternaryTwoCloseOneAfarSysAmount{ 0 };
    int ternaryAfarSysAmount{ 0 };
};