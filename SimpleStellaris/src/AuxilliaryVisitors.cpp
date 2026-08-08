#include "SceneNodeVisitors.h"
#include "ECSGame.h"
#include "Components.h"
#include "CommonGameCode.h"
#include "EntitiesFunctions.h"
#include <random>
#include <iostream>
#include <SFML/Audio.hpp>
#include "SpaceObjectTypes.h"
#include "WorldGenerator.h"

//Auxiliary visitors, are visitors which are not called by any system
//They are called by other visitors


void SceneNodeSpaceObjectsCounter::ProcessNode(SceneNode& node) 
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Check if pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has system component
        if (spEntity->HasComponent(ComponentType::ObjectSystem))
        {
            std::shared_ptr<ObjectSystemComponent> spComSys = GetObjectSystemComponent(*spEntity);

            switch (spComSys->systemType) 
            {
            case SpaceSystemType::Single:
                singleSysAmount++;
                break;
            case SpaceSystemType::BinaryClose:
                binaryCloseSysAmount++;
                break;
            case SpaceSystemType::BinaryAfar:
                binaryAfarSysAmount++;
                break;
            case SpaceSystemType::TernaryTwoCloseThirdAfar:
                ternaryTwoCloseOneAfarSysAmount++;
                break;
            case SpaceSystemType::TernaryAfar:
                ternaryAfarSysAmount++;
                break;
            }

            std::cout << "-- System (" << spEntity->GetName() << "): " << '\n';
        }
        else if (spEntity->HasComponent(ComponentType::Star))
        {
            std::shared_ptr<StarComponent> spComStar = GetStarComponent(*spEntity);

            switch (spComStar->starType)
            {
            case StarType::RedGiant:
                redGiantAmount++;
                std::cout << "   Red Giant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::RedSupergiant:
                redSupGiantAmount++;
                std::cout << "   Red Supergiant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Otype:
                OclassAmount++;
                std::cout << "   Blue Supergiant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Btype:
                BclassAmount++;
                std::cout << "   Blue Giant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Atype:
                AclassAmount++;
                std::cout << "   Bluish-White Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Ftype:
                FclassAmount++;
                std::cout << "   White Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::GsunLike:
                GclassAmount++;
                std::cout << "   Yellow Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::KorangeDwarf:
                KclassAmount++;
                std::cout << "   Orange Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::MredDwarf:
                MclassAmount++;
                std::cout << "   Red Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::BrownDwarf:
                brownDwarfAmount++;
                std::cout << "   Brawn Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::WhiteDwarf:
                whiteDwarfAmount++;
                std::cout << "   White Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::NeutronStar:
                neutronAmount++;
                std::cout << "   Neutron Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::BlackHole:
                blackHoleAmount++;
                std::cout << "   Black Hole (" << spEntity->GetName() << "): " << '\n';
                break;
            }
        }
    }
}



//System visibility processing function
void SceneNodeVisitorChangeAllSystemVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has object system component
        if (spEntity->HasComponent(ComponentType::ObjectSystem) && spEntity->GetName()!="InsideSystem")
        {
            spEntity->hidden = hidden;
        }
    }
}



