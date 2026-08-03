#pragma once
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "SceneNode.h"
#include "EntitiesFunctions.h"
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
//#include <memory>
#include "Components.h"
#include "Component.h"
//#include <SFML/Graphics.hpp>
#include <iostream>
//#include <random>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "GameState.h"
//#include "ParticlesConfigurations.h"
#include "SpaceObjectTypes.h"


struct SpaceMapConfigurations
{
	//Background configs
	sf::Vector2i backgroundSize{ 18,18 };

	//System configs
	int systemAmount{ 1000 };
	sf::Vector2f systemEntitySize{ 20.f, 20.f };
	float minDistanceBetweenSystems{ 5.f };
	int maxAmountOfSystemPosRegen{10};
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


class WorldGenerator 
{
private:
	static unsigned int seed;
	static std::shared_ptr<std::mt19937> randomizer;
	static float nebulaRareness;
	static int numberOfNebulas;

	//Distributions
	static std::shared_ptr<std::discrete_distribution<int>> starDistribution;
	static std::shared_ptr<std::discrete_distribution<int>> giantSysDistribution;
	static std::shared_ptr<std::discrete_distribution<int>> mediumSysDistribution;
	static std::shared_ptr<std::discrete_distribution<int>> dwarfSysDistribution;
	static std::shared_ptr<std::discrete_distribution<int>> binarySysDistribution;
	static std::shared_ptr<std::discrete_distribution<int>> ternarySysDistribution;
	static std::shared_ptr<std::uniform_real_distribution<float>> closeStarsDistances;
	static std::shared_ptr<std::uniform_real_distribution<float>> afarStarsDistances;
	static std::shared_ptr<std::uniform_int_distribution<int>> oneThird;

	WorldGenerator() = delete;                    // Prevent construction
	WorldGenerator(const WorldGenerator&) = delete;         // Prevent copying
	WorldGenerator& operator=(const WorldGenerator&) = delete;

	static void GenerateSystemType(std::shared_ptr<std::discrete_distribution<int>> systemTypeDist, std::shared_ptr<ObjectSystemComponent> spSystemCom, std::shared_ptr<SceneNode> wpSystemNode, std::shared_ptr<Entity> spStar1Entuty);
	static void StarTypeGenerator(std::weak_ptr<StarComponent> wpStarCom);
	static void checkRandomDistribution();

public:

	static int getSeed() { return seed; }
	static void Initialize(unsigned int seed);
	//Min and max value are used so vector 0,3 all values 0,1,2,3 will be generated
	static std::vector<int> GenerateGridOfRandomNumbers(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
	static std::vector<int> GenerateGridOfTiles(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
	static void GenerateSpaceMap(std::shared_ptr<SceneNode> ptrSpaceMapNode, SpaceMapConfigurations& mapConfig);
};


//To set rectangleShapes and textures for all objects 
class TextureSetter : public SceneNodeVisitor
{
public:
	TextureSetter() {}

	void ProcessNode(SceneNode& node) override;

	SpaceMapConfigurations mapConfig;
	std::shared_ptr<SceneNode> ptrSpaceMapNode;
private:
	void SetSystemTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType);
};