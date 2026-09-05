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
#include "Components.h"
#include "Component.h"
#include <iostream>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "GameState.h"
#include "SpaceObjectTypes.h"
#include "SpaceMapConfiguration.h"
#include "CompilerInstructions.h"
#include "Systems.h"


class WorldGenerator 
{
private:
	unsigned int seed{0};
	SpaceMapConfigurations mapConfig;

	std::shared_ptr<std::mt19937> randomizer;
    std::vector<double> orbitsGenerated;
	std::vector<double> moonOrbitsGenerated;
	bool worldGenerated{false};

	//Distributions
	std::shared_ptr<std::discrete_distribution<int>> starDistribution;
	std::shared_ptr<std::discrete_distribution<int>> giantSysDistribution;
	std::shared_ptr<std::discrete_distribution<int>> mediumSysDistribution;
	std::shared_ptr<std::discrete_distribution<int>> dwarfSysDistribution;
	std::shared_ptr<std::discrete_distribution<int>> binarySysDistribution;
	std::shared_ptr<std::discrete_distribution<int>> ternarySysDistribution;
	std::shared_ptr<std::uniform_real_distribution<double>> closeStarsDistances;
	std::shared_ptr<std::uniform_real_distribution<double>> afarStarsDistances;
	std::shared_ptr<std::uniform_int_distribution<int>> oneThird;
	std::shared_ptr<std::uniform_real_distribution<float>> from0to1Dist;
	std::shared_ptr<std::uniform_real_distribution<float>> from0to2_3Dist;
	std::shared_ptr<std::uniform_int_distribution<int>> redSupGiantPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> redGiantPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> OclassPlanetsDist;
    std::shared_ptr<std::uniform_int_distribution<int>> BclassPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> AclassPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> FclassPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> GclassPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> KclassPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> MclassPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> brownDwarfPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> whiteDwarfPlanetsDist;
	std::shared_ptr<std::uniform_int_distribution<int>> neutronStarPlanetsDist;
	std::shared_ptr<std::discrete_distribution<int>> closerThanHabitableZoneDist;
	std::shared_ptr<std::discrete_distribution<int>> withinHabitableZoneDist;
	std::shared_ptr<std::discrete_distribution<int>> furtherThanHabitableZoneDist;
	std::shared_ptr<std::uniform_real_distribution<float>> smallRockyPlanetDist;
	std::shared_ptr<std::uniform_real_distribution<float>> mediumRockyPlanetDist;
	std::shared_ptr<std::uniform_real_distribution<float>> largeRockyPlanetDist;
	std::shared_ptr<std::uniform_real_distribution<float>> smallIcyPlanetDist;
	std::shared_ptr<std::uniform_real_distribution<float>> mediumIcyPlanetDist;
	std::shared_ptr<std::uniform_real_distribution<float>> largeIcyPlanetDist;
	std::shared_ptr<std::uniform_real_distribution<float>> smallGiantPlanetDist;
	std::shared_ptr<std::uniform_real_distribution<float>> largeGiantPlanetDist;
	std::shared_ptr<std::discrete_distribution<int>> closeOrbitMoonDist;
	std::shared_ptr<std::discrete_distribution<int>> habitableZoneMoonDist;
	std::shared_ptr<std::discrete_distribution<int>> farOrbitMoonDist;
	std::shared_ptr<std::uniform_int_distribution<int>> RingTypeDist;
	std::shared_ptr<std::discrete_distribution<int>> barrenPlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> venusLikePlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> oceanicPlanetDistrictsDist;
    std::shared_ptr<std::discrete_distribution<int>> earthLikeClosePlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> earthLikeMediumPlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> earthLikeFarPlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> titanLikePlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> moltenPlanetDistrictsDist;
    std::shared_ptr<std::discrete_distribution<int>> icyPlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> voulcanicPlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> desertClosePlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> desertMediumPlanetDistrictsDist;
	std::shared_ptr<std::discrete_distribution<int>> desertFarPlanetDistrictsDist;

