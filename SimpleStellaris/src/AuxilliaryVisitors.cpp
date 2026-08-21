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


SceneNodeSpaceObjectsCounter::SceneNodeSpaceObjectsCounter(SpaceMapConfigurations& mapConfig) : mapConfig(mapConfig)
{
    for (int i = 0; i < 41; i++) 
    {
        planetTypesAmount.push_back(0);
    }

    for (int i = 0; i < 11; i++)
    {
        moonTypesAmount.push_back(0);
    }

    for (int i = 0; i < 10; i++)
    {
        numberOfMoonsAmount.push_back(0);
    }
}


void SceneNodeSpaceObjectsCounter::ProcessNode(SceneNode& node) 
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Check if pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has system component
        if (spEntity->HasComponent<ObjectSystemComponent>() && spEntity->GetName()!= "InsideSystem")
        {
            std::shared_ptr<ObjectSystemComponent> spComSys = spEntity->FindComponent<ObjectSystemComponent>().lock();

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

            spComSys->spAllSystemObjectsNode->AcceptVisitor(*this);
        }
        else if (spEntity->HasComponent<StarComponent>())
        {
            std::shared_ptr<StarComponent> spComStar = spEntity->FindComponent<StarComponent>().lock();

            switch (spComStar->starType)
            {
            case StarType::RedGiant:
                redGiantAmount++;
                //std::cout << "   Red Giant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::RedSupergiant:
                redSupGiantAmount++;
                //std::cout << "   Red Supergiant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Otype:
                OclassAmount++;
                //std::cout << "   Blue Supergiant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Btype:
                BclassAmount++;
                //std::cout << "   Blue Giant Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Atype:
                AclassAmount++;
                //std::cout << "   Bluish-White Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::Ftype:
                FclassAmount++;
                //std::cout << "   White Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::GsunLike:
                GclassAmount++;
                //std::cout << "   Yellow Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::KorangeDwarf:
                KclassAmount++;
                //std::cout << "   Orange Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::MredDwarf:
                MclassAmount++;
                //std::cout << "   Red Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::BrownDwarf:
                brownDwarfAmount++;
                //std::cout << "   Brawn Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::WhiteDwarf:
                whiteDwarfAmount++;
                //std::cout << "   White Dwarf Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::NeutronStar:
                neutronAmount++;
                //std::cout << "   Neutron Star (" << spEntity->GetName() << "): " << '\n';
                break;
            case StarType::BlackHole:
                blackHoleAmount++;
                blackHolePos = node.GetParent().lock()->GetEntity().lock()->GetPosition();
                break;
            }
        }
        else if (spEntity->HasComponent<PlanetComponent>())
        {
            std::shared_ptr<PlanetComponent> spComPlanet = spEntity->FindComponent<PlanetComponent>().lock();
            std::weak_ptr<HabitablePlanetComponent> wpHabitablePlanet;

            if (!spComPlanet->isMoon)
            {
                switch (spComPlanet->planetType)
                {
                case PlanetType::BarrenDark:
                    if (spComPlanet->planetSize < mapConfig.smallRockyPlanetSizes.y)
                        planetTypesAmount[0]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumRockyPlanetSizes.y)
                        planetTypesAmount[1]++;
                    else
                        planetTypesAmount[2]++;
                    break;
                case PlanetType::BarrenGrey:
                    if (spComPlanet->planetSize < mapConfig.smallRockyPlanetSizes.y)
                        planetTypesAmount[3]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumRockyPlanetSizes.y)
                        planetTypesAmount[4]++;
                    else
                        planetTypesAmount[5]++;
                    break;
                case PlanetType::BarrenMarsLike:
                    if (spComPlanet->planetSize < mapConfig.smallRockyPlanetSizes.y)
                        planetTypesAmount[6]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumRockyPlanetSizes.y)
                        planetTypesAmount[7]++;
                    else
                        planetTypesAmount[8]++;
                    break;
                case PlanetType::VenusLike:
                    if (spComPlanet->planetSize < mapConfig.smallRockyPlanetSizes.y)
                        planetTypesAmount[9]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumRockyPlanetSizes.y)
                        planetTypesAmount[10]++;
                    else
                        planetTypesAmount[11]++;
                    break;
                case PlanetType::Oceanic:
                    if (spComPlanet->planetSize < mapConfig.smallIcyPlanetSizes.y)
                        planetTypesAmount[12]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumIcyPlanetSizes.y)
                        planetTypesAmount[13]++;
                    else
                        planetTypesAmount[14]++;
                    break;
                case PlanetType::EarthLike:
                    wpHabitablePlanet = spEntity->FindComponent<HabitablePlanetComponent>().lock();
                    if (spComPlanet->planetSize < mapConfig.smallRockyPlanetSizes.y)
                    {
                        if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
                            planetTypesAmount[15]++;
                        else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
                            planetTypesAmount[16]++;
                        else
                            planetTypesAmount[17]++;
                    }
                    else if (spComPlanet->planetSize < mapConfig.mediumRockyPlanetSizes.y)
                    {
                        if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
                            planetTypesAmount[18]++;
                        else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
                            planetTypesAmount[19]++;
                        else
                            planetTypesAmount[20]++;
                    }
                    else
                    {
                        if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Close)
                            planetTypesAmount[21]++;
                        else if (wpHabitablePlanet.lock()->distanceToStar == DistanceToStar::Medium)
                            planetTypesAmount[22]++;
                        else
                            planetTypesAmount[23]++;
                    }
                    break;
                case PlanetType::TitanLike:
                    planetTypesAmount[24]++;
                    break;
                case PlanetType::Molten:
                    if (spComPlanet->planetSize < mapConfig.smallRockyPlanetSizes.y)
                        planetTypesAmount[25]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumRockyPlanetSizes.y)
                        planetTypesAmount[26]++;
                    else
                        planetTypesAmount[27]++;
                    break;
                case PlanetType::Icy:
                    if (spComPlanet->planetSize < mapConfig.smallIcyPlanetSizes.y)
                        planetTypesAmount[28]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumIcyPlanetSizes.y)
                        planetTypesAmount[29]++;
                    else
                        planetTypesAmount[30]++;
                    break;
                case PlanetType::Voulcanic:
                    planetTypesAmount[31]++;
                    break;
                case PlanetType::Desert:
                    if (spComPlanet->planetSize < mapConfig.smallRockyPlanetSizes.y)
                        planetTypesAmount[32]++;
                    else if (spComPlanet->planetSize < mapConfig.mediumRockyPlanetSizes.y)
                        planetTypesAmount[33]++;
                    else
                        planetTypesAmount[34]++;
                    break;
                case PlanetType::HotJupiter:
                    planetTypesAmount[35]++;
                    break;
                case PlanetType::HotNeptune:
                    planetTypesAmount[36]++;
                    break;
                case PlanetType::JupiterLike:
                    planetTypesAmount[37]++;
                    break;
                case PlanetType::SaturnLike:
                    planetTypesAmount[38]++;
                    break;
                case PlanetType::NeptuneLike:
                    planetTypesAmount[39]++;
                    break;
                case PlanetType::UranusLike:
                    planetTypesAmount[40]++;
                    break;
                }

                numberOfMoonsAmount[node.GetAllChildren().size()]++;
            }
            else 
            {
                switch (spComPlanet->planetType)
                {
                case PlanetType::BarrenDark:
                    moonTypesAmount[0]++;
                    break;
                case PlanetType::BarrenGrey:
                    moonTypesAmount[1]++;
                    break;
                case PlanetType::BarrenMarsLike:
                    moonTypesAmount[2]++;
                    break;
                case PlanetType::VenusLike:
                    moonTypesAmount[3]++;
                    break;
                case PlanetType::Oceanic:
                    moonTypesAmount[4]++;
                    break;
                case PlanetType::EarthLike:
                    moonTypesAmount[5]++;
                    break;
                case PlanetType::TitanLike:
                    moonTypesAmount[6]++;
                    break;
                case PlanetType::Molten:
                    moonTypesAmount[7]++;
                    break;
                case PlanetType::Icy:
                    moonTypesAmount[8]++;
                    break;
                case PlanetType::Voulcanic:
                    moonTypesAmount[9]++;
                    break;
                case PlanetType::Desert:
                    moonTypesAmount[10]++;
                    break;
                }
            }
        }
        else if (spEntity->HasComponent<RingComponent>())
        {
            ringsAmount++;
        }
    }
}



