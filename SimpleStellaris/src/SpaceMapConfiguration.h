#pragma once

#include <SFML/Graphics.hpp>

struct SpaceMapConfigurations
{
	//Background configs
	sf::Vector2i backgroundSize{ 18,18 };

	//System configs
	int systemAmount{ 1000 };
	sf::Vector2f systemEntitySize{ 20.f, 20.f };
	float minDistanceBetweenSystems{ 5.f };
	int maxAmountOfSystemPosRegen{ 10 };
	//Distance in light years
	sf::Vector2f verticalPosBoundaries{ -400.f, 400.f };
	sf::Vector2f horizontalPosBoundaries{ -400.f, 400.f };

	//Star sizes
	//All sizes relative to sun size
	float redSupGiantSize = 1000.f;
	float redGiantSize = 100.f;
	float classOSize = 10.f;
	float classBSize = 3.f;
	float classASize = 1.6f;
	float classFSize = 1.2f;
	float classGSize = 1.f;
	float classKSize = 0.8f;
	float classMSize = 0.5f;
	float brownDwarfSize = 0.1f;
	float whiteDwarfSize = 0.01f;
	float neutronSize = 0.01f;
	float blackHoleSize = 0.001f;

	//Star chances to appear
	float redSupGiantChance = 0.25f;//Before 0.001
	float redGiantChance = 2.f;//Before 1.8
	float classOChance = 0.25f;//Before 0.001
	float classBChance = 0.4f;//Before 0.2
	float classAChance = 0.7f;//Before 0.4
	float classFChance = 4.f;//Before 3
	float classGChance = 7.f;// Before 6
	float classKChance = 10.f;//before 8
	float classMChance = 60.f;//Before 63.6
	float brownDwarfChance = 10.f;//Before 13
	float whiteDwarfChance = 5.f;//Before 4
	float neutronChance = 0.2f;//Before 0.001
	float blackHoleChance = 0.2f;//Before 0.001

	//Singular, binary and ternary system chances
	float giantSingleChance = 10.f;
	float giantBinaryChance = 68.f;
	float giantTernaryChance = 22.f;
	float mediumSingleChance = 55.f;
	float mediumBinaryChance = 35.f;
	float mediumTernaryChance = 10.f;
	float dwarfSingleChance = 75.f;
	float dwarfBinaryChance = 22.f;
	float dwarfTernaryChance = 3.f;

	//Binary and ternary system star distances chances and boundaries
	float closeBinaryChance = 35.f;
	float afarBinaryChance = 65.f;
	float ternaryCloseBinaryThirdAfarChance = 90.f;
	float afarTernaryChance = 10.f;
	//Distances in A.U.
	sf::Vector2f closeStarsBoundaries{ 0.1f, 10.f };
	sf::Vector2f afarStarsBoundaries{ 15.f, 1000.f };
	float sunDiameter{ 0.009346f };//0.009346f

	//Planets configuration
	//Habitable zone size
	sf::Vector2f habitableZoneRedSupergiant{ 70.999f,71.f };//No hab planets
	sf::Vector2f habitableZoneRedGiant{ 20.f,50.f };
	sf::Vector2f habitableZoneClassO{ 70.999f,71.f };//No hab planets
	sf::Vector2f habitableZoneClassB{ 10.f,50.f };
	sf::Vector2f habitableZoneClassA{ 3.f,6.f };
	sf::Vector2f habitableZoneClassF{ 1.5f,3.f };
	sf::Vector2f habitableZoneClassG{ 0.8f,1.5f };
	sf::Vector2f habitableZoneClassK{ 0.4f,0.8f };
	sf::Vector2f habitableZoneClassM{ 0.05f,0.3f };
	sf::Vector2f habitableZoneBrownDwarf{ 0.003f,0.03f };
	sf::Vector2f habitableZoneWhiteDwarf{ 0.199f,0.2f };//No hab planets

