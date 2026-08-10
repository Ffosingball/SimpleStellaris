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
#include "SpaceMapConfiguration.h"


class WorldGenerator 
{
private:
	static unsigned int seed;
	static std::shared_ptr<std::mt19937> randomizer;
	static float nebulaRareness;
	static int numberOfNebulas;
	static std::vector<float> orbitsGenerated;

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
	static std::shared_ptr<std::uniform_real_distribution<float>> from0to1Dist;
	static std::shared_ptr<std::uniform_real_distribution<float>> from0to2_3Dist;
	static std::shared_ptr<std::uniform_int_distribution<int>> redSupGiantPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> redGiantPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> OclassPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> BclassPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> AclassPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> FclassPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> GclassPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> KclassPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> MclassPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> brownDwarfPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> whiteDwarfPlanetsDist;
	static std::shared_ptr<std::uniform_int_distribution<int>> neutronStarPlanetsDist;
	static std::shared_ptr<std::discrete_distribution<int>> closerThanHabitableZoneDist;
	static std::shared_ptr<std::discrete_distribution<int>> withinHabitableZoneDist;
	static std::shared_ptr<std::discrete_distribution<int>> furtherThanHabitableZoneDist;
	static std::shared_ptr<std::uniform_real_distribution<float>> smallRockyPlanetDist;
	static std::shared_ptr<std::uniform_real_distribution<float>> mediumRockyPlanetDist;
	static std::shared_ptr<std::uniform_real_distribution<float>> largeRockyPlanetDist;
	static std::shared_ptr<std::uniform_real_distribution<float>> smallIcyPlanetDist;
	static std::shared_ptr<std::uniform_real_distribution<float>> mediumIcyPlanetDist;
	static std::shared_ptr<std::uniform_real_distribution<float>> largeIcyPlanetDist;

	WorldGenerator() = delete;                    // Prevent construction
	WorldGenerator(const WorldGenerator&) = delete;         // Prevent copying
	WorldGenerator& operator=(const WorldGenerator&) = delete;

	static void GenerateSystemType(std::shared_ptr<std::discrete_distribution<int>> systemTypeDist, std::shared_ptr<ObjectSystemComponent> spSystemCom, std::shared_ptr<SceneNode> wpSystemNode, std::shared_ptr<Entity> spStar1Entuty, SpaceMapConfigurations& mapConfig);
	static void StarTypeGenerator(std::weak_ptr<StarComponent> wpStarCom);
	static void checkRandomDistribution();
	static void GeneratePlanets(std::shared_ptr<SceneNode> spSystemOrStarNode, SpaceMapConfigurations& mapConfig, float distanceBetweenStars, bool singleStarSystem, bool inheritPosition);
	static void GenerateSinglePlanet(sf::Vector2f orbitBoundaries, sf::Vector2f habitableZoneBoundaries, int num, std::shared_ptr<SceneNode> spNode, SpaceMapConfigurations& mapConfig, float starMass, bool inheritPosition);

public:

	static int const getSeed() { return seed; }
	static void Initialize(unsigned int seed);
	//Min and max value are used so vector 0,3 all values 0,1,2,3 will be generated
	static std::vector<int> GenerateGridOfRandomNumbers(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
	static std::vector<int> GenerateGridOfTiles(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
	static void GenerateSpaceMap(std::shared_ptr<SceneNode> ptrSpaceMapNode, SpaceMapConfigurations& mapConfig);
	static void GenerateNebulas(std::shared_ptr<SceneNode> ptrNebulasNode, SpaceMapConfigurations& mapConfig, std::shared_ptr<SceneNode> spSystemNamesNode);
};


//To set rectangleShapes and textures for all objects and generate names
class TextureSetter : public SceneNodeVisitor
{
public:
	TextureSetter(unsigned int seed);

	void ProcessNode(SceneNode& node) override;

	SpaceMapConfigurations mapConfig;
	std::weak_ptr<SceneNode> wpSpaceMapNode;

private:
	void SetSystemTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType);
	void SetSystemName(std::shared_ptr<ObjectSystemComponent> spSpaceSys, StarType starType);
	void SetStarTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType);

	unsigned int seed{0};
	std::vector<std::string> listOfBrightStarNames;
	std::vector<std::string> listOfMediumStarNames;
	std::vector<std::string> listOfDimStarNames;
	std::shared_ptr<std::mt19937> randomizer;

	std::weak_ptr<SceneNode> wpSystemNamesNode;
};


std::string GetSystemTextureName(StarType starType);
std::string GetPlanetTextureName(PlanetType planetType, std::weak_ptr<HabitablePlanetComponent> wpHabitablePlanet);


class SortedPlanetComponentsList 
{
public:
	SortedPlanetComponentsList() {};

	void AddPlanetComponent(std::shared_ptr<PlanetComponent> spPlanCom);

	std::weak_ptr<PlanetComponent> DequeuePlanetComponent();

	int Size();

private:
	std::vector<std::shared_ptr<PlanetComponent>> sortedListOfPlanetCom;
};