void SceneNodeSpaceObjectsCounter::OutputAllData()
{
    std::cout << " -- Space Map Statistics -- \n";
    
    int total = 0;
    std::cout << "\n";
    std::cout << " - Systems statistics - \n";
    total += singleSysAmount;
    std::cout << "Single systems: " << singleSysAmount << '\n';
    total += binaryCloseSysAmount;
    std::cout << "Binary close systems: " << binaryCloseSysAmount << '\n';
    total += binaryAfarSysAmount;
    std::cout << "Binary afar systems: " << binaryAfarSysAmount << '\n';
    total += ternaryTwoCloseOneAfarSysAmount;
    std::cout << "Ternary (binary + single) systems: " << ternaryTwoCloseOneAfarSysAmount << '\n';
    total += ternaryAfarSysAmount;
    std::cout << "Ternary all afar systems: " << ternaryAfarSysAmount << '\n';
    std::cout << " - Total systems: " << total<<'\n';

    total = 0;
    std::cout << "\n";
    std::cout << " - Star statistics - \n";
    total += redSupGiantAmount;
    std::cout << "Red Supergiants: " << redSupGiantAmount << '\n';
    total += redGiantAmount;
    std::cout << "Red Giants: " << redGiantAmount << '\n';
    total += OclassAmount;
    std::cout << "O class: " << OclassAmount << '\n';
    total += BclassAmount;
    std::cout << "B class: " << BclassAmount << '\n';
    total += AclassAmount;
    std::cout << "A class: " << AclassAmount << '\n';
    total += FclassAmount;
    std::cout << "F class: " << FclassAmount << '\n';
    total += GclassAmount;
    std::cout << "G class: " << GclassAmount << '\n';
    total += KclassAmount;
    std::cout << "K class: " << KclassAmount << '\n';
    total += MclassAmount;
    std::cout << "M class: " << MclassAmount << '\n';
    total += brownDwarfAmount;
    std::cout << "Brown Dwarfs: " << brownDwarfAmount << '\n';
    total += whiteDwarfAmount;
    std::cout << "White Dwarfs: " << whiteDwarfAmount << '\n';
    total += neutronAmount;
    std::cout << "Neutron Stars: " << neutronAmount << '\n';
    total += blackHoleAmount;
    std::cout << "Black Holes: " << blackHoleAmount << '\n';
    std::cout << "Black Hole Position: " << blackHolePos.x<<"; "<< blackHolePos.y << '\n';
    std::cout << " - Total stars: " << total << '\n';

    total = 0;
    int totalHabitable = 0;
    std::cout << "\n";
    std::cout << " - Planet statistics - \n";
    std::cout << "Small Barren Dark: " << planetTypesAmount[0] << '\n';
    std::cout << "Medium Barren Dark: " << planetTypesAmount[1] << '\n';
    std::cout << "Large Barren Dark: " << planetTypesAmount[2] << '\n';
    std::cout << "Small Barren Grey: " << planetTypesAmount[3] << '\n';
    std::cout << "Medium Barren Grey: " << planetTypesAmount[4] << '\n';
    std::cout << "Large Barren Grey: " << planetTypesAmount[5] << '\n';
    std::cout << "Small Barren Red: " << planetTypesAmount[6] << '\n';
    std::cout << "Medium Barren Red: " << planetTypesAmount[7] << '\n';
    std::cout << "Large Barren Red: " << planetTypesAmount[8] << '\n';
    std::cout << "Small Venus Like: " << planetTypesAmount[9] << '\n';
    std::cout << "Medium Venus Like: " << planetTypesAmount[10] << '\n';
    std::cout << "Large Venus Like: " << planetTypesAmount[11] << '\n';
    totalHabitable += planetTypesAmount[12];
    totalHabitable += planetTypesAmount[13];
    totalHabitable += planetTypesAmount[14];
    std::cout << "Small Oceanic: " << planetTypesAmount[12] << '\n';
    std::cout << "Medium Oceanic: " << planetTypesAmount[13] << '\n';
    std::cout << "Large Oceanic: " << planetTypesAmount[14] << '\n';
    totalHabitable += planetTypesAmount[15];
    totalHabitable += planetTypesAmount[16];
    totalHabitable += planetTypesAmount[17];
    totalHabitable += planetTypesAmount[18];
    totalHabitable += planetTypesAmount[19];
    totalHabitable += planetTypesAmount[20];
    totalHabitable += planetTypesAmount[21];
    totalHabitable += planetTypesAmount[22];
    totalHabitable += planetTypesAmount[23];
    std::cout << "Small Earth Like Close: " << planetTypesAmount[15] << '\n';
    std::cout << "Small Earth Like Medium: " << planetTypesAmount[16] << '\n';
    std::cout << "Small Earth Like Afar: " << planetTypesAmount[17] << '\n';
    std::cout << "Medium Earth Like Close: " << planetTypesAmount[18] << '\n';
    std::cout << "Medium Earth Like Medium: " << planetTypesAmount[19] << '\n';
    std::cout << "Medium Earth Like Afar: " << planetTypesAmount[20] << '\n';
    std::cout << "Large Earth Like Close: " << planetTypesAmount[21] << '\n';
    std::cout << "Large Earth Like Medium: " << planetTypesAmount[22] << '\n';
    std::cout << "Large Earth Like Afar: " << planetTypesAmount[23] << '\n';
    std::cout << "Titan Like: " << planetTypesAmount[24] << '\n';
    std::cout << "Small Molten: " << planetTypesAmount[25] << '\n';
    std::cout << "Medium Molten: " << planetTypesAmount[26] << '\n';
    std::cout << "Large Molten: " << planetTypesAmount[27] << '\n';
    std::cout << "Small Icy: " << planetTypesAmount[28] << '\n';
    std::cout << "Medium Icy: " << planetTypesAmount[29] << '\n';
    std::cout << "Large Icy: " << planetTypesAmount[30] << '\n';
    std::cout << "Voulcanic: " << planetTypesAmount[31] << '\n';
    totalHabitable += planetTypesAmount[32];
    totalHabitable += planetTypesAmount[33];
    totalHabitable += planetTypesAmount[34];
    std::cout << "Small Desert: " << planetTypesAmount[32] << '\n';
    std::cout << "Medium Desert: " << planetTypesAmount[33] << '\n';
    std::cout << "Large Desert: " << planetTypesAmount[34] << '\n';
    std::cout << "Hot Jupiter: " << planetTypesAmount[35] << '\n';
    std::cout << "Hot Neptune: " << planetTypesAmount[36] << '\n';
    std::cout << "Jupiter Like: " << planetTypesAmount[37] << '\n';
    std::cout << "Saturn Like: " << planetTypesAmount[38] << '\n';
    std::cout << "Neptune Like: " << planetTypesAmount[39] << '\n';
    std::cout << "Uranus Like: " << planetTypesAmount[40] << '\n';
    for (int i : planetTypesAmount) 
    {
        total += i;
    }
    std::cout << " - Total planets: " << total << '\n';
    std::cout << " - Total habitable planets: " << totalHabitable << '\n';

    total = 0;
    std::cout << "\n";
    std::cout << " - Moons statistics - \n";
    std::cout << "Barren Dark: " << moonTypesAmount[0] << '\n';
    std::cout << "Barren Grey: " << moonTypesAmount[1] << '\n';
    std::cout << "Barren Red: " << moonTypesAmount[2] << '\n';
    std::cout << "Venus Like: " << moonTypesAmount[3] << '\n';
    std::cout << "Oceanic: " << moonTypesAmount[4] << '\n';
    std::cout << "Earth Like: " << moonTypesAmount[5] << '\n';
    std::cout << "Titan Like: " << moonTypesAmount[6] << '\n';
    std::cout << "Molten: " << moonTypesAmount[7] << '\n';
    std::cout << "Icy: " << moonTypesAmount[8] << '\n';
    std::cout << "Voulcanic: " << moonTypesAmount[9] << '\n';
    std::cout << "Desert: " << moonTypesAmount[10] << '\n';
    for (int i : moonTypesAmount)
    {
        total += i;
    }
    std::cout << " - Total moons: " << total << '\n';
    std::cout << " - Number of moons in planets - \n";
    for (int i =0; i<numberOfMoonsAmount.size(); i++) 
    {
        std::cout << i << " moons have " << numberOfMoonsAmount[i] << " planets\n";
    }

    std::cout << "\n";
    std::cout << " - Number of planets with rings: " << ringsAmount << '\n';
}