	//Orbits distance boundaries
	sf::Vector2f orbitBoundariesRedSupergiant{ 8.f,1000.f };
	sf::Vector2f orbitBoundariesRedGiant{ 2.f,200.f };
	sf::Vector2f orbitBoundariesClassO{ 1.f,3000.f };
	sf::Vector2f orbitBoundariesClassB{ 0.7f,1000.f };
	sf::Vector2f orbitBoundariesClassA{ 0.2f,156.f };
	sf::Vector2f orbitBoundariesClassF{ 0.1f,78.f };
	sf::Vector2f orbitBoundariesClassG{ 0.07f,40.f };
	sf::Vector2f orbitBoundariesClassK{ 0.04f,21.f };
	sf::Vector2f orbitBoundariesClassM{ 0.01f,8.f };
	sf::Vector2f orbitBoundariesBrownDwarf{ 0.001f,0.78f };
	sf::Vector2f orbitBoundariesWhiteDwarf{ 1.f,20.f };
	sf::Vector2f orbitBoundariesNeutronStar{ 5.f,50.f };

	//Chances of planet to orbit star
	//1 - 100%, 0 - 0%
	float redSupGiantPlanetChance = 1.f;
	float redGiantPlanetChance = 1.f;
	float classOPlanetChance = 0.7f;
	float classBPlanetChance = 0.1f;
	float classAPlanetChance = 1.f;
	float classFPlanetChance = 1.f;
	float classGPlanetChance = 1.f;
	float classKPlanetChance = 1.f;
	float classMPlanetChance = 1.f;
	float brownDwarfPlanetChance = 0.6f;
	float whiteDwarfPlanetChance = 0.3f;
	float neutronPlanetChance = 0.4f;
	float blackHolePlanetChance = 0.f;

	//Number of planets
	sf::Vector2i planetsAmountRedSupergiant{ 2,10 };
	sf::Vector2i planetsAmountRedGiant{ 1,7 };
	sf::Vector2i planetsAmountClassO{ 1,3 };
	sf::Vector2i planetsAmountClassB{ 1,6 };
	sf::Vector2i planetsAmountClassA{ 3,12 };
	sf::Vector2i planetsAmountClassF{ 5,15 };
	sf::Vector2i planetsAmountClassG{ 4,12 };
	sf::Vector2i planetsAmountClassK{ 2,8 };
	sf::Vector2i planetsAmountClassM{ 1,6 };
	sf::Vector2i planetsAmountBrownDwarf{ 1,2 };
	sf::Vector2i planetsAmountWhiteDwarf{ 1,4 };
	sf::Vector2i planetsAmountNeutronStar{ 1,2 };

	//Chances of planet type in front of habitable zone
	//Lava planet is 100% if it is 3 times closer than min habitable zone boundary
	float closeOrbitVenusLikeChance = 40.f;
	float closeOrbitBarrenChance = 55.f;
	float closeOrbitHotJupiterChance = 2.5f;
	float closeOrbitHotNeptuneChance = 2.5f;

	//Chances of planet type within of habitable zone
	float mediumOrbitVenusLikeChance = 32.f;
	float mediumOrbitOceanicChance = 10.f;
	float mediumOrbitEarthLikeChance = 5.f;
	float mediumOrbitDesertChance = 5.f;
	float mediumOrbitBarrenChance = 43.f;
	float mediumOrbitHotJupiterChance = 2.5f;
	float mediumOrbitHotNeptuneChance = 2.5f;

	//Chances of planet type further of habitable zone
	float afarOrbitIcyChance = 35.f;
	float afarOrbitBarrenChance = 10.f;
	//After decide texture in 50/50 chance
	float afarOrbitNeptuneLikeChance = 30.f;
	float afarOrbitJupiterLikeChance = 25.f;

	//Rocky planet config; applied for all planets except: gas giants, icy and oceanic worlds
	sf::Vector2f smallRockyPlanetSizes{ 0.3f,0.8f };
	sf::Vector2f mediumRockyPlanetSizes{ 0.8f,1.3f };
	sf::Vector2f largeRockyPlanetSizes{ 1.3f,2.2f };
	//1 - 100%, 0 - 0%
	float smallRockyPlanetChance = 0.45f;
	float mediumRockyPlanetChance = 0.35f;
	float largeRockyPlanetChance = 0.2f;

