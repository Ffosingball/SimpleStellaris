#include "SceneNodeVisitors.h"
#include "ECSGame.h"
#include "Components.h"
#include "CommonGameCode.h"
#include "EntitiesFunctions.h"
#include <random>
#include <iostream>
#include <SFML/Audio.hpp>
#include "SpaceObjectTypes.h"

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
void SceneNodeVisitorChangeSingleSystemVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        if (spEntity->GetName() == "CenterOfMass")
        {
            spEntity->hidden = hidden;
        }
        else if (spEntity->HasComponent(ComponentType::ObjectSystem) && spEntity->GetName() == "InsideSystem")
        {
            spEntity->hidden = hidden;
        }
        else if (spEntity->HasComponent(ComponentType::Star))
        {
            spEntity->hidden = hidden;
        }
        else if (spEntity->HasComponent(ComponentType::Planet))
        {
            spEntity->hidden = hidden;
        }
    }
}