//System visibility processing function
void SceneNodeVisitorChangeAllSystemVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has object system component
        if (spEntity->HasComponent<ObjectSystemComponent>() && spEntity->GetName()!="InsideSystem")
        {
            spEntity->hidden = hidden;
        }
        else if (spEntity->HasComponent<NebulaComponent>())
        {
            std::shared_ptr<NebulaComponent> spNebula = spEntity->FindComponent<NebulaComponent>().lock();
            spNebula->wpTextFollower.lock()->hideAnyway = hidden;
        }
    }
}


void SceneNodeVisitorChangeNebulasVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        if (spEntity->HasComponent<NebulaComponent>())
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

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        if (spEntity->HasComponent<ObjectSystemComponent>() && spEntity->GetName() == "InsideSystem")
        {
            spEntity->hidden = hidden;
            if (!hidden)
            {
                //Now set center of mass icon
                std::weak_ptr<Entity> wpCoM = ECSGame::Instance().GetEntityManager().NewEntity("CenterOfMass");
                node.AddChild(std::make_shared<SceneNode>(wpCoM));
                std::shared_ptr<StarComponent> spStar = spEntity->FindComponent<StarComponent>().lock();
                wpCoM.lock()->hidden = false;
                CreateIconForSystemOverview(node.FindChild(*wpCoM.lock()).lock(), spSystemIconsNode, "CenterOfMassIcon", "ObjectIconInSys", false, centerOfMassIconSize);
                CreateOrbitFor(spObjectOrbitsNode, "OrbitInSys", spEntity->inheritParentPosition, spStar->orbitRadius, node.GetParent().lock()->FindChild("CenterOfMass"), 5.f, sf::Color(100, 100, 100, 255), false);
            }
            else
            {
                std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("ObjectIconInSys").lock()->GetEntity();
                signals::onDeleteEntity(wpE, spSystemIconsNode);
                std::weak_ptr<Entity> wpE2 = node.FindChild("CenterOfMass").lock()->GetEntity();
                signals::onDeleteEntity(wpE2, node.GetSharedPtrToItself());
                std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("OrbitInSys").lock()->GetEntity();
                signals::onDeleteEntity(wpE3, spObjectOrbitsNode);
            }
        }
        else if (spEntity->HasComponent<ObjectSystemComponent>())
        {
            std::shared_ptr<SceneNode> spNode = node.FindChild("Node").lock();
            std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();
            if (!hidden)
            {
                if (spSysCom->systemType != SpaceSystemType::Single)
                {
                    //Now set center of mass icon
                    std::weak_ptr<Entity> wpCoM = ECSGame::Instance().GetEntityManager().NewEntity("CenterOfMass");
                    spNode->AddChild(std::make_shared<SceneNode>(wpCoM));
                    wpCoM.lock()->inheritParentPosition = false;
                    wpCoM.lock()->hidden = false;
                    CreateIconForSystemOverview(node.FindChild(*wpCoM.lock()).lock(), spSystemIconsNode, "CenterOfMassIcon", "ObjectIcon" + spSysCom->systemName, false, centerOfMassIconSize);
                }
            }
            else 
            {
                if (spSysCom->systemType != SpaceSystemType::Single)
                {
                    std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("ObjectIcon" + spSysCom->systemName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE, spSystemIconsNode);
                    std::weak_ptr<Entity> wpE2 = spNode->FindChild("CenterOfMass").lock()->GetEntity();
                    signals::onDeleteEntity(wpE2, spNode);
                }
            }
        }
        else if (spEntity->HasComponent<StarComponent>())
        {
            spEntity->hidden = hidden;
            std::shared_ptr<StarComponent> spStar = spEntity->FindComponent<StarComponent>().lock();
            if (!hidden) 
            {
                //Create text name entity for star
                std::string name{ "StarNameText" + spStar->starName };
                spStar->wpStarNameText = CreateSystemText(spSystemIconsNode, node.GetSharedPtrToItself(), name, false);
                //spStar->wpStarNameText.lock()->hidden = true;
                //spEntity->hidden = false;
                //Create icon
                CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, GetSystemTextureName(spStar->starType), "ObjectIcon" + spStar->starName, false, starIconSize, true, sf::Vector2f{0.001f, 10.f});
                //Create orbit
                if (node.GetParent().lock()->GetParent().lock()->GetEntity().lock()->HasComponent<ObjectSystemComponent>())
                {
                    if (node.GetParent().lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType != SpaceSystemType::Single)
                        CreateOrbitFor(spObjectOrbitsNode, "Orbit" + spStar->starName, spEntity->inheritParentPosition, spStar->orbitRadius, node.GetParent().lock()->FindChild("CenterOfMass"), 5.f, sf::Color(100, 100, 100, 255), false);
                }
                else 
                {
                    if (node.GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType != SpaceSystemType::Single)
                        CreateOrbitFor(spObjectOrbitsNode, "Orbit" + spStar->starName, spEntity->inheritParentPosition, spStar->orbitRadius, node.GetParent().lock()->FindChild("CenterOfMass"), 5.f, sf::Color(100, 100, 100, 255), false);
                }
            }
            else 
            {
                std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("StarNameText" + spStar->starName).lock()->GetEntity();
                signals::onDeleteEntity(wpE, spSystemIconsNode);
                std::weak_ptr<Entity> wpE2 = spSystemIconsNode->FindChild("ObjectIcon" + spStar->starName).lock()->GetEntity();
                signals::onDeleteEntity(wpE2, spSystemIconsNode);
                if (node.GetParent().lock()->GetParent().lock()->GetEntity().lock()->HasComponent<ObjectSystemComponent>())
                {
                    if (node.GetParent().lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType != SpaceSystemType::Single)
                    {
                        std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + spStar->starName).lock()->GetEntity();
                        signals::onDeleteEntity(wpE3, spObjectOrbitsNode);
                    }
                }
                else 
                {
                    if (node.GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType != SpaceSystemType::Single)
                    {
                        std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + spStar->starName).lock()->GetEntity();
                        signals::onDeleteEntity(wpE3, spObjectOrbitsNode);
                    }
                }

            }
        }
        else if (spEntity->HasComponent<PlanetComponent>())
        {
            std::shared_ptr<PlanetComponent> spPlanet = spEntity->FindComponent<PlanetComponent>().lock();
            if (!spPlanet->isMoon)
            {
                spEntity->hidden = hidden;
                //std::shared_ptr<PlanetComponent> spPlanet = GetPlanetComponent(*spEntity);
                if (!hidden)
                {
                    //std::cout << spPlanet->planetName << ": " << spEntity->inheritParentPosition << '\n';
                    //Create text name entity for star
                    std::string name{ "PlanetNameText" + spPlanet->planetName };
                    CreateSystemText(spSystemIconsNode, node.GetSharedPtrToItself(), name, true);
                    //spStar->wpStarNameText.lock()->hidden = true;
                    //spEntity->hidden = false;
                    //Create icon
                    CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, spPlanet->planetIconTextureName, "PlanetIcon" + spPlanet->planetName, true, planetIconSize);
                    //Get parent for orbits
                    std::weak_ptr<SceneNode> wpFollowNode;
                    if (node.GetParent().lock()->GetParent().lock()->GetEntity().lock()->HasComponent<ObjectSystemComponent>())
                    {
                        if (node.GetParent().lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType == SpaceSystemType::BinaryClose)
                            wpFollowNode = node.GetParent().lock()->FindChild("CenterOfMass");
                        else
                            wpFollowNode = node.GetParent();
                    }
                    else
                        wpFollowNode = node.GetParent();
                    //Create orbit
                    CreateOrbitFor(spObjectOrbitsNode, "Orbit" + spPlanet->planetName, spEntity->inheritParentPosition, spPlanet->orbitRadius, wpFollowNode, 2.f, sf::Color(200, 200, 200, 255), true);
                    //Create ring icon if needed
                    if (node.FindChild("Rings").lock() != nullptr)
                    {
                        std::shared_ptr<RingComponent> spRingCom = node.FindChild("Rings").lock()->GetEntity().lock()->FindComponent<RingComponent>().lock();
                        CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, spRingCom->ringIconTextureName, "RingIcon" + spPlanet->planetName, true, planetIconSize);
                    }
                }
                else
                {
                    std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("PlanetNameText" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE, spSystemIconsNode);
                    std::weak_ptr<Entity> wpE2 = spSystemIconsNode->FindChild("PlanetIcon" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE2, spSystemIconsNode);
                    std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE3, spObjectOrbitsNode);

                    if (node.FindChild("Rings").lock() != nullptr) 
                    {
                        std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("RingIcon" + spPlanet->planetName).lock()->GetEntity();
                        signals::onDeleteEntity(wpE, spSystemIconsNode);
                    }
                }
            }
        }
    }
}