	//Icy planet config; applied for icy and oceanic worlds
	sf::Vector2f smallIcyPlanetSizes{ 0.4f,0.8f };
	sf::Vector2f mediumIcyPlanetSizes{ 0.8f,1.5f };
	sf::Vector2f largeIcyPlanetSizes{ 1.5f,3.5f };
	//1 - 100%, 0 - 0%
	float smallIcyPlanetChance = 0.2f;
	float mediumIcyPlanetChance = 0.5f;
	float largeIcyPlanetChance = 0.3f;

	//gas giants config
	sf::Vector2f smallGasSizes{ 3.5f,7.f };
	sf::Vector2f largeGasSizes{ 7.5f,13.f };

	//Planet orbits config
	float chanceOfTheClosePlanet{ 0.4f };
	float minDistanceBetweenPlanetOrbitsInPercentage{ 0.1f };

	//Nebulas generation config
	float minimalDistanceFromTheMapBorder{ 0.15f };
	sf::Vector2i numOfNebulasInWorld{ 2,4 };
	sf::Vector2f nebulaSizeRange{ 150.f,450.f };
	std::vector<std::string> nebulaNames{ "Nebula Aurelia", "Nebula Caelestis", "Nebula Ignis", "Nebula Aeterna", "Nebula Umbra", "Nebula Stellaris", "Nebula Serpens", "Nebula Lucida", "Nebula Ardens", "Nebula Vespera", "Nebula Aurora", "Nebula Imperia", "Nebula Solaris", "Nebula Caerulea", "Nebula Fulgoris", "Nebula Noctis", "Nebula Draconis", "Nebula Magnifica", "Nebula Venatoria", "Nebula Eternum"};
	int numOfNebulaTextures = 4;

	//Moons generation config
	float planetCameraMaxBoundary = 14000.f;
	float earthDiameter{ 12.756f };
	float maxMoonSizeRelativeToPlanetSize = 0.4f;
	float minMoonSize = 0.1f;
	
	//Moons chances
	float smallPlanet0MoonChance = 0.4f;
	float smallPlanet1MoonChance = 0.6f;
	float mediumPlanet1MoonChance = 0.7f;
	float mediumPlanet2MoonChance = 0.3f;
	float largePlanet1MoonChance = 0.65f;
	float largePlanet2MoonChance = 0.23f;
	float largePlanet3MoonChance = 0.12f;
	float smallGasSize1MoonChance = 0.15f;
	float smallGasSize2MoonChance = 0.25f;
	float smallGasSize3MoonChance = 0.35f;
	float smallgGasSize4MoonChance = 0.25f;
	float largeGasSize1_2MoonChance = 0.15f;
	float largeGasSize3_4MoonChance = 0.25f;
	float largeGasSize5_6MoonChance = 0.35f;
	float largeGasSize7_8MoonChance = 0.25f;

	//Moons orbit chances
	float rockyPlanetCloseOrbitMoonChance = 0.25f;
	float rockyPlanetHabitableOrbitMoonChance = 0.5f;
	float rockyPlanetAfarOrbitMoonChance = 0.75f;
	float gasPlanetCloseOrbitMoonChance = 0.6f;
	float gasPlanetHabitableOrbitMoonChance = 0.8f;
	float gasPlanetAfarOrbitMoonChance = 1.f;

	//Moons orbits config
	float closeMoonOrbitChance = 0.7f;
	float rockyPlanetCloseOrbitMultiplier = 0.6f;
	float gasPlanetCloseOrbitMultiplier = 0.2f;

	//Moon type chances
	//ALSO voulcanic planets should appear only around gas giants!!!
	float closerThanHabitBarrenMoonChance = 0.6f;
	float closerThanHabitVoulcanicMoonChance = 0.2f;
	float closerThanHabitVenusLikeMoonChance = 0.2f;
	float furtherThanHabitBarrenMoonChance = 0.5f;
	float furtherThanHabitVoulcanicMoonChance = 0.05f;
	float furtherThanHabitTitanLikeMoonChance = 0.15f;
	float furtherThanHabitIcyMoonChance = 0.3f;

	float habitZoneBarrenMoonChance = 0.5f;
	float habitZoneOceanMoonChance = 0.1f;
	float habitZoneVenusLikeMoonChance = 0.2f;
	float habitZoneEarthLikeMoonChance = 0.02f;
	float habitZoneDesertMoonChance = 0.02f;
	float habitZoneVoulcanicMoonChance = 0.2f;
};