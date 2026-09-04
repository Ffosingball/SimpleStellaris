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
    //Initialize vectors
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

    processHiddenNode = true;
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

            //Count correct system type
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

            //Count correct star type
            switch (spComStar->starType)
            {
            case StarType::RedGiant:
                redGiantAmount++;
                break;
            case StarType::RedSupergiant:
                redSupGiantAmount++;
                break;
            case StarType::Otype:
                OclassAmount++;
                break;
            case StarType::Btype:
                BclassAmount++;
                break;
            case StarType::Atype:
                AclassAmount++;
                break;
            case StarType::Ftype:
                FclassAmount++;
                break;
            case StarType::GsunLike:
                GclassAmount++;
                break;
            case StarType::KorangeDwarf:
                KclassAmount++;
                break;
            case StarType::MredDwarf:
                MclassAmount++;
                break;
            case StarType::BrownDwarf:
                brownDwarfAmount++;
                break;
            case StarType::WhiteDwarf:
                whiteDwarfAmount++;
                break;
            case StarType::NeutronStar:
                neutronAmount++;
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

            //Check if it is a moon or not
            if (!spComPlanet->isMoon)
            {
                //If not, count correct planet type
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
                //Else count correct moon type
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
    std::cout << " - Rings statistics - \n";
    std::cout << " - Number of planets with rings: " << ringsAmount << '\n';

    std::cout << "\n";
    std::cout << " - Districts statistics - \n";
    std::cout << " - Number of planets with districts: " << planetsWithDistrictsAmount << '\n';
    std::cout << " - Number of planets without districts: " << planetsWithoutDistrictsAmount << '\n';
    std::cout << " - Number of districts: " << districtsAmount << '\n';
}



//System visibility processing function
void SceneNodeVisitorChangeSingleSystemVisibility::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity is a center of mass of two stars inside ternary system
        if (spEntity->HasComponent<ObjectSystemComponent>() && spEntity->GetName() == "InsideSystem")
        {
            //spEntity->hidden = hidden;
            if (!hidden)
            {
                //Now set center of mass icon and orbit around which it rotates
                std::weak_ptr<Entity> wpCoM = ECSGame::Instance().GetEntityManager().NewEntity("CenterOfMass");
                node.AddChild(std::make_shared<SceneNode>(wpCoM));
                std::shared_ptr<StarComponent> spStar = spEntity->FindComponent<StarComponent>().lock();
                wpCoM.lock()->hidden = false;
                CreateIconForSystemOverview(node.FindChild(*wpCoM.lock()).lock(), spSystemIconsNode, "CenterOfMassIcon", "ObjectIconInSys", false, centerOfMassIconSize);
                CreateOrbitFor(spObjectOrbitsNode, "OrbitInSys", spEntity->inheritParentPosition, spStar->orbitRadius, node.GetParent().lock()->FindChild("CenterOfMass"), 5.f, sf::Color(100, 100, 100, 255), false);
            }
            else
            {
                //If hide then delete all created nodes with entities
                std::weak_ptr<SceneNode> wpN = spSystemIconsNode->FindChild("ObjectIconInSys");
                signals::onDeleteSceneNode(wpN);
                std::weak_ptr<SceneNode> wpE2 = node.FindChild("CenterOfMass");
                signals::onDeleteSceneNode(wpE2);
                std::weak_ptr<SceneNode> wpE3 = spObjectOrbitsNode->FindChild("OrbitInSys");
                signals::onDeleteSceneNode(wpE3);
            }
        }
        else if (spEntity->HasComponent<ObjectSystemComponent>())
        {
            std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();
            if (!hidden)
            {
                //Check if system is not single
                if (spSysCom->systemType != SpaceSystemType::Single)
                {
                    //Then create there center of mass icon
                    std::weak_ptr<Entity> wpCoM = ECSGame::Instance().GetEntityManager().NewEntity("CenterOfMass");
                    node.AddChild(std::make_shared<SceneNode>(wpCoM));
                    wpCoM.lock()->inheritParentPosition = false;
                    wpCoM.lock()->hidden = false;
                    CreateIconForSystemOverview(node.FindChild(*wpCoM.lock()).lock(), spSystemIconsNode, "CenterOfMassIcon", "ObjectIcon" + spSysCom->systemName, false, centerOfMassIconSize);
                }
            }
            else 
            {
                //If hide then delete all created nodes with entities
                if (spSysCom->systemType != SpaceSystemType::Single)
                {
                    std::weak_ptr<SceneNode> wpE = spSystemIconsNode->FindChild("ObjectIcon" + spSysCom->systemName);
                    signals::onDeleteSceneNode(wpE);
                    std::weak_ptr<SceneNode> wpE2 = node.FindChild("CenterOfMass");
                    signals::onDeleteSceneNode(wpE2);
                }
            }
        }
        else if (spEntity->HasComponent<StarComponent>())
        {
            //If star then
            spEntity->hidden = hidden;
            std::shared_ptr<StarComponent> spStar = spEntity->FindComponent<StarComponent>().lock();
            if (!hidden) 
            {
                //Create text name entity for star
                std::string name{ "StarNameText" + spStar->starName };
                spStar->wpStarNameText = CreateSystemText(spSystemIconsNode, node.GetSharedPtrToItself(), name, false);
                //Create icon
                std::shared_ptr<Entity> spIconEntity = CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, GetSystemTextureName(spStar->starType), "ObjectIcon" + spStar->starName, false, starIconSize, true, sf::Vector2f{0.001f, 10.f});
                spIconEntity->AddComponent<StarIconComponent>();
                //Create orbit if system is not single
                if (node.GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType != SpaceSystemType::Single)
                    CreateOrbitFor(spObjectOrbitsNode, "Orbit" + spStar->starName, spEntity->inheritParentPosition, spStar->orbitRadius, node.GetParent().lock()->FindChild("CenterOfMass"), 5.f, sf::Color(100, 100, 100, 255), false);
            }
            else 
            {
                //If hide then delete all created nodes with entities
                std::weak_ptr<SceneNode> wpE = spSystemIconsNode->FindChild("StarNameText" + spStar->starName);
                signals::onDeleteSceneNode(wpE);
                std::weak_ptr<SceneNode> wpE2 = spSystemIconsNode->FindChild("ObjectIcon" + spStar->starName);
                signals::onDeleteSceneNode(wpE2);
                //Delete orbit id system not single
                if (node.GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType != SpaceSystemType::Single)
                {
                    std::weak_ptr<SceneNode> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + spStar->starName);
                    signals::onDeleteSceneNode(wpE3);
                }
            }
        }
        else if (spEntity->HasComponent<PlanetComponent>())
        {
            std::shared_ptr<PlanetComponent> spPlanet = spEntity->FindComponent<PlanetComponent>().lock();
            if (!spPlanet->isMoon)
            {
                spEntity->hidden = hidden;
                if (!hidden)
                {
                    //Create text name entity for star
                    std::string name{ "PlanetNameText" + spPlanet->planetName };
                    CreateSystemText(spSystemIconsNode, node.GetSharedPtrToItself(), name, true);
                    //Create icon
                    CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, spPlanet->planetIconTextureName, "PlanetIcon" + spPlanet->planetName, true, planetIconSize);
                    //Get parent for orbits
                    std::weak_ptr<SceneNode> wpFollowNode;
                    if (node.GetParent().lock()->GetEntity().lock()->HasComponent<ObjectSystemComponent>())
                    {
                        if (node.GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemType == SpaceSystemType::BinaryClose)
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
                    //Create habitable planet icon
                    VisitorCountHabitablePlanets visitor;
                    node.AcceptVisitor(visitor);
                    if (visitor.counter > 0)
                    {
                        std::shared_ptr<Entity> spIcEn = CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, std::to_string(visitor.counter) + "HabitablePlanetIcon", spPlanet->planetName + "HabitableIcon", true, WorldGenerator::mapConfig.habitablePlanetIconSystemViewSize);
                        spIcEn->FindComponent<UIFollowerComponent>().lock()->offset = WorldGenerator::mapConfig.habitablePlanetIconOffset;
                    }
                }
                else
                {
                    //If hide then delete all created nodes with entities
                    std::weak_ptr<SceneNode> wpE = spSystemIconsNode->FindChild("PlanetNameText" + spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE);
                    std::weak_ptr<SceneNode> wpE2 = spSystemIconsNode->FindChild("PlanetIcon" + spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE2);
                    std::weak_ptr<SceneNode> wpE3 = spObjectOrbitsNode->FindChild("Orbit" + spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE3);

                    if (node.FindChild("Rings").lock() != nullptr) 
                    {
                        std::weak_ptr<SceneNode> wpE = spSystemIconsNode->FindChild("RingIcon" + spPlanet->planetName);
                        signals::onDeleteSceneNode(wpE);
                    }

                    std::weak_ptr<SceneNode> wpE4 = spSystemIconsNode->FindChild(spPlanet->planetName + "HabitableIcon");
                    signals::onDeleteSceneNode(wpE4);
                }
            }
        }

        if (spEntity->HasComponent<RectangleShapeComponent>()) 
        {
            std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
            sf::Color color = spRectShape->shape.getFillColor();
            color.a = 255;
            spRectShape->shape.setFillColor(color);
        }
    }
}



