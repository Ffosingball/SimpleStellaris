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
	float redSupGiantChance = 0.1f;//Before 0.001
	float redGiantChance = 1.8f;
	float classOChance = 0.1f;//Before 0.001
	float classBChance = 0.2f;//Before 0.1
	float classAChance = 0.4f;
	float classFChance = 3.f;
	float classGChance = 6.f;
	float classKChance = 8.f;
	float classMChance = 63.f;//Before 63.6
	float brownDwarfChance = 13.f;
	float whiteDwarfChance = 4.f;
	float neutronChance = 0.1f;//Before 0.001
	float blackHoleChance = 0.01f;//Before 0.001

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
	float sunDiameter{0.009346f};//0.009346f

	//Planets configuration
	//Habitable zone size
	sf::Vector2f habitableZoneRedSupergiant{ 70.999f,71.f };//No hab planets
	sf::Vector2f habitableZoneRedGiant{20.f,50.f};
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
	float mediumOrbitVenusLikeChance = 22.5f;
	float mediumOrbitOceanicChance = 15.8f;
	float mediumOrbitEarthLikeChance = 15.8f;
	float mediumOrbitDesertChance = 15.8f;
	float mediumOrbitBarrenChance = 25.f;
	float mediumOrbitHotJupiterChance = 2.5f;
	float mediumOrbitHotNeptuneChance = 2.5f;

	//Chances of planet type further of habitable zone
	float afarOrbitIcyChance = 35.f;
	float afarOrbitBarrenChance = 10.f;
	//After decide texture in 50/50 chance
	float afarOrbitNeptuneLikeChance = 30.f;
	float afarOrbitJupiterLikeChance = 25.f;

	//Rocky planet config; applied for all planets except: gas giants, icy and oceanic worlds
	sf::Vector2f smallRockyPlanetSizes{0.3f,0.8f};
	sf::Vector2f mediumRockyPlanetSizes{ 0.8f,1.2f };
	sf::Vector2f largeRockyPlanetSizes{ 1.2f,2.2f };
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
};