//System visibility processing function
//I assume that the position of nodes in the system will not be changed!
void SceneNodeVisitorChangeSingleSystemVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    counter++;

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        if (spEntity->HasComponent(ComponentType::ObjectSystem) && spEntity->GetName() == "InsideSystem")
        {
            spEntity->hidden = hidden;
            if (!hidden)
            {
                //Now set center of mass icon
                std::weak_ptr<Entity> wpCoM = ECSGame::Instance().GetEntityManager().NewEntity("CenterOfMass");
                node.AddChild(std::make_shared<SceneNode>(wpCoM));
                std::shared_ptr<StarComponent> spStar = GetStarComponent(*spEntity);
                wpCoM.lock()->hidden = false;
                CreateIconForSystemOverview(node.FindChild(*wpCoM.lock()).lock(), spSystemIconsNode, "CenterOfMassIcon", "ObjectIcon" + std::to_string(counter), false, centerOfMassIconSize);
                CreateOrbitFor(spObjectOrbitsNode, "Orbit" + std::to_string(counter), spEntity->inheritParentPosition, spStar->orbitRadius, node.GetParent().lock()->FindChild("CenterOfMass"), 5.f, sf::Color(100, 100, 100, 255), false);
            }
            else
            {
                std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("ObjectIcon" + std::to_string(counter)).lock()->GetEntity();
                signals::onDeleteEntity(wpE, spSystemIconsNode);
                std::weak_ptr<Entity> wpE2 = node.FindChild("CenterOfMass").lock()->GetEntity();
                signals::onDeleteEntity(wpE2, node.GetSharedPtrToItself());
                std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + std::to_string(counter)).lock()->GetEntity();
                signals::onDeleteEntity(wpE3, spSystemIconsNode);
            }
        }
        else if (spEntity->HasComponent(ComponentType::ObjectSystem))
        {
            if (!hidden)
            {
                if (GetObjectSystemComponent(*spEntity)->systemType != SpaceSystemType::Single)
                {
                    //Now set center of mass icon
                    std::weak_ptr<Entity> wpCoM = ECSGame::Instance().GetEntityManager().NewEntity("CenterOfMass");
                    node.AddChild(std::make_shared<SceneNode>(wpCoM));
                    wpCoM.lock()->inheritParentPosition = false;
                    wpCoM.lock()->hidden = false;
                    CreateIconForSystemOverview(node.FindChild(*wpCoM.lock()).lock(), spSystemIconsNode, "CenterOfMassIcon", "ObjectIcon" + std::to_string(counter), false, centerOfMassIconSize);
                }
            }
            else 
            {
                if (GetObjectSystemComponent(*spEntity)->systemType != SpaceSystemType::Single)
                {
                    std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("ObjectIcon" + std::to_string(counter)).lock()->GetEntity();
                    signals::onDeleteEntity(wpE, spSystemIconsNode);
                    std::weak_ptr<Entity> wpE2 = node.FindChild("CenterOfMass").lock()->GetEntity();
                    signals::onDeleteEntity(wpE2, node.GetSharedPtrToItself());
                }
            }
        }
        else if (spEntity->HasComponent(ComponentType::Star))
        {
            spEntity->hidden = hidden;
            if (!hidden) 
            {
                std::shared_ptr<StarComponent> spStar = GetStarComponent(*spEntity);
                //Create text name entity for star
                std::string name{ "StarNameText" + std::to_string(counter) };
                spStar->wpStarNameText = CreateSystemText(spSystemIconsNode, node.GetSharedPtrToItself(), name, false);
                //spStar->wpStarNameText.lock()->hidden = true;
                //spEntity->hidden = false;
                //Create icon
                CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, GetSystemTextureName(spStar->starType), "ObjectIcon" + std::to_string(counter), false, starIconSize);
                //Create orbit
                if(GetObjectSystemComponent(*node.GetParent().lock()->GetEntity().lock())->systemType!=SpaceSystemType::Single)
                    CreateOrbitFor(spObjectOrbitsNode, "Orbit" + std::to_string(counter), spEntity->inheritParentPosition, spStar->orbitRadius, node.GetParent().lock()->FindChild("CenterOfMass"), 5.f, sf::Color(100, 100, 100, 255), false);
            }
            else 
            {
                std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("StarNameText" + std::to_string(counter)).lock()->GetEntity();
                signals::onDeleteEntity(wpE, spSystemIconsNode);
                std::weak_ptr<Entity> wpE2 = spSystemIconsNode->FindChild("ObjectIcon" + std::to_string(counter)).lock()->GetEntity();
                signals::onDeleteEntity(wpE2, spSystemIconsNode);
                if (GetObjectSystemComponent(*node.GetParent().lock()->GetEntity().lock())->systemType != SpaceSystemType::Single)
                {
                    std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + std::to_string(counter)).lock()->GetEntity();
                    signals::onDeleteEntity(wpE3, spSystemIconsNode);
                }

            }
        }
        else if (spEntity->HasComponent(ComponentType::Planet))
        {
            spEntity->hidden = hidden;
            if (!hidden)
            {
                std::shared_ptr<PlanetComponent> spPlanet = GetPlanetComponent(*spEntity);
                //std::cout << spPlanet->planetName << ": " << spEntity->inheritParentPosition << '\n';
                //Create text name entity for star
                std::string name{ "PlanetNameText" + std::to_string(counter) };
                CreateSystemText(spSystemIconsNode, node.GetSharedPtrToItself(), name, true);
                //spStar->wpStarNameText.lock()->hidden = true;
                //spEntity->hidden = false;
                //Create icon
                CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, spPlanet->planetIconTextureName, "PlanetIcon" + std::to_string(counter), true, planetIconSize);
                //Get parent for orbits
                std::weak_ptr<SceneNode> wpFollowNode;
                if (node.GetParent().lock()->GetEntity().lock()->HasComponent(ComponentType::ObjectSystem))
                {
                    if (GetObjectSystemComponent(*node.GetParent().lock()->GetEntity().lock())->systemType == SpaceSystemType::BinaryClose)
                        wpFollowNode = node.GetParent().lock()->FindChild("CenterOfMass");
                    else
                        wpFollowNode = node.GetParent();
                }
                else
                    wpFollowNode = node.GetParent();
                //Create orbit
                CreateOrbitFor(spObjectOrbitsNode, "Orbit" + std::to_string(counter), spEntity->inheritParentPosition, spPlanet->orbitRadius, wpFollowNode, 2.f, sf::Color(200,200,200,255), true);
            }
            else
            {
                std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("PlanetNameText" + std::to_string(counter)).lock()->GetEntity();
                signals::onDeleteEntity(wpE, spSystemIconsNode);
                std::weak_ptr<Entity> wpE2 = spSystemIconsNode->FindChild("PlanetIcon" + std::to_string(counter)).lock()->GetEntity();
                signals::onDeleteEntity(wpE2, spSystemIconsNode);
                std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + std::to_string(counter)).lock()->GetEntity();
                signals::onDeleteEntity(wpE3, spSystemIconsNode);
            }
        }
    }
}