//This function checks if current node is the closest to the provided distance or not
void SceneNodeVisitorGetClosestNodeToPosition::ProcessNode(SceneNode& node) 
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        bool checkDistance = false;
        bool setParentAsTarget = false;
        //Now find out whether this node should be considered or not
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

        bool foundCloser = false;
        if (checkDistance) 
        {
            //If yes then check if this is the closest one and if its within the provided area
            float newDistance = gel::distanceBetween2Points(position, node.GetCombinedPosition());
            if (newDistance < gel::distanceBetween2Points(closestPosition, position))
            {
                if (newDistance < maxDistance)
                    foundCloser = true;
                else if (spEntity->HasComponent<RectangleShapeComponent>())
                {
                    if (gel::IsPointInTheArea(position, node.GetCombinedPosition(), spEntity->FindComponent<RectangleShapeComponent>().lock()->shape.getSize()))
                        foundCloser = true;
                }
            }
        }

        if (foundCloser) 
        {
            closestPosition = node.GetCombinedPosition();
            if (!setParentAsTarget)
                wpClosestNode = node.GetSharedPtrToItself();
            else
                wpClosestNode = node.GetParent();
        }
    }
}



//Planet visibility processing function
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
                    //Create habitable planet icon
                    if (spEntity->HasComponent<HabitablePlanetComponent>())
                    {
                        std::shared_ptr<Entity> spIcEn = CreateIconForSystemOverview(node.GetSharedPtrToItself(), spSystemIconsNode, "1HabitablePlanetIcon", spPlanet->planetName + "HabitableIcon", false, WorldGenerator::mapConfig.habitablePlanetIconSystemViewSize, true);
                        spIcEn->FindComponent<UIFollowerComponent>().lock()->offset = WorldGenerator::mapConfig.habitablePlanetIconOffset;
                    }
                }
                else 
                {
                    std::shared_ptr<Entity> spPlanPic = CreateNewEntityAt(node.GetSharedPtrToItself(), "PlanetPicture").lock();
                    std::shared_ptr<RectangleShapeComponent> spRecShape = spPlanPic->AddComponent<RectangleShapeComponent>().lock();
                    SetupRectangleShape(spRecShape, sf::Vector2f{1.f,1.f} * spPlanet->planetSize * earthDiameter, GetPlanetTextureName(spPlanet->planetType, spEntity->FindComponent<HabitablePlanetComponent>().lock()));
                    spPlanPic->inheritParentPosition = false;

                    spPlanetPicNode = node.FindChild(*spPlanPic).lock();
                    //Create text name entity for star
                    std::string name{ spPlanet->planetName};
                    CreateSystemText(spSystemIconsNode, spPlanetPicNode, name, false);
                    //Create icon
                    CreateIconForSystemOverview(spPlanetPicNode, spSystemIconsNode, spPlanet->planetIconTextureName, "PlanetIcon" + spPlanet->planetName, false, planetIconSize, true);
                    //Create rings icon if they exist
                    if (node.FindChild("Rings").lock() != nullptr)
                    {
                        std::shared_ptr<RingComponent> spRingCom = node.FindChild("Rings").lock()->GetEntity().lock()->FindComponent<RingComponent>().lock();
                        CreateIconForSystemOverview(spPlanetPicNode, spSystemIconsNode, spRingCom->ringIconTextureName, "RingIcon" + spPlanet->planetName, false, planetIconSize, true);
                    }
                    //Create habitable planet icon
                    if (spEntity->HasComponent<HabitablePlanetComponent>())
                    {
                        std::shared_ptr<Entity> spIcEn = CreateIconForSystemOverview(spPlanetPicNode, spSystemIconsNode, "1HabitablePlanetIcon", spPlanet->planetName + "HabitableIcon", false, WorldGenerator::mapConfig.habitablePlanetIconSystemViewSize, true);
                        spIcEn->FindComponent<UIFollowerComponent>().lock()->offset = WorldGenerator::mapConfig.habitablePlanetIconOffset;
                    }
                }
            }
            else
            {
                //If hide then delete all created nodes with entities
                if (spPlanet->isMoon)
                {
                    std::weak_ptr<SceneNode> wpE3 = spObjectOrbitsNode->FindChild("MoonOrbit" + spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE3);
                    std::weak_ptr<SceneNode> wpE = spSystemIconsNode->FindChild("MoonNameText" + spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE);
                    std::weak_ptr<SceneNode> wpE2 = spSystemIconsNode->FindChild("MoonIcon" + spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE2);
                }
                else 
                {
                    std::weak_ptr<SceneNode> wpE3 = node.FindChild("PlanetPicture");
                    signals::onDeleteSceneNode(wpE3);
                    std::weak_ptr<SceneNode> wpE = spSystemIconsNode->FindChild(spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE);
                    std::weak_ptr<SceneNode> wpE2 = spSystemIconsNode->FindChild("PlanetIcon" + spPlanet->planetName);
                    signals::onDeleteSceneNode(wpE2);

                    if (node.FindChild("Rings").lock() != nullptr)
                    {
                        std::weak_ptr<SceneNode> wpE = spSystemIconsNode->FindChild("RingIcon" + spPlanet->planetName);
                        signals::onDeleteSceneNode(wpE);
                    }
                }

                std::weak_ptr<SceneNode> wpE4 = spSystemIconsNode->FindChild(spPlanet->planetName + "HabitableIcon");
                signals::onDeleteSceneNode(wpE4);
            }
        }
        else if (spEntity->HasComponent<RingComponent>())
        {
            spEntity->hidden = hidden;
        }
    }
}