	WorldGenerator() = default;

	void GenerateSystemType(std::shared_ptr<std::discrete_distribution<int>> systemTypeDist, std::shared_ptr<ObjectSystemComponent> spSystemCom, std::shared_ptr<SceneNode> wpSystemNode, std::shared_ptr<Entity> spStar1Entuty);
	void GenerateStarProperties(std::weak_ptr<StarComponent> wpStarCom, std::weak_ptr<Entity> wpEntity);
	void GeneratePlanets(std::shared_ptr<SceneNode> spSystemOrStarNode, double distanceBetweenStars, bool singleStarSystem, bool inheritPosition);
	void GenerateSinglePlanet(sf::Vector2f orbitBoundaries, sf::Vector2f habitableZoneBoundaries, int num, std::shared_ptr<SceneNode> spNode, float starMass, bool inheritPosition);
	void GenerateMoons(std::shared_ptr<PlanetComponent> spPlanet, sf::Vector2f habitableZoneBoundaries, std::shared_ptr<SceneNode> spNode);
	void CreateMoon(std::shared_ptr<std::uniform_real_distribution<float>> spMoonOrbitDist, float maxMoonSize, int orbitType, std::shared_ptr<SceneNode> spNode, int num, float mainPlanetSize, DistanceToStar habitDistToStar);
	void GenerateRings(std::shared_ptr<SceneNode> spPlanetNode, float planetSize, PlanetType planetType);
	PlanetDistrictType GetDistrictType(PlanetType planetType, std::weak_ptr<HabitablePlanetComponent> wpHabitPlanet, std::string& districtTextureName, bool generateIceCaps, float iceCapChanceMultiplier, std::mt19937& planetRandomizer);

public:

	std::shared_ptr<InputSystem> spInputSystem;

	//WorldGenerator() = delete;                    // Prevent construction
	WorldGenerator(const WorldGenerator&) = delete;         // Prevent copying
	WorldGenerator& operator=(const WorldGenerator&) = delete;

	static WorldGenerator& Instance()
	{
		static WorldGenerator worldGen;
		return worldGen;
	}

	//Getters
	int const getSeed() { return seed; }
	SpaceMapConfigurations& const getSpaceMapConfig() { return mapConfig; }
	bool const IsWorldGenerated() { return worldGenerated; }

	//Setter
	void SetWorldIsGenerated() { worldGenerated = true; }

	//Call at the begining of the game
	void Initialize(std::shared_ptr<InputSystem> spInputSystem);
	//Call every time when need to generate new world
	void ResetGenerator(int seed, SpaceMapConfigurations spaceMapConfig);

	//Min and max value are used so vector 0,3 all values 0,1,2,3 will be generated
	std::vector<int> GenerateGridOfRandomNumbers(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
	std::vector<int> GenerateGridOfTiles(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
	void GenerateSpaceMap(std::shared_ptr<SceneNode> ptrSpaceMapNode);
	void GenerateNebulas(std::shared_ptr<SceneNode> ptrNebulasNode, std::shared_ptr<SceneNode> spSystemNamesNode);
	std::shared_ptr<SceneNode> GenerateDistricts(int planetSeed, std::shared_ptr<SceneNode> spPlanetNode);
};


//To set rectangleShapes and textures for all objects and generate names
class TextureAndNameSetter : public SceneNodeVisitor
{
public:
	TextureAndNameSetter(unsigned int seed, std::weak_ptr<SceneNode> wpSystemNamesNode);

	void ProcessNode(SceneNode& node) override;

	std::weak_ptr<SceneNode> wpSpaceMapNode;

private:
	void SetSystemTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType);
	void SetSystemName(std::shared_ptr<ObjectSystemComponent> spSpaceSys, StarType starType);
	void SetStarTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType, float starSize);

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