void SceneNodeVisitorGetClosestNodeToPosition::ProcessNode(SceneNode& node) 
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        bool checkDistance = false;
        bool setParentAsTarget = false;
        if (spEntity->HasComponent<StarComponent>())
            checkDistance = true;
        else if (spEntity->HasComponent<PlanetComponent>() && checkPlanets && currentOverview == OverviewType::System)
            checkDistance = true;
        else if (spEntity->HasComponent<PlanetComponent>() && currentOverview == OverviewType::Planet)
        {
            std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
            if(spPlanetCom->isMoon)
                checkDistance = true;
        }
        else if (currentOverview == OverviewType::Planet && spEntity->GetName() == "PlanetPicture")
        {
            checkDistance = true;
            setParentAsTarget = true;
        }

        if (checkDistance) 
        {
            float newDistance = gel::distanceBetween2Points(position, node.GetCombinedPosition());
            if (newDistance < gel::distanceBetween2Points(closestPosition, position) && newDistance < maxDistance) 
            {
                closestPosition = node.GetCombinedPosition();
                if(!setParentAsTarget)
                    wpClosestNode = node.GetSharedPtrToItself();
                else
                    wpClosestNode = node.GetParent();
            }
        }
    }
}



//Planet visibility processing function
//I assume that the position of nodes in the system will not be changed!
void SceneNodeVisitorChangeSinglePlanetVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        if (spEntity->HasComponent<PlanetComponent>())
        {
            spEntity->hidden = hidden;
            std::shared_ptr<PlanetComponent> spPlanet = spEntity->FindComponent<PlanetComponent>().lock();

            if (!hidden)
            {
                if (spPlanet->isMoon)
                {
                    //Create text name entity for star
                    std::string name{ "MoonNameText" + spPlanet->planetName };
                    CreateSystemText(spSystemIconsNode, node.GetSharedPtrToItself(), name, false);
                    //Create icon
                    CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, spPlanet->planetIconTextureName, "MoonIcon" + spPlanet->planetName, false, moonIconSize, true);
                    //Create orbit
                    CreateOrbitFor(spObjectOrbitsNode, "MoonOrbit" + spPlanet->planetName, spEntity->inheritParentPosition, spPlanet->orbitRadius, spPlanetPicNode, 2.f, sf::Color(200, 200, 200, 255), false);
                }
                else 
                {
                    std::shared_ptr<Entity> spPlanPic = CreateNewEntityAt(node.GetSharedPtrToItself(), "PlanetPicture").lock();
                    std::shared_ptr<RectangleShapeComponent> spRecShape = spPlanPic->AddComponent<RectangleShapeComponent>().lock();
                    SetupRectangleShape(spRecShape, sf::Vector2f{1.f,1.f} * spPlanet->planetSize * earthDiameter, GetPlanetTextureName(spPlanet->planetType, spEntity->FindComponent<HabitablePlanetComponent>().lock()));
                    spPlanPic->inheritParentPosition = false;

                    spPlanetPicNode = node.FindChild(*spPlanPic).lock();
                    //Create text name entity for star
                    std::string name{ "PlanetText"+spPlanet->planetName};
                    CreateSystemText(spSystemIconsNode, spPlanetPicNode, name, false);
                    //Create icon
                    CreateIconForSystemOverview(spPlanetPicNode, spSystemIconsNode, spPlanet->planetIconTextureName, "PlanetIcon" + spPlanet->planetName, false, planetIconSize, true);
                    //Create rings icon if they exist
                    if (node.FindChild("Rings").lock() != nullptr)
                    {
                        std::shared_ptr<RingComponent> spRingCom = node.FindChild("Rings").lock()->GetEntity().lock()->FindComponent<RingComponent>().lock();
                        CreateIconForSystemOverview(spPlanetPicNode, spSystemIconsNode, spRingCom->ringIconTextureName, "RingIcon" + spPlanet->planetName, false, planetIconSize, true);
                    }
                }
            }
            else
            {
                if (spPlanet->isMoon)
                {
                    std::weak_ptr<Entity> wpE3 = spObjectOrbitsNode->FindChild("MoonOrbit" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE3, spObjectOrbitsNode);
                    std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("MoonNameText" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE, spSystemIconsNode);
                    std::weak_ptr<Entity> wpE2 = spSystemIconsNode->FindChild("MoonIcon" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE2, spSystemIconsNode);
                }
                else 
                {
                    std::weak_ptr<Entity> wpE3 = node.FindChild("PlanetPicture").lock()->GetEntity();
                    signals::onDeleteEntity(wpE3, node.GetSharedPtrToItself());
                    std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("PlanetText" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE, spSystemIconsNode);
                    std::weak_ptr<Entity> wpE2 = spSystemIconsNode->FindChild("PlanetIcon" + spPlanet->planetName).lock()->GetEntity();
                    signals::onDeleteEntity(wpE2, spSystemIconsNode);

                    if (node.FindChild("Rings").lock() != nullptr)
                    {
                        std::weak_ptr<Entity> wpE = spSystemIconsNode->FindChild("RingIcon" + spPlanet->planetName).lock()->GetEntity();
                        signals::onDeleteEntity(wpE, spSystemIconsNode);
                    }
                }
            }
        }
        else if (spEntity->HasComponent<RingComponent>())
        {
            spEntity->hidden = hidden;
        }
    }
}