//Counts all planets, or planets with moons
void VisitorCountPlanets::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        if (spEntity->HasComponent<PlanetComponent>())
        {
            std::shared_ptr<PlanetComponent> spPlanet = spEntity->FindComponent<PlanetComponent>().lock();
            
            if(countMoons)
                counter++;
            else
            {
                if (!spPlanet->isMoon)
                    counter++;
            }
        }
    }
}



//Counts only habitable planets
void VisitorCountHabitablePlanets::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        if (spEntity->HasComponent<HabitablePlanetComponent>())
        {
            counter++;
        }
    }
}



//Changes every node visibility except entities with StarIconComponent
void ChangeAllNodesVisibilityExceptStarIcons::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();

    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //std::cout << spEntity->GetName() << '\n';
        if (!spEntity->HasComponent<StarIconComponent>())
        {
            spEntity->hidden = hidden;
            if (spEntity->HasComponent<UIFollowerComponent>())
            {
                std::shared_ptr<UIFollowerComponent> spUIFol = spEntity->FindComponent<UIFollowerComponent>().lock();
                spUIFol->hideAnyway = hidden;
            }
        }
        //else
        //    std::cout << "Star Icon!" << '\n';

        //std::cout << node.GetCombinedParentsNames() << '\n';
    }
}


void SceneNodeSpaceObjectsMemorySize::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Check if pointer is valid
    if (spEntity != nullptr)
    {
        memoryUsageByEntities++;// = sizeof(*spEntity) + spEntity->GetComponentsSize() + (spEntity->GetName().size() * sizeof(char));

        //Check if entity has system component
        if (spEntity->HasComponent<ObjectSystemComponent>())
        {
            std::shared_ptr<ObjectSystemComponent> spComSys = spEntity->FindComponent<ObjectSystemComponent>().lock();
            memoryUsageByObjectSystemComponent++;// = sizeof(*spComSys) + (spComSys->systemName.size() * sizeof(char));
            if(spComSys->spAllSystemObjectsNode !=nullptr)
                spComSys->spAllSystemObjectsNode->AcceptVisitor(*this);
        }
        
        if (spEntity->HasComponent<StarComponent>())
        {
            std::shared_ptr<StarComponent> spComStar = spEntity->FindComponent<StarComponent>().lock();
            memoryUsageByStarComponent++;// = sizeof(*spComStar) + (spComStar->starName.size() * sizeof(char));
        }
        
        if (spEntity->HasComponent<PlanetComponent>())
        {
            std::shared_ptr<PlanetComponent> spComPlanet = spEntity->FindComponent<PlanetComponent>().lock();
            memoryUsageByPlanetComponent++;// = sizeof(*spComPlanet) + (spComPlanet->planetName.size() * sizeof(char)) + (spComPlanet->planetIconTextureName.size() * sizeof(char));
        }

        if (spEntity->HasComponent<HabitablePlanetComponent>())
        {
            std::shared_ptr<HabitablePlanetComponent> spComHabPlanet = spEntity->FindComponent<HabitablePlanetComponent>().lock();
            memoryUsageByHabitablePlanetComponent++;// = sizeof(*spComHabPlanet);
        }
        
        if (spEntity->HasComponent<RingComponent>())
        {
            std::shared_ptr<RingComponent> spComRing = spEntity->FindComponent<RingComponent>().lock();
            memoryUsageByRingComponent++;// = sizeof(*spComRing) + (spComRing->ringIconTextureName.size() * sizeof(char));
        }
        
        if (spEntity->HasComponent<DistrictComponent>())
        {
            std::shared_ptr<DistrictComponent> spComDistrict = spEntity->FindComponent<DistrictComponent>().lock();
            memoryUsageByDistrictComponent++;// = sizeof(*spComDistrict);
        }

        if (spEntity->HasComponent<RectangleShapeComponent>())
        {
            std::shared_ptr<RectangleShapeComponent> spComDistrict = spEntity->FindComponent<RectangleShapeComponent>().lock();
            memoryUsageByRectangleShapeComponent++;// = sizeof(*spComDistrict);
        }

        if (spEntity->HasComponent<ButtonComponent>())
        {
            std::shared_ptr<ButtonComponent> spComDistrict = spEntity->FindComponent<ButtonComponent>().lock();
            memoryUsageByButtonComponent++;// = sizeof(*spComDistrict);
        }

        if (spEntity->HasComponent<UIPartComponent>())
        {
            std::shared_ptr<UIPartComponent> spComDistrict = spEntity->FindComponent<UIPartComponent>().lock();
            memoryUsageByUIPartComponent++;// = sizeof(*spComDistrict);
        }
    }
}


