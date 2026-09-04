#pragma once
#include "SceneNodeVisitor.h"
#include <memory>
#include "Systems.h"
#include "Components.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "SpaceMapConfiguration.h"



//I assume all visitors exist only a single frame, so this is why it is allowed to have a
// //shared pointers for them
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


//Processes entities with button components
class SceneNodeVisitorButton : public SceneNodeVisitor
{
public:
    SceneNodeVisitorButton(InputSystem& inputSystem, sf::Vector2f mousePos, std::weak_ptr<Entity> wpFrontmost) : inputSystem{ inputSystem }, mousePosition{ mousePos }, wpFrontmostEntity{wpFrontmost} {}

    void ProcessNode(SceneNode& node) override;

private:
    //References to input system
    InputSystem& inputSystem;
    sf::Vector2f mousePosition;
    std::weak_ptr<Entity> wpFrontmostEntity;
};


//Find frontmost UI part
class SceneNodeVisitorFrontmostMouseHit : public SceneNodeVisitor
{
public:
    SceneNodeVisitorFrontmostMouseHit(sf::Vector2f mousePos) : mousePosition{ mousePos } {}

    void ProcessNode(SceneNode& node) override;

    std::weak_ptr<SceneNode> wpFrontmostNode;

private:
    sf::Vector2f mousePosition;
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
    SceneNodeVisitorUI(UISystem& system, std::shared_ptr<CameraComponent> activeCameraCom, std::shared_ptr<CameraComponent> uiCameraCom) : uiSystem(system), spCamCom(activeCameraCom), spUICamCom(uiCameraCom) { processHiddenNode = true; }

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

    int planetsWithDistrictsAmount{ 0 };
    int planetsWithoutDistrictsAmount{ 0 };
    int districtsAmount{ 0 };

    SpaceMapConfigurations& mapConfig;

    sf::Vector2f blackHolePos;

    int ringsAmount{0};
};



class SceneNodeSpaceObjectsMemorySize : public SceneNodeVisitor
{
public:
    SceneNodeSpaceObjectsMemorySize() { processHiddenNode = true; };

    void ProcessNode(SceneNode& node) override;

    void OutputAllData();

private:
    int memoryUsageByEntities{ 0 };
    int memoryUsageByPlanetComponent{ 0 };
    int memoryUsageByStarComponent{ 0 };
    int memoryUsageByObjectSystemComponent{ 0 };
    int memoryUsageByButtonComponent{ 0 };
    int memoryUsageByUIPartComponent{ 0 };
    int memoryUsageByRectangleShapeComponent{ 0 };
    int memoryUsageByRingComponent{ 0 };
    int memoryUsageByDistrictComponent{ 0 };
    int memoryUsageByHabitablePlanetComponent{ 0 };
};



//To change visibility of the system node which accepted this visitor
class SceneNodeVisitorSystemVisibility : public SceneNodeVisitor
{
public:
    SceneNodeVisitorSystemVisibility(std::shared_ptr<CameraComponent> activeCameraCom) : spCamCom(activeCameraCom) { processHiddenNode = true; }

    void ProcessNode(SceneNode& node) override;

private:
    std::shared_ptr<CameraComponent> spCamCom;
};



//To change visibility of the planet node which accepted this visitor
class SceneNodeVisitorChangeSingleSystemVisibility: public SceneNodeVisitor
{
public:
    SceneNodeVisitorChangeSingleSystemVisibility(bool hidden, std::shared_ptr<SceneNode> spSystemIconsNode, std::shared_ptr<SceneNode> spObjectOrbitsNode) : hidden(hidden), spSystemIconsNode(spSystemIconsNode), spObjectOrbitsNode(spObjectOrbitsNode) { processHiddenNode = true; }

    void ProcessNode(SceneNode& node) override;

private:
    bool hidden;
    std::shared_ptr<SceneNode> spSystemIconsNode;
    std::shared_ptr<SceneNode> spObjectOrbitsNode;

    sf::Vector2f planetIconSize{70.f, 70.f};
    sf::Vector2f starIconSize{ 140.f, 140.f };
    sf::Vector2f centerOfMassIconSize{ 90.f, 90.f };
};



class SceneNodeVisitorChangeSinglePlanetVisibility : public SceneNodeVisitor
{
public:
    SceneNodeVisitorChangeSinglePlanetVisibility(bool hidden, std::shared_ptr<SceneNode> spSystemIconsNode, std::shared_ptr<SceneNode> spObjectOrbitsNode, float earthDiameter) : hidden(hidden), spSystemIconsNode(spSystemIconsNode), spObjectOrbitsNode(spObjectOrbitsNode), earthDiameter(earthDiameter) { processHiddenNode = true; }

    void ProcessNode(SceneNode& node) override;

private:
    bool hidden;
    std::shared_ptr<SceneNode> spSystemIconsNode;
    std::shared_ptr<SceneNode> spObjectOrbitsNode;
    std::shared_ptr<SceneNode> spPlanetPicNode;
    float earthDiameter{1.f};

    sf::Vector2f planetIconSize{ 120.f, 120.f };
    sf::Vector2f moonIconSize{ 80.f, 80.f };
};



class SceneNodeVisitorMoveObjectsInSystem : public SceneNodeVisitor
{
public:
    SceneNodeVisitorMoveObjectsInSystem() { processHiddenNode = true; }// : simulateOnlyMoons(simOnlyMoons) {}

    void ProcessNode(SceneNode& node) override;

//private:
//   bool simulateOnlyMoons{ false };
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
    //bool 
};



class VisitorCountPlanets : public SceneNodeVisitor
{
public:
    VisitorCountPlanets(bool countMoons) : countMoons(countMoons) { processHiddenNode = true; }

    void ProcessNode(SceneNode& node) override;

    int counter{0};
private:
    bool countMoons{0};
};



class VisitorCountHabitablePlanets : public SceneNodeVisitor
{
public:
    VisitorCountHabitablePlanets() { processHiddenNode = true; }

    void ProcessNode(SceneNode& node) override;

    int counter{0};
};



class VisitorCountAllNodes : public SceneNodeVisitor
{
public:
    VisitorCountAllNodes() { processHiddenNode = true; }

    void ProcessNode(SceneNode& node) override;

    int counter{ 0 };
};



class ChangeAllNodesVisibilityExceptStarIcons : public SceneNodeVisitor
{
public:
    ChangeAllNodesVisibilityExceptStarIcons(bool hidden) : hidden(hidden) { processHiddenNode = true; }

    void ProcessNode(SceneNode& node) override;

private:
    bool hidden;
};


class VisitorGetClosestButtonAtDirection : public SceneNodeVisitor
{
public:
    VisitorGetClosestButtonAtDirection(int direction, sf::Vector2f curPos) : direction( direction ), currentPosition(curPos) {}

    void ProcessNode(SceneNode& node) override;

    std::weak_ptr<SceneNode> wpClosestButton;
private:
    int direction{ 0 };
    sf::Vector2f currentPosition;
};