#pragma once
#include "SceneNodeVisitor.h"
#include <memory>
#include "Systems.h"
#include "Components.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "SpaceMapConfiguration.h"



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
//I assume that entity has only one component for rendering!
class SceneNodeVisitorRender : public SceneNodeVisitor
{
public:
    SceneNodeVisitorRender(sf::RenderWindow& zRenderWindow) :renderWindow(zRenderWindow) {}

    void ProcessNode(SceneNode& node) override;

    void OutputRenderStatistics();

    int renderedEntities{ 0 };
    int didNotRenderedEntities{ 0 };

private:
    sf::RenderWindow& renderWindow;
};



//To process entities with UI components
class SceneNodeVisitorUI : public SceneNodeVisitor
{
public:
    SceneNodeVisitorUI(UISystem& system, std::shared_ptr<CameraComponent> activeCameraCom, std::shared_ptr<CameraComponent> uiCameraCom) : uiSystem(system), spCamCom(activeCameraCom), spUICamCom(uiCameraCom) {}

    void ProcessNode(SceneNode& node) override;
private:
    UISystem& uiSystem;
    std::shared_ptr<CameraComponent> spCamCom;
    std::shared_ptr<CameraComponent> spUICamCom;
};



//To render entities with UI component
class SceneNodeVisitorRenderUI : public SceneNodeVisitor
{
public:
    SceneNodeVisitorRenderUI(sf::RenderWindow& zRenderWindow) :renderWindow(zRenderWindow) {}

    void ProcessNode(SceneNode& node) override;

    void OutputRenderStatistics();

    int renderedEntities{ 0 };
    int didNotRenderedEntities{ 0 };

private:
    sf::RenderWindow& renderWindow;
};



//To count and print all space objects
class SceneNodeSpaceObjectsCounter : public SceneNodeVisitor
{
public:
    SceneNodeSpaceObjectsCounter(SpaceMapConfigurations& mapConfig);

    void ProcessNode(SceneNode& node) override;

    void OutputAllData();

private:
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

    std::vector<int> planetTypesAmount;
    std::vector<int> moonTypesAmount;
    std::vector<int> numberOfMoonsAmount;

    SpaceMapConfigurations& mapConfig;

    sf::Vector2f blackHolePos;
};



class SceneNodeVisitorSystemVisibility : public SceneNodeVisitor
{
public:
    SceneNodeVisitorSystemVisibility(std::shared_ptr<CameraComponent> activeCameraCom) : spCamCom(activeCameraCom) {}

    void ProcessNode(SceneNode& node) override;

private:
    std::shared_ptr<CameraComponent> spCamCom;
};



class SceneNodeVisitorChangeAllSystemVisibility : public SceneNodeVisitor
{
public:
    SceneNodeVisitorChangeAllSystemVisibility(bool hidden) : hidden(hidden) {}

    void ProcessNode(SceneNode& node) override;

private:
    bool hidden;
};



class SceneNodeVisitorChangeSingleSystemVisibility: public SceneNodeVisitor
{
public:
    SceneNodeVisitorChangeSingleSystemVisibility(bool hidden, std::shared_ptr<SceneNode> spSystemIconsNode, std::shared_ptr<SceneNode> spObjectOrbitsNode) : hidden(hidden), spSystemIconsNode(spSystemIconsNode), spObjectOrbitsNode(spObjectOrbitsNode) {}

    void ProcessNode(SceneNode& node) override;

private:
    bool hidden;
    std::shared_ptr<SceneNode> spSystemIconsNode;
    std::shared_ptr<SceneNode> spObjectOrbitsNode;

    sf::Vector2f planetIconSize{50.f, 50.f};
    sf::Vector2f starIconSize{ 140.f, 140.f };
    sf::Vector2f centerOfMassIconSize{ 90.f, 90.f };
};



class SceneNodeVisitorChangeSinglePlanetVisibility : public SceneNodeVisitor
{
public:
    SceneNodeVisitorChangeSinglePlanetVisibility(bool hidden, std::shared_ptr<SceneNode> spSystemIconsNode, std::shared_ptr<SceneNode> spObjectOrbitsNode, float earthDiameter) : hidden(hidden), spSystemIconsNode(spSystemIconsNode), spObjectOrbitsNode(spObjectOrbitsNode), earthDiameter(earthDiameter) {}

    void ProcessNode(SceneNode& node) override;

private:
    bool hidden;
    std::shared_ptr<SceneNode> spSystemIconsNode;
    std::shared_ptr<SceneNode> spObjectOrbitsNode;
    std::shared_ptr<SceneNode> spPlanetPicNode;
    float earthDiameter{1.f};

    sf::Vector2f planetIconSize{ 70.f, 70.f };
    sf::Vector2f moonIconSize{ 50.f, 50.f };
};



class SceneNodeVisitorMoveObjectsInSystem : public SceneNodeVisitor
{
public:
    SceneNodeVisitorMoveObjectsInSystem(bool simOnlyMoons) : simulateOnlyMoons(simOnlyMoons) {}

    void ProcessNode(SceneNode& node) override;

private:
    bool simulateOnlyMoons{ false };
};



class SceneNodeVisitorChangeNebulasVisibility : public SceneNodeVisitor
{
public:
    SceneNodeVisitorChangeNebulasVisibility(bool hidden) : hidden(hidden) {}

    void ProcessNode(SceneNode& node) override;
private:
    bool hidden;
};



class SceneNodeVisitorGetClosestNodeToPosition : public SceneNodeVisitor
{
public:
    SceneNodeVisitorGetClosestNodeToPosition(sf::Vector2f position, float maxDistance, bool checkPlanets) : position(position), maxDistance(maxDistance), checkPlanets(checkPlanets) {}

    void ProcessNode(SceneNode& node) override;

    std::weak_ptr<SceneNode> wpClosestNode;
    OverviewType currentOverview=OverviewType::System;

private:
    sf::Vector2f closestPosition{9999999.f, 9999999.f};

    sf::Vector2f position;
    float maxDistance;
    bool checkPlanets;
};