void SceneNodeSpaceObjectsMemorySize::OutputAllData()
{
    std::cout << " -- Space Map Memory Statistics -- \n";
    std::cout << "Entity: " << memoryUsageByEntities << '\n';//" B; "<<memoryUsageByEntities/1024<<" KB; "<< memoryUsageByEntities / 1024/1024<<" MB\n";
    std::cout << "Planet Component: " << memoryUsageByPlanetComponent << '\n';// << " B; " << memoryUsageByPlanetComponent / 1024 << " KB; " << memoryUsageByPlanetComponent / 1024 / 1024 << " MB\n";
    std::cout << "Star Component: " << memoryUsageByStarComponent << '\n';// << " B; " << memoryUsageByStarComponent / 1024 << " KB; " << memoryUsageByStarComponent / 1024 / 1024 << " MB\n";
    std::cout << "Object System Component: " << memoryUsageByObjectSystemComponent << '\n';// << " B; " << memoryUsageByObjectSystemComponent / 1024 << " KB; " << memoryUsageByObjectSystemComponent / 1024 / 1024 << " MB\n";
    std::cout << "Button Component: " << memoryUsageByButtonComponent << '\n';// << " B; " << memoryUsageByButtonComponent / 1024 << " KB; " << memoryUsageByButtonComponent / 1024 / 1024 << " MB\n";
    std::cout << "UI Part Component: " << memoryUsageByUIPartComponent << '\n';// << " B; " << memoryUsageByUIPartComponent / 1024 << " KB; " << memoryUsageByUIPartComponent / 1024 / 1024 << " MB\n";
    std::cout << "Rectangle Shape Component: " << memoryUsageByRectangleShapeComponent << '\n';// << " B; " << memoryUsageByRectangleShapeComponent / 1024 << " KB; " << memoryUsageByRectangleShapeComponent / 1024 / 1024 << " MB\n";
    std::cout << "Ring Component: " << memoryUsageByRingComponent << '\n';// << " B; " << memoryUsageByRingComponent / 1024 << " KB; " << memoryUsageByRingComponent / 1024 / 1024 << " MB\n";
    std::cout << "District Component: " << memoryUsageByDistrictComponent << '\n';// << " B; " << memoryUsageByDistrictComponent / 1024 << " KB; " << memoryUsageByDistrictComponent / 1024 / 1024 << " MB\n";
    std::cout << "Habitable Planet Component: " << memoryUsageByHabitablePlanetComponent << '\n';// << " B; " << memoryUsageByHabitablePlanetComponent / 1024 << " KB; " << memoryUsageByHabitablePlanetComponent / 1024 / 1024 << " MB\n";
}


//Counts every node
void VisitorCountAllNodes::ProcessNode(SceneNode& node)
{
    counter++;
}



//Buttons processing function
void VisitorGetClosestButtonAtDirection::ProcessNode(SceneNode& node)
{
    std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
    //Check that pointer is valid
    if (spEntity != nullptr)
    {
        //Check if entity has Button component and it is not hidden
        if (spEntity->HasComponent<ButtonComponent>())
        {
            sf::Vector2f entityPos = node.GetCombinedPosition() - currentPosition;
            bool suitableButton{ false };
            switch (direction) 
            {
            case 0:
                if (entityPos.y > 0 && abs(entityPos.y) > abs(entityPos.x)) 
                    suitableButton = true;
                break;
            case 1:
                if (entityPos.x > 0 && abs(entityPos.x) > abs(entityPos.y))
                    suitableButton = true;
                break;
            case 2:
                if (entityPos.y < 0 && abs(entityPos.y) > abs(entityPos.x))
                    suitableButton = true;
                break;
            case 3:
                if (entityPos.x < 0 && abs(entityPos.x) > abs(entityPos.y))
                    suitableButton = true;
                break;
            }

            if (suitableButton) 
            {
                if (wpClosestButton.lock() != nullptr)
                {
                    if (gel::distanceBetween2Points(sf::Vector2f{ 0.f,0.f }, entityPos) < gel::distanceBetween2Points(sf::Vector2f{ 0.f,0.f }, wpClosestButton.lock()->GetCombinedPosition() - currentPosition))
                        wpClosestButton = node.GetSharedPtrToItself();
                }
                else
                    wpClosestButton = node.GetSharedPtrToItself();
            }
        }
    }
}