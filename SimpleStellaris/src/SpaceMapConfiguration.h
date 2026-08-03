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
	float closeTernaryChance = 5.f;
	float afarCloseBinaryThirdAfarChance = 30.f;
	float afarTernaryChance = 65.f;
	//Distances in A.U.
	sf::Vector2f closeStarsBoundaries{ 0.1f, 10.f };
	sf::Vector2f afarStarsBoundaries{ 15.f, 1000.f };
};