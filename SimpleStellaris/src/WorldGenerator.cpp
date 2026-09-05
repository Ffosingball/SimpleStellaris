#include "WorldGenerator.h"
#include "EntitiesFunctions.h"
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SceneNode.h"
#include <memory>
#include "Components.h"
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "GameState.h"
#include "SpaceObjectTypes.h"
#include <unordered_map>
#include <rapidcsv.h>
#include "CompilerInstructions.h"
#include "Systems.h"
#include <functional>
#include "SceneNodeVisitors.h"
#include "GetTextureNameFunctions.h"


void WorldGenerator::Initialize(std::shared_ptr<InputSystem> spInputSystem)
{
	this->spInputSystem = spInputSystem;
	oneThird = std::make_shared<std::uniform_int_distribution<int>>(0, 2);
	from0to1Dist = std::make_shared<std::uniform_real_distribution<float>>(0.f, 1.f);
	from0to2_3Dist = std::make_shared<std::uniform_real_distribution<float>>(0.f, 2.f / 3.f);
}


void WorldGenerator::ResetGenerator(int seedIn, SpaceMapConfigurations spaceMapConfig)
{
	seed = seedIn;
	mapConfig = spaceMapConfig;
	randomizer = std::make_shared<std::mt19937>(std::mt19937{ seed });
	worldGenerated = false;
	//std::cout << "Resetting in World generator\n";
	//std::cout << mapConfig.habitableZoneWhiteDwarf.x<<"; "<< mapConfig.habitableZoneWhiteDwarf.y << '\n';

	//Create all weights
	std::vector<float> starWeights(13);
	starWeights[0] = mapConfig.redSupGiantChance;
	starWeights[1] = mapConfig.redGiantChance;
	starWeights[2] = mapConfig.classOChance;
	starWeights[3] = mapConfig.classBChance;
	starWeights[4] = mapConfig.classAChance;
	starWeights[5] = mapConfig.classFChance;
	starWeights[6] = mapConfig.classGChance;
	starWeights[7] = mapConfig.classKChance;
	starWeights[8] = mapConfig.classMChance;
	starWeights[9] = mapConfig.brownDwarfChance;
	starWeights[10] = mapConfig.whiteDwarfChance;
	starWeights[11] = mapConfig.neutronChance;
	starWeights[12] = mapConfig.blackHoleChance;

	std::vector<float> giantSystemWeights(3);
	giantSystemWeights[0] = mapConfig.giantSingleChance;
	giantSystemWeights[1] = mapConfig.giantBinaryChance;
	giantSystemWeights[2] = mapConfig.giantTernaryChance;

	std::vector<float> mediumSystemWeights(3);
	mediumSystemWeights[0] = mapConfig.mediumSingleChance;
	mediumSystemWeights[1] = mapConfig.mediumBinaryChance;
	mediumSystemWeights[2] = mapConfig.mediumTernaryChance;

	std::vector<float> dwarfsSystemWeights(3);
	dwarfsSystemWeights[0] = mapConfig.dwarfSingleChance;
	dwarfsSystemWeights[1] = mapConfig.dwarfBinaryChance;
	dwarfsSystemWeights[2] = mapConfig.dwarfTernaryChance;

	std::vector<float> binarySystemWeights(2);
	binarySystemWeights[0] = mapConfig.closeBinaryChance;
	binarySystemWeights[1] = mapConfig.afarBinaryChance;

	std::vector<float> ternarySystemWeights(2);
	ternarySystemWeights[0] = mapConfig.ternaryCloseBinaryThirdAfarChance;
	ternarySystemWeights[1] = mapConfig.afarTernaryChance;

	std::vector<float> closeOrbitWeights(4);
	closeOrbitWeights[0] = mapConfig.closeOrbitVenusLikeChance;
	closeOrbitWeights[1] = mapConfig.closeOrbitBarrenChance;
	closeOrbitWeights[2] = mapConfig.closeOrbitHotJupiterChance;
	closeOrbitWeights[3] = mapConfig.closeOrbitHotNeptuneChance;

	std::vector<float> mediumOrbitWeights(7);
	mediumOrbitWeights[0] = mapConfig.mediumOrbitVenusLikeChance;
	mediumOrbitWeights[1] = mapConfig.mediumOrbitOceanicChance;
	mediumOrbitWeights[2] = mapConfig.mediumOrbitEarthLikeChance;
	mediumOrbitWeights[3] = mapConfig.mediumOrbitDesertChance;
	mediumOrbitWeights[4] = mapConfig.mediumOrbitBarrenChance;
	mediumOrbitWeights[5] = mapConfig.mediumOrbitHotJupiterChance;
	mediumOrbitWeights[6] = mapConfig.mediumOrbitHotNeptuneChance;

	std::vector<float> afarOrbitWeights(4);
	afarOrbitWeights[0] = mapConfig.afarOrbitIcyChance;
	afarOrbitWeights[1] = mapConfig.afarOrbitBarrenChance;
	afarOrbitWeights[2] = mapConfig.afarOrbitNeptuneLikeChance;
	afarOrbitWeights[3] = mapConfig.afarOrbitJupiterLikeChance;

	std::vector<float> closeOrbitMoonWeights(3);
	closeOrbitMoonWeights[0] = mapConfig.closerThanHabitBarrenMoonChance;
	closeOrbitMoonWeights[1] = mapConfig.closerThanHabitVoulcanicMoonChance;
	closeOrbitMoonWeights[2] = mapConfig.closerThanHabitVenusLikeMoonChance;

	std::vector<float> farOrbitMoonWeights(4);
	farOrbitMoonWeights[0] = mapConfig.furtherThanHabitBarrenMoonChance;
	farOrbitMoonWeights[1] = mapConfig.furtherThanHabitVoulcanicMoonChance;
	farOrbitMoonWeights[2] = mapConfig.furtherThanHabitTitanLikeMoonChance;
	farOrbitMoonWeights[3] = mapConfig.furtherThanHabitIcyMoonChance;

	std::vector<float> habitableZoneOrbitMoonWeights(6);
	habitableZoneOrbitMoonWeights[0] = mapConfig.habitZoneBarrenMoonChance;
	habitableZoneOrbitMoonWeights[1] = mapConfig.habitZoneOceanMoonChance;
	habitableZoneOrbitMoonWeights[2] = mapConfig.habitZoneVenusLikeMoonChance;
	habitableZoneOrbitMoonWeights[3] = mapConfig.habitZoneEarthLikeMoonChance;
	habitableZoneOrbitMoonWeights[4] = mapConfig.habitZoneDesertMoonChance;
	habitableZoneOrbitMoonWeights[5] = mapConfig.habitZoneVoulcanicMoonChance;

	std::vector<float> barrenPlanetDistrictsWeights(4);
	barrenPlanetDistrictsWeights[0] = mapConfig.barrenPlanetBarrenDistrictChance;
	barrenPlanetDistrictsWeights[1] = mapConfig.barrenPlanetCratorDistrictChance;
	barrenPlanetDistrictsWeights[2] = mapConfig.barrenPlanetMountainsDistrictChance;
	barrenPlanetDistrictsWeights[3] = mapConfig.barrenPlanetDesertDistrictChance;

	std::vector<float> venusLikePlanetDistrictsWeights(3);
	venusLikePlanetDistrictsWeights[0] = mapConfig.venusLikePlanetVoulcanoDistrictChance;
	venusLikePlanetDistrictsWeights[1] = mapConfig.venusLikePlanetMountainsDistrictChance;
	venusLikePlanetDistrictsWeights[2] = mapConfig.venusLikePlanetToxicLandDistrictChance;

	std::vector<float> oceanicPlanetDistrictsWeights(2);
	oceanicPlanetDistrictsWeights[0] = mapConfig.oceanicPlanetOceanDistrictChance;
	oceanicPlanetDistrictsWeights[1] = mapConfig.oceanicPlanetIslandsDistrictChance;

	std::vector<float> earthLikeClosePlanetDistrictsWeights(6);
	earthLikeClosePlanetDistrictsWeights[0] = mapConfig.earthLikeClosePlanetMountainsDistrictChance;
	earthLikeClosePlanetDistrictsWeights[1] = mapConfig.earthLikeClosePlanetOceanDistrictChance;
	earthLikeClosePlanetDistrictsWeights[2] = mapConfig.earthLikeClosePlanetVoulcanoDistrictChance;
	earthLikeClosePlanetDistrictsWeights[3] = mapConfig.earthLikeClosePlanetDesertDistrictChance;
	earthLikeClosePlanetDistrictsWeights[4] = mapConfig.earthLikeClosePlanetRainforestDistrictChance;
	earthLikeClosePlanetDistrictsWeights[5] = mapConfig.earthLikeClosePlanetSteppeDistrictChance;

	std::vector<float> earthLikeMediumPlanetDistrictsWeights(7);
	earthLikeMediumPlanetDistrictsWeights[0] = mapConfig.earthLikeMediumPlanetMountainsDistrictChance;
	earthLikeMediumPlanetDistrictsWeights[1] = mapConfig.earthLikeMediumPlanetOceanDistrictChance;
	earthLikeMediumPlanetDistrictsWeights[2] = mapConfig.earthLikeMediumPlanetVoulcanoDistrictChance;
	earthLikeMediumPlanetDistrictsWeights[3] = mapConfig.earthLikeMediumPlanetDesertDistrictChance;
	earthLikeMediumPlanetDistrictsWeights[4] = mapConfig.earthLikeMediumPlanetForestDistrictChance;
	earthLikeMediumPlanetDistrictsWeights[5] = mapConfig.earthLikeMediumPlanetRainforestDistrictChance;
	earthLikeMediumPlanetDistrictsWeights[6] = mapConfig.earthLikeMediumPlanetSteppeDistrictChance;

	std::vector<float> earthLikeFarPlanetDistrictsWeights(6);
	earthLikeFarPlanetDistrictsWeights[0] = mapConfig.earthLikeFarPlanetMountainsDistrictChance;
	earthLikeFarPlanetDistrictsWeights[1] = mapConfig.earthLikeFarPlanetOceanDistrictChance;
	earthLikeFarPlanetDistrictsWeights[2] = mapConfig.earthLikeFarPlanetVoulcanoDistrictChance;
	earthLikeFarPlanetDistrictsWeights[3] = mapConfig.earthLikeFarPlanetDesertDistrictChance;
	earthLikeFarPlanetDistrictsWeights[4] = mapConfig.earthLikeFarPlanetForestDistrictChance;
	earthLikeFarPlanetDistrictsWeights[5] = mapConfig.earthLikeFarPlanetSteppeDistrictChance;

	std::vector<float> titanLikePlanetDistrictsWeights(4);
	titanLikePlanetDistrictsWeights[0] = mapConfig.titanLikePlanetBarrenDistrictChance;
	titanLikePlanetDistrictsWeights[1] = mapConfig.titanLikePlanetMountainsDistrictChance;
	titanLikePlanetDistrictsWeights[2] = mapConfig.titanLikePlanetMethanOceanDistrictChance;
	titanLikePlanetDistrictsWeights[3] = mapConfig.titanLikePlanetVoulcanoDistrictChance;

	std::vector<float> icyPlanetDistrictsWeights(4);
	icyPlanetDistrictsWeights[0] = mapConfig.icyPlanetIceSheetDistrictChance;
	icyPlanetDistrictsWeights[1] = mapConfig.icyPlanetBarrenDistrictChance;
	icyPlanetDistrictsWeights[2] = mapConfig.icyPlanetMountainsDistrictChance;
	icyPlanetDistrictsWeights[3] = mapConfig.icyPlanetVoulcanoDistrictChance;

	std::vector<float> voulcanicPlanetDistrictsWeights(2);
	voulcanicPlanetDistrictsWeights[0] = mapConfig.voulcanicPlanetVoulcanoDistrictChance;
	voulcanicPlanetDistrictsWeights[1] = mapConfig.voulcanicPlanetBarrenDistrictChance;

	std::vector<float> moltenPlanetDistrictsWeights(2);
	moltenPlanetDistrictsWeights[0] = mapConfig.moltenPlanetVoulcanoDistrictChance;
	moltenPlanetDistrictsWeights[1] = mapConfig.moltenPlanetBarrenDistrictChance;

	std::vector<float> desertClosePlanetDistrictsWeights(5);
	desertClosePlanetDistrictsWeights[0] = mapConfig.desertPlanetCloseDesertDistrictChance;
	desertClosePlanetDistrictsWeights[1] = mapConfig.desertPlanetCloseBarrenDistrictChance;
	desertClosePlanetDistrictsWeights[2] = mapConfig.desertPlanetCloseSteppeDistrictChance;
	desertClosePlanetDistrictsWeights[3] = mapConfig.desertPlanetCloseMountainsDistrictChance;
	desertClosePlanetDistrictsWeights[4] = mapConfig.desertPlanetCloseVoulcanoDistrictChance;

	std::vector<float> desertMediumPlanetDistrictsWeights(5);
	desertMediumPlanetDistrictsWeights[0] = mapConfig.desertPlanetMediumDesertDistrictChance;
	desertMediumPlanetDistrictsWeights[1] = mapConfig.desertPlanetMediumBarrenDistrictChance;
	desertMediumPlanetDistrictsWeights[2] = mapConfig.desertPlanetMediumSteppeDistrictChance;
	desertMediumPlanetDistrictsWeights[3] = mapConfig.desertPlanetMediumMountainsDistrictChance;
	desertMediumPlanetDistrictsWeights[4] = mapConfig.desertPlanetMediumVoulcanoDistrictChance;

	std::vector<float> desertFarPlanetDistrictsWeights(5);
	desertFarPlanetDistrictsWeights[0] = mapConfig.desertPlanetFarDesertDistrictChance;
	desertFarPlanetDistrictsWeights[1] = mapConfig.desertPlanetFarBarrenDistrictChance;
	desertFarPlanetDistrictsWeights[2] = mapConfig.desertPlanetFarSteppeDistrictChance;
	desertFarPlanetDistrictsWeights[3] = mapConfig.desertPlanetFarMountainsDistrictChance;
	desertFarPlanetDistrictsWeights[4] = mapConfig.desertPlanetFarVoulcanoDistrictChance;

	//Create all distributions
	starDistribution = std::make_shared<std::discrete_distribution<int>>(starWeights.begin(), starWeights.end());
	giantSysDistribution = std::make_shared<std::discrete_distribution<int>>(giantSystemWeights.begin(), giantSystemWeights.end());
	mediumSysDistribution = std::make_shared<std::discrete_distribution<int>>(mediumSystemWeights.begin(), mediumSystemWeights.end());
	dwarfSysDistribution = std::make_shared<std::discrete_distribution<int>>(dwarfsSystemWeights.begin(), dwarfsSystemWeights.end());
	binarySysDistribution = std::make_shared<std::discrete_distribution<int>>(binarySystemWeights.begin(), binarySystemWeights.end());
	ternarySysDistribution = std::make_shared<std::discrete_distribution<int>>(ternarySystemWeights.begin(), ternarySystemWeights.end());
	closeStarsDistances = std::make_shared<std::uniform_real_distribution<double>>(static_cast<double>(mapConfig.closeStarsBoundaries.x), static_cast<double>(mapConfig.closeStarsBoundaries.y));
	afarStarsDistances = std::make_shared<std::uniform_real_distribution<double>>(static_cast<double>(mapConfig.afarStarsBoundaries.x), static_cast<double>(mapConfig.afarStarsBoundaries.y));
	redSupGiantPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountRedSupergiant.x, mapConfig.planetsAmountRedSupergiant.y);
	redGiantPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountRedGiant.x, mapConfig.planetsAmountRedGiant.y);
	OclassPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountClassO.x, mapConfig.planetsAmountClassO.y);
	BclassPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountClassB.x, mapConfig.planetsAmountClassB.y);
	AclassPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountClassA.x, mapConfig.planetsAmountClassA.y);
	FclassPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountClassF.x, mapConfig.planetsAmountClassF.y);
	GclassPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountClassG.x, mapConfig.planetsAmountClassG.y);
	KclassPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountClassK.x, mapConfig.planetsAmountClassK.y);
	MclassPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountClassM.x, mapConfig.planetsAmountClassM.y);
	brownDwarfPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountBrownDwarf.x, mapConfig.planetsAmountBrownDwarf.y);
	whiteDwarfPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountWhiteDwarf.x, mapConfig.planetsAmountWhiteDwarf.y);
	neutronStarPlanetsDist = std::make_shared<std::uniform_int_distribution<int>>(mapConfig.planetsAmountNeutronStar.x, mapConfig.planetsAmountNeutronStar.y);
	closerThanHabitableZoneDist = std::make_shared<std::discrete_distribution<int>>(closeOrbitWeights.begin(), closeOrbitWeights.end());
	withinHabitableZoneDist = std::make_shared<std::discrete_distribution<int>>(mediumOrbitWeights.begin(), mediumOrbitWeights.end());
	furtherThanHabitableZoneDist = std::make_shared<std::discrete_distribution<int>>(afarOrbitWeights.begin(), afarOrbitWeights.end());
	smallRockyPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.smallRockyPlanetSizes.x, mapConfig.smallRockyPlanetSizes.y);
	mediumRockyPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.mediumRockyPlanetSizes.x, mapConfig.mediumRockyPlanetSizes.y);
	largeRockyPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.largeRockyPlanetSizes.x, mapConfig.largeRockyPlanetSizes.y);
	smallIcyPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.smallIcyPlanetSizes.x, mapConfig.smallIcyPlanetSizes.y);
	mediumIcyPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.mediumIcyPlanetSizes.x, mapConfig.mediumIcyPlanetSizes.y);
	largeIcyPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.largeIcyPlanetSizes.x, mapConfig.largeIcyPlanetSizes.y);
	largeGiantPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.largeGasSizes.x, mapConfig.largeGasSizes.y);
	smallGiantPlanetDist = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.smallGasSizes.x, mapConfig.smallGasSizes.y);
	closeOrbitMoonDist = std::make_shared<std::discrete_distribution<int>>(closeOrbitMoonWeights.begin(), closeOrbitMoonWeights.end());
	habitableZoneMoonDist = std::make_shared<std::discrete_distribution<int>>(habitableZoneOrbitMoonWeights.begin(), habitableZoneOrbitMoonWeights.end());
	farOrbitMoonDist = std::make_shared<std::discrete_distribution<int>>(farOrbitMoonWeights.begin(), farOrbitMoonWeights.end());
	RingTypeDist = std::make_shared<std::uniform_int_distribution<int>>(0, mapConfig.numOfAvailableRingTextures - 1);
	barrenPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(barrenPlanetDistrictsWeights.begin(), barrenPlanetDistrictsWeights.end());
	venusLikePlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(venusLikePlanetDistrictsWeights.begin(), venusLikePlanetDistrictsWeights.end());
	oceanicPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(oceanicPlanetDistrictsWeights.begin(), oceanicPlanetDistrictsWeights.end());
	earthLikeClosePlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(earthLikeClosePlanetDistrictsWeights.begin(), earthLikeClosePlanetDistrictsWeights.end());
	earthLikeMediumPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(earthLikeMediumPlanetDistrictsWeights.begin(), earthLikeMediumPlanetDistrictsWeights.end());
	earthLikeFarPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(earthLikeFarPlanetDistrictsWeights.begin(), earthLikeFarPlanetDistrictsWeights.end());
	titanLikePlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(titanLikePlanetDistrictsWeights.begin(), titanLikePlanetDistrictsWeights.end());
	moltenPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(moltenPlanetDistrictsWeights.begin(), moltenPlanetDistrictsWeights.end());
	icyPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(icyPlanetDistrictsWeights.begin(), icyPlanetDistrictsWeights.end());
	voulcanicPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(voulcanicPlanetDistrictsWeights.begin(), voulcanicPlanetDistrictsWeights.end());
	desertClosePlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(desertClosePlanetDistrictsWeights.begin(), desertClosePlanetDistrictsWeights.end());
	desertMediumPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(desertMediumPlanetDistrictsWeights.begin(), desertMediumPlanetDistrictsWeights.end());
	desertFarPlanetDistrictsDist = std::make_shared<std::discrete_distribution<int>>(desertFarPlanetDistrictsWeights.begin(), desertFarPlanetDistrictsWeights.end());
}



std::vector<int> WorldGenerator::GenerateGridOfTiles(sf::Vector2i gridSize, sf::Vector2i minMaxValues)
{
	//Here i assumed that only first 6 tiles has nebulas and i want tiles with nebulas to generate
	//more rarely than usual ones
	std::vector<int> randomNumbers(gridSize.x * gridSize.y);

	std::uniform_int_distribution nextNumber{ minMaxValues.x, minMaxValues.y };
	std::uniform_real_distribution nebulaChance{ 0.f, mapConfig.nebulaRarenessBackground };
	std::uniform_int_distribution randomStep{ mapConfig.numberOfNebulasBackground, minMaxValues.y - mapConfig.numberOfNebulasBackground };
	for (int& val : randomNumbers)
	{
		int nextNum = nextNumber(*randomizer);
		if (nextNum < mapConfig.numberOfNebulasBackground)
		{
			if (nebulaChance(*randomizer) > 1.f)
			{
				nextNum += randomStep(*randomizer);
			}
		}
		val = nextNum;
	}

	return randomNumbers;
}


std::vector<int> WorldGenerator::GenerateGridOfRandomNumbers(sf::Vector2i gridSize, sf::Vector2i minMaxValues) 
{
	std::vector<int> randomNumbers(gridSize.x*gridSize.y);

	std::uniform_int_distribution nextNumber{ minMaxValues.x, minMaxValues.y };
	for (int& val : randomNumbers) 
	{
		val = nextNumber(*randomizer);
	}

	return randomNumbers;
}


PlanetDistrictType WorldGenerator::GetDistrictType(PlanetType planetType, std::weak_ptr<HabitablePlanetComponent> wpHabitPlanet, std::string& districtTextureName, bool generateIceCaps, float iceCapChanceMultiplier, std::mt19937& planetRandomizer)
{
	int val{ -1 };
	switch (planetType)
	{
	case PlanetType::BarrenGrey:
		val = (*barrenPlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "BarrenGreyBarrenDistrict";
			return PlanetDistrictType::Barren;
		}
		else if (val == 1)
		{
			districtTextureName = "BarrenGreyCratorDistrict";
			return PlanetDistrictType::Crator;
		}
		else if (val == 2)
		{
			districtTextureName = "BarrenGreyMountainsDistrict";
			return PlanetDistrictType::Mountains;
		}
		else if (val == 3)
		{
			districtTextureName = "BarrenGreyDesertDistrict";
			return PlanetDistrictType::Desert;
		}

		return PlanetDistrictType::None;
	case PlanetType::BarrenDark:
		val = (*barrenPlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "BarrenDarkBarrenDistrict";
			return PlanetDistrictType::Barren;
		}
		else if (val == 1)
		{
			districtTextureName = "BarrenDarkCratorDistrict";
			return PlanetDistrictType::Crator;
		}
		else if (val == 2)
		{
			districtTextureName = "BarrenDarkMountainsDistrict";
			return PlanetDistrictType::Mountains;
		}
		else if (val == 3)
		{
			districtTextureName = "BarrenDarkDesertDistrict";
			return PlanetDistrictType::Desert;
		}

		return PlanetDistrictType::None;
	case PlanetType::BarrenMarsLike:
		val = (*barrenPlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "BarrenRedBarrenDistrict";
			return PlanetDistrictType::Barren;
		}
		else if (val == 1)
		{
			districtTextureName = "BarrenRedCratorDistrict";
			return PlanetDistrictType::Crator;
		}
		else if (val == 2)
		{
			districtTextureName = "BarrenRedMountainsDistrict";
			return PlanetDistrictType::Mountains;
		}
		else if (val == 3)
		{
			districtTextureName = "BarrenRedDesertDistrict";
			return PlanetDistrictType::Desert;
		}

		return PlanetDistrictType::None;
	case PlanetType::VenusLike:
		val = (*venusLikePlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "VenusLikeVoulcanoDistrict";
			return PlanetDistrictType::Voulcano;
		}
		else if (val == 1)
		{
			districtTextureName = "VenusLikeMountainsDistrict";
			return PlanetDistrictType::Mountains;
		}
		else if (val == 2)
		{
			districtTextureName = "VenusLikeBarrenDistrict";
			return PlanetDistrictType::Barren;
		}

		return PlanetDistrictType::None;
	case PlanetType::Oceanic:
		if (generateIceCaps)
		{
			if (wpHabitPlanet.lock()->distanceToStar == DistanceToStar::Close)
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.oceanicClosePlanetIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}
			else if (wpHabitPlanet.lock()->distanceToStar == DistanceToStar::Medium)
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.oceanicMediumPlanetIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}
			else
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.oceanicFarPlanetIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}
		}

		val = (*oceanicPlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "OceanDistrict";
			return PlanetDistrictType::Ocean;
		}
		else if (val == 1)
		{
			districtTextureName = "IslandsDistrict";
			return PlanetDistrictType::Islands;
		}

		return PlanetDistrictType::None;
	case PlanetType::EarthLike:
		if (wpHabitPlanet.lock()->distanceToStar == DistanceToStar::Close)
		{
			if (generateIceCaps) 
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.earthLikeClosePlanetIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}

			val = (*earthLikeClosePlanetDistrictsDist)(planetRandomizer);
			if (val == 0)
			{
				districtTextureName = "EarthLikeMountainsDistrict";
				return PlanetDistrictType::Mountains;
			}
			else if (val == 1)
			{
				districtTextureName = "OceanDistrict";
				return PlanetDistrictType::Ocean;
			}
			else if (val == 2)
			{
				districtTextureName = "EarthLikeVoulcanoDistrict";
				return PlanetDistrictType::Voulcano;
			}
			else if (val == 3)
			{
				districtTextureName = "DesertDistrict";
				return PlanetDistrictType::Desert;
			}
			else if (val == 4)
			{
				districtTextureName = "CloseRainforestDistrict";
				return PlanetDistrictType::Rainforest;
			}
			else if (val == 5)
			{
				districtTextureName = "EarthLikeCloseSteppeDistrict";
				return PlanetDistrictType::Steppe;
			}
		}
		else if (wpHabitPlanet.lock()->distanceToStar == DistanceToStar::Medium)
		{
			if (generateIceCaps)
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.earthLikeMediumPlanetIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}

			val = (*earthLikeMediumPlanetDistrictsDist)(planetRandomizer);
			if (val == 0)
			{
				districtTextureName = "EarthLikeMountainsDistrict";
				return PlanetDistrictType::Mountains;
			}
			else if (val == 1)
			{
				districtTextureName = "OceanDistrict";
				return PlanetDistrictType::Ocean;
			}
			else if (val == 2)
			{
				districtTextureName = "EarthLikeVoulcanoDistrict";
				return PlanetDistrictType::Voulcano;
			}
			else if (val == 3)
			{
				districtTextureName = "DesertDistrict";
				return PlanetDistrictType::Desert;
			}
			else if (val == 4)
			{
				districtTextureName = "MediumForestDistrict";
				return PlanetDistrictType::Forest;
			}
			else if (val == 5)
			{
				districtTextureName = "MediumRainforestDistrict";
				return PlanetDistrictType::Rainforest;
			}
			else if (val == 6)
			{
				districtTextureName = "SteppeDistrict";
				return PlanetDistrictType::Steppe;
			}
		}
		else
		{
			if (generateIceCaps)
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.earthLikeFarPlanetIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}

			val = (*earthLikeFarPlanetDistrictsDist)(planetRandomizer);
			if (val == 0)
			{
				districtTextureName = "EarthLikeMountainsDistrict";
				return PlanetDistrictType::Mountains;
			}
			else if (val == 1)
			{
				districtTextureName = "OceanDistrict";
				return PlanetDistrictType::Ocean;
			}
			else if (val == 2)
			{
				districtTextureName = "EarthLikeVoulcanoDistrict";
				return PlanetDistrictType::Voulcano;
			}
			else if (val == 3)
			{
				districtTextureName = "DesertDistrict";
				return PlanetDistrictType::Desert;
			}
			else if (val == 4)
			{
				districtTextureName = "FarForestDistrict";
				return PlanetDistrictType::Forest;
			}
			else if (val == 5)
			{
				districtTextureName = "EarthLikeFarSteppeDistrict";
				return PlanetDistrictType::Steppe;
			}
		}

		return PlanetDistrictType::None;
	case PlanetType::TitanLike:
		val = (*titanLikePlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "TitanLikeBarrenDistrict";
			return PlanetDistrictType::Barren;
		}
		else if (val == 1)
		{
			districtTextureName = "TitanLikeMountainsDistrict";
			return PlanetDistrictType::Mountains;
		}
		else if (val == 2)
		{
			districtTextureName = "MethanOceanDistrict";
			return PlanetDistrictType::MethanOcean;
		}
		else if (val == 3)
		{
			districtTextureName = "TitanLikeVoulcanoDistrict";
			return PlanetDistrictType::Voulcano;
		}

		return PlanetDistrictType::None;
	case PlanetType::Molten:
		val = (*moltenPlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "MoltenVoulcanoDistrict";
			return PlanetDistrictType::Voulcano;
		}
		else if (val == 1)
		{
			districtTextureName = "MoltenLandDistrict";
			return PlanetDistrictType::MoltenLand;
		}

		return PlanetDistrictType::None;
	case PlanetType::Icy:
		val = (*icyPlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "IceSheetDistrict";
			return PlanetDistrictType::IceSheet;
		}
		else if (val == 1)
		{
			districtTextureName = "IcyBarrenDistrict";
			return PlanetDistrictType::Barren;
		}
		else if (val == 2)
		{
			districtTextureName = "IcyMountainsDistrict";
			return PlanetDistrictType::Mountains;
		}
		else if (val == 3)
		{
			districtTextureName = "IcyVoulcanoDistrict";
			return PlanetDistrictType::Voulcano;
		}

		return PlanetDistrictType::None;
	case PlanetType::Voulcanic:
		val = (*voulcanicPlanetDistrictsDist)(planetRandomizer);
		if (val == 0)
		{
			districtTextureName = "VoulcanicVoulcanoDistrict";
			return PlanetDistrictType::Voulcano;
		}
		else if (val == 1)
		{
			districtTextureName = "VoulcanicBarrenDistrict";
			return PlanetDistrictType::Barren;
		}

		return PlanetDistrictType::None;
	case PlanetType::Desert:
		if (wpHabitPlanet.lock()->distanceToStar == DistanceToStar::Close)
		{
			if (generateIceCaps)
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.desertPlanetCloseIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}

			val = (*desertClosePlanetDistrictsDist)(planetRandomizer);
			if (val == 0)
			{
				districtTextureName = "DesertDistrict";
				return PlanetDistrictType::Desert;
			}
			else if (val == 1)
			{
				districtTextureName = "DesertBarrenDistrict";
				return PlanetDistrictType::Barren;
			}
			else if (val == 2)
			{
				districtTextureName = "EarthLikeCloseSteppeDistrict";
				return PlanetDistrictType::Steppe;
			}
			else if (val == 3)
			{
				districtTextureName = "DesertMountainsDistrict";
				return PlanetDistrictType::Mountains;
			}
			else if (val == 4)
			{
				districtTextureName = "DesertVoulcanoDistrict";
				return PlanetDistrictType::Voulcano;
			}
		}
		else if (wpHabitPlanet.lock()->distanceToStar == DistanceToStar::Medium)
		{
			if (generateIceCaps)
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.desertPlanetMediumIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}

			val = (*desertMediumPlanetDistrictsDist)(planetRandomizer);
			if (val == 0)
			{
				districtTextureName = "DesertDistrict";
				return PlanetDistrictType::Desert;
			}
			else if (val == 1)
			{
				districtTextureName = "DesertBarrenDistrict";
				return PlanetDistrictType::Barren;
			}
			else if (val == 2)
			{
				districtTextureName = "SteppeDistrict";
				return PlanetDistrictType::Steppe;
			}
			else if (val == 3)
			{
				districtTextureName = "DesertMountainsDistrict";
				return PlanetDistrictType::Mountains;
			}
			else if (val == 4)
			{
				districtTextureName = "DesertVoulcanoDistrict";
				return PlanetDistrictType::Voulcano;
			}
		}
		else
		{
			if (generateIceCaps)
			{
				if ((*from0to1Dist)(planetRandomizer) < mapConfig.desertPlanetFarIceSheetAtThePolesChance * iceCapChanceMultiplier)
				{
					districtTextureName = "IceSheetDistrict";
					return PlanetDistrictType::IceSheet;
				}
			}

			val = (*desertFarPlanetDistrictsDist)(planetRandomizer);
			if (val == 0)
			{
				districtTextureName = "DesertDistrict";
				return PlanetDistrictType::Desert;
			}
			else if (val == 1)
			{
				districtTextureName = "DesertBarrenDistrict";
				return PlanetDistrictType::Barren;
			}
			else if (val == 2)
			{
				districtTextureName = "EarthLikeFarSteppeDistrict";
				return PlanetDistrictType::Steppe;
			}
			else if (val == 3)
			{
				districtTextureName = "DesertMountainsDistrict";
				return PlanetDistrictType::Mountains;
			}
			else if (val == 4)
			{
				districtTextureName = "DesertVoulcanoDistrict";
				return PlanetDistrictType::Voulcano;
			}
		}

		return PlanetDistrictType::None;
	}

	return PlanetDistrictType::None;
}



std::shared_ptr<SceneNode> WorldGenerator::GenerateDistricts(int planetSeed, std::shared_ptr<SceneNode> spPlanetNode)
{
	std::shared_ptr<Entity> spEntity = spPlanetNode->GetEntity().lock();
	std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
	std::weak_ptr<HabitablePlanetComponent> wpHabitPlanet = spEntity->FindComponent<HabitablePlanetComponent>();
	std::mt19937 planetRandomizer(planetSeed);
	float uiSize = ECSGame::Instance().GetUISize();

	//Calculate how many districts a planet will have
	int numOfDistricts = (int)(spPlanetCom->planetSize * mapConfig.districtsAmount);

	//Calculate number of rows
	int numOfRows = 1;
	while (numOfRows * ((numOfRows * 2) - 1) < numOfDistricts)
	{
		numOfRows++;
	}
	int numOfColumns = numOfDistricts / numOfRows;

	//Create node which will store all districts nodes
	std::shared_ptr<Entity> spNewEn = ECSGame::Instance().GetEntityManager().NewEntity("Node").lock();
	spNewEn->SetPosition(sf::Vector2f{ mapConfig.districtPosOffset.x -(numOfColumns*mapConfig.distanceBetweenDistricts.x/2), mapConfig.districtPosOffset.y - (numOfRows * mapConfig.distanceBetweenDistricts.y / 2) } * uiSize);
	std::shared_ptr<SceneNode> spNewNode = std::make_shared<SceneNode>(spNewEn);

	//Create districts
	int extraDistricts = numOfDistricts % numOfRows;
	int currentRow = 0;
	bool generateIceCaps{false};
	float iceCapChanceMul = 0.f;
	std::shared_ptr<InputSystem> spInputSys = spInputSystem;
	while (currentRow < numOfRows) 
	{
		//Check if ice cap should be generated on this row or not
		if (wpHabitPlanet.lock() != nullptr)
		{
			if (currentRow == 0 || currentRow == numOfRows - 1)
			{
				if (numOfRows > 3) 
				{
					generateIceCaps = true;
					iceCapChanceMul = 1.f;
				}
				else if (numOfRows > 2) 
				{
					generateIceCaps = true;
					iceCapChanceMul = 0.5f;
				}
				else
					generateIceCaps = false;
			}
			else
				generateIceCaps = false;
		}
		else
			generateIceCaps = false;

		int currentColumn = 0;
		while ((currentColumn < numOfColumns) || (currentColumn==numOfColumns && extraDistricts>currentRow)) 
		{
			//Create district at this planet
			std::shared_ptr<Entity> spDistrict = CreateNewEntityAt(spNewNode, "District"+std::to_string(currentRow*numOfColumns+currentColumn)).lock();
			spDistrict->SetPosition(sf::Vector2f(mapConfig.distanceBetweenDistricts.x*currentColumn, mapConfig.distanceBetweenDistricts.y * currentRow) * uiSize);
			
			std::string districtTextureName;
			std::shared_ptr<DistrictComponent> spDistrictCom = spDistrict->AddComponent<DistrictComponent>().lock();
			spDistrictCom->districtType = GetDistrictType(spPlanetCom->planetType, wpHabitPlanet, districtTextureName, generateIceCaps, iceCapChanceMul, planetRandomizer);
			spDistrictCom->districtID = currentRow * numOfColumns + currentColumn;

			std::shared_ptr<RectangleShapeComponent> spRectShapeCom = spDistrict->AddComponent<RectangleShapeComponent>().lock();
			SetupRectangleShape(spRectShapeCom, mapConfig.districtSize*uiSize, districtTextureName);
			
			std::shared_ptr<ButtonComponent> spButtonCom = spDistrict->AddComponent<ButtonComponent>().lock();
			spButtonCom->buttonSize = mapConfig.districtSize*uiSize;
			spButtonCom->onButtonHovered = [spInputSys](std::shared_ptr<Entity> entity)
				{
					spInputSys->DistrictHovered(entity);
				};
			spButtonCom->onButtonUnhovered = [spInputSys](std::shared_ptr<Entity> entity)
				{
					spInputSys->DistrictUnhovered(entity);
				};
			currentColumn++;
		}
		currentRow++;
	}

	return spNewNode;
}


void WorldGenerator::GenerateStarProperties(std::weak_ptr<StarComponent> wpStarCom, std::weak_ptr<Entity> wpEntity) 
{
	std::shared_ptr<StarComponent> spStarCom = wpStarCom.lock();
	std::weak_ptr<NeutronStarComponent> wpNS;

	//std::cout << mapConfig.habitableZoneClassG.x << "; " << mapConfig.habitableZoneClassG.y << '\n';

	switch ((*starDistribution)(*randomizer))
	{
	case 0:
		spStarCom->starType = StarType::RedSupergiant;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeRedSupergiant.x, mapConfig.starSizeRedSupergiant.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize/20.f, mapConfig.starMassPower);
		break;
	case 1:
		spStarCom->starType = StarType::RedGiant;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeRedGiant.x, mapConfig.starSizeRedGiant.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize/20.f, mapConfig.starMassPower);
		break;
	case 2:
		spStarCom->starType = StarType::Otype;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeClassO.x, mapConfig.starSizeClassO.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 3:
		spStarCom->starType = StarType::Btype;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeClassB.x, mapConfig.starSizeClassB.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 4:
		spStarCom->starType = StarType::Atype;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeClassA.x, mapConfig.starSizeClassA.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 5:
		spStarCom->starType = StarType::Ftype;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeClassF.x, mapConfig.starSizeClassF.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 6:
		spStarCom->starType = StarType::GsunLike;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeClassG.x, mapConfig.starSizeClassG.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 7:
		spStarCom->starType = StarType::KorangeDwarf;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeClassK.x, mapConfig.starSizeClassK.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 8:
		spStarCom->starType = StarType::MredDwarf;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeClassM.x, mapConfig.starSizeClassM.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 9:
		spStarCom->starType = StarType::BrownDwarf;
		spStarCom->starSize = gel::linearInterpolation(mapConfig.starSizeBrownDwarf.x, mapConfig.starSizeBrownDwarf.y, (*from0to1Dist)(*randomizer));
		spStarCom->starMass = std::pow(spStarCom->starSize, mapConfig.starMassPower);
		break;
	case 10:
		spStarCom->starType = StarType::WhiteDwarf;
		spStarCom->starSize = 0.01f;
		spStarCom->starMass = gel::linearInterpolation(mapConfig.starMassWhiteDwarf.x, mapConfig.starMassWhiteDwarf.y, (*from0to1Dist)(*randomizer));
		break;
	case 11:
		spStarCom->starType = StarType::NeutronStar;
		spStarCom->starSize = 0.001f;
		spStarCom->starMass = gel::linearInterpolation(mapConfig.starMassNeutronStar.x, mapConfig.starMassNeutronStar.y, (*from0to1Dist)(*randomizer));
		
		wpNS = wpEntity.lock()->AddComponent<NeutronStarComponent>().lock();
		wpNS.lock()->pulsarNum = gel::RandInt(0, mapConfig.numOfPulsarMusic);
		break;
	case 12:
		spStarCom->starType = StarType::BlackHole;
		spStarCom->starSize = 0.0001f;
		spStarCom->starMass = gel::linearInterpolation(mapConfig.starMassBlackHole.x, mapConfig.starMassBlackHole.y, (*from0to1Dist)(*randomizer));
		break;
	}
}



void CalculateBinarySystemProperties(std::shared_ptr<Entity> star1Sp, std::shared_ptr<Entity> star2Sp, double distanceBetweenStars, float randomPosition, std::shared_ptr<SceneNode> ptrSystemNode)
{
	std::shared_ptr<StarComponent> spStar1Com = star1Sp->FindComponent<StarComponent>().lock();
	std::shared_ptr<StarComponent> spStar2Com = star2Sp->FindComponent<StarComponent>().lock();

	float mass1{0.f};
	if (star1Sp->HasComponent<ObjectSystemComponent>())
	{
		std::shared_ptr<SceneNode> insideSysSp = ptrSystemNode->FindChild(*star1Sp).lock();
		std::vector<std::shared_ptr<SceneNode>> children = insideSysSp->GetAllChildren();
		for (std::shared_ptr<SceneNode> child : children) 
		{
			if (child->GetEntity().lock()->HasComponent<StarComponent>())
				mass1 += child->GetEntity().lock()->FindComponent<StarComponent>().lock()->starMass;
		}
	}
	else
		mass1 = spStar1Com->starMass;


	float mass2{ 0.f };
	if (star2Sp->HasComponent<ObjectSystemComponent>())
	{
		std::shared_ptr<SceneNode> insideSysSp = ptrSystemNode->FindChild(*star2Sp).lock();
		std::vector<std::shared_ptr<SceneNode>> children = insideSysSp->GetAllChildren();
		for (std::shared_ptr<SceneNode> child : children)
		{
			if (child->GetEntity().lock()->HasComponent<StarComponent>())
				mass2 += child->GetEntity().lock()->FindComponent<StarComponent>().lock()->starMass;
		}
	}
	else
		mass2 = spStar2Com->starMass;

	spStar1Com->orbitRadius = distanceBetweenStars * (mass2/(mass1+mass2));
	spStar2Com->orbitRadius = distanceBetweenStars * (mass1 / (mass1 + mass2));

	double T = std::sqrt(std::pow(distanceBetweenStars,3)/(mass1+mass2));
	//velocity in radians per year
	double omega = (2*gel::PI) / T;

	//Assuming that in my game every year has 365 days
	spStar1Com->rotationalVelocity = omega / 365;
	spStar2Com->rotationalVelocity = omega / 365;

	spStar1Com->initialRotationPosition = gel::PI*randomPosition;
	spStar2Com->initialRotationPosition = gel::PI * (1+randomPosition);
}


void CalculateTernaryAfarSystemProperties(std::shared_ptr<Entity> star1Sp, std::shared_ptr<Entity> star2Sp, std::shared_ptr<Entity> star3Sp, double distanceBetweenStars, float randomPosition)
{
	std::shared_ptr<StarComponent> spStar1Com = star1Sp->FindComponent<StarComponent>().lock();
	std::shared_ptr<StarComponent> spStar2Com = star2Sp->FindComponent<StarComponent>().lock();
	std::shared_ptr<StarComponent> spStar3Com = star3Sp->FindComponent<StarComponent>().lock();

	spStar1Com->orbitRadius = distanceBetweenStars / 2.f;
	spStar2Com->orbitRadius = distanceBetweenStars / 2.f;
	spStar3Com->orbitRadius = distanceBetweenStars / 2.f;

	float mass1 = spStar1Com->starMass;
	float mass2 = spStar2Com->starMass;
	float mass3 = spStar3Com->starMass;

	double T = std::sqrt(std::pow(distanceBetweenStars, 3) / (mass1 + mass2 + mass3));
	//velocity in radians per year
	double omega = (2 * gel::PI) / T;

	spStar1Com->rotationalVelocity = omega / 365;
	spStar2Com->rotationalVelocity = omega / 365;
	spStar3Com->rotationalVelocity = omega / 365;

	spStar1Com->initialRotationPosition = gel::PI * randomPosition;
	spStar2Com->initialRotationPosition = gel::PI * ((2.f/3.f) + randomPosition);
	spStar3Com->initialRotationPosition = gel::PI * ((4.f / 3.f) + randomPosition);
}



//Orbit type: 0 - close, 1 - within habitable, 2 - far
void WorldGenerator::CreateMoon(std::shared_ptr<std::uniform_real_distribution<float>> spMoonOrbitDist, float maxMoonSize, int orbitType, std::shared_ptr<SceneNode> spNode, int num, float mainPlanetSize, DistanceToStar habitDistToStar)
{
	std::shared_ptr<Entity> spMoon = CreateNewEntityAt(spNode, "Moon"+std::to_string(num)).lock();
	std::shared_ptr<PlanetComponent> spPlanetCom = spMoon->AddComponent<PlanetComponent>().lock();
	spMoon->inheritParentPosition = false;
	spPlanetCom->isMoon = true;
	spMoon->hidden = true;

	if (maxMoonSize > mapConfig.largeRockyPlanetSizes.y)
		maxMoonSize = mapConfig.largeRockyPlanetSizes.x;

	std::uniform_real_distribution<float> moonSizeDist(mapConfig.minMoonSize, maxMoonSize);
	spPlanetCom->planetSize = moonSizeDist(*randomizer);

	//spPlanetCom->orbitRadius = orbit;
	float orbit = (*spMoonOrbitDist)(*randomizer);
	int regeneratedCounter = 0;
	int i = 0;
	while (i < moonOrbitsGenerated.size() && regeneratedCounter < mapConfig.maxAmountOfSystemPosRegen)
	{
		if (orbit / moonOrbitsGenerated[i] < 1 + mapConfig.minDistanceBetweenPlanetOrbitsInPercentage && orbit / moonOrbitsGenerated[i] > 1 - mapConfig.minDistanceBetweenPlanetOrbitsInPercentage)
		{
			orbit = (*spMoonOrbitDist)(*randomizer);
			i = -1;
			regeneratedCounter++;
		}

		i++;
	}
	if (regeneratedCounter >= mapConfig.maxAmountOfSystemPosRegen)
	{
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
		std::cout << "Regenerated moon orbit " << regeneratedCounter << " times!\n";
#endif
		return;
	}
	moonOrbitsGenerated.push_back(orbit);
	spPlanetCom->orbitRadius = orbit;
	
	int barrenType{ -1 };
	if (orbitType == 0) 
	{
		switch ((*closeOrbitMoonDist)(*randomizer)) 
		{
		case 0:
			barrenType = (*oneThird)(*randomizer);
			if (barrenType == 0)
				spPlanetCom->planetType = PlanetType::BarrenDark;
			else if (barrenType == 1)
				spPlanetCom->planetType = PlanetType::BarrenGrey;
			else
				spPlanetCom->planetType = PlanetType::BarrenMarsLike;
			break;
		case 1:
			if(mainPlanetSize>mapConfig.smallGasSizes.x)
				spPlanetCom->planetType = PlanetType::Voulcanic;
			else
				spPlanetCom->planetType = PlanetType::BarrenGrey;
			break;
		case 2:
			spPlanetCom->planetType = PlanetType::VenusLike;
			break;
		}
	}
	else if (orbitType == 1) 
	{
		switch ((*habitableZoneMoonDist)(*randomizer))
		{
		case 0:
			barrenType = (*oneThird)(*randomizer);
			if (barrenType == 0)
				spPlanetCom->planetType = PlanetType::BarrenDark;
			else if (barrenType == 1)
				spPlanetCom->planetType = PlanetType::BarrenGrey;
			else
				spPlanetCom->planetType = PlanetType::BarrenMarsLike;
			break;
		case 1:
			spPlanetCom->planetType = PlanetType::Oceanic;
			spMoon->AddComponent<HabitablePlanetComponent>();
			break;
		case 2:
			spPlanetCom->planetType = PlanetType::VenusLike;
			break;
		case 3:
			spPlanetCom->planetType = PlanetType::EarthLike;
			spMoon->AddComponent<HabitablePlanetComponent>();
			break;
		case 4:
			spPlanetCom->planetType = PlanetType::Desert;
			spMoon->AddComponent<HabitablePlanetComponent>();
			break;
		case 5:
			if (mainPlanetSize > mapConfig.smallGasSizes.x)
				spPlanetCom->planetType = PlanetType::Voulcanic;
			else
				spPlanetCom->planetType = PlanetType::BarrenGrey;
			break;
		}
	}
	else 
	{
		switch ((*farOrbitMoonDist)(*randomizer))
		{
		case 0:
			barrenType = (*oneThird)(*randomizer);
			if (barrenType == 0)
				spPlanetCom->planetType = PlanetType::BarrenDark;
			else if (barrenType == 1)
				spPlanetCom->planetType = PlanetType::BarrenGrey;
			else
				spPlanetCom->planetType = PlanetType::BarrenMarsLike;
			break;
		case 1:
			if (mainPlanetSize > mapConfig.smallGasSizes.x)
				spPlanetCom->planetType = PlanetType::Voulcanic;
			else
				spPlanetCom->planetType = PlanetType::BarrenGrey;
			break;
		case 2:
			spPlanetCom->planetType = PlanetType::TitanLike;
			break;
		case 3:
			spPlanetCom->planetType = PlanetType::Icy;
			break;
		}
	}

	if (spMoon->HasComponent<HabitablePlanetComponent>())
	{
		std::shared_ptr<HabitablePlanetComponent> spHabPlCom = spMoon->FindComponent<HabitablePlanetComponent>().lock();
		spHabPlCom->distanceToStar = habitDistToStar;
	}

	spPlanetCom->initialRotationPosition = (*from0to1Dist)(*randomizer) * 2 * gel::PI;
	spPlanetCom->rotationalVelocity = (std::sqrt(6.6743 * (std::pow(10, -11) * 1.194 * std::pow(10, 25) * gel::sphereVolume(mainPlanetSize/2.0)) / (spPlanetCom->orbitRadius * std::pow(10, 6))) * 86.4) / (spPlanetCom->orbitRadius * std::pow(10, 4));

	spPlanetCom->planetDistrictsSeed = (*randomizer)() / 2;
	//GenerateDistricts(spPlanetCom, spMoon->FindComponent<HabitablePlanetComponent>(), spNode->FindChild(*spMoon).lock());
}



void WorldGenerator::GenerateMoons(std::shared_ptr<PlanetComponent> spPlanet, sf::Vector2f habitableZoneBoundaries, std::shared_ptr<SceneNode> spNode)
{
	float chanceOfNoMoon{ 0.f };
	int orbitType = 0;
	if (static_cast<float>(spPlanet->orbitRadius) < habitableZoneBoundaries.x)
	{
		if (spPlanet->planetSize > mapConfig.smallGasSizes.x)
			chanceOfNoMoon = mapConfig.gasPlanetCloseOrbitMoonChance;
		else
			chanceOfNoMoon = mapConfig.rockyPlanetCloseOrbitMoonChance;

		orbitType = 0;
	}
	else if (static_cast<float>(spPlanet->orbitRadius) < habitableZoneBoundaries.y)
	{
		if (spPlanet->planetSize > mapConfig.smallGasSizes.x)
			chanceOfNoMoon = mapConfig.gasPlanetHabitableOrbitMoonChance;
		else
			chanceOfNoMoon = mapConfig.rockyPlanetHabitableOrbitMoonChance;

		orbitType = 1;
	}
	else
	{
		if (spPlanet->planetSize > mapConfig.smallGasSizes.x)
			chanceOfNoMoon = mapConfig.gasPlanetAfarOrbitMoonChance;
		else
			chanceOfNoMoon = mapConfig.rockyPlanetAfarOrbitMoonChance;

		orbitType = 2;
	}


	if ((*from0to1Dist)(*randomizer) < chanceOfNoMoon)
	{
		int numberOfMoons{ 0 };
		if (spPlanet->planetType == PlanetType::Icy || spPlanet->planetType == PlanetType::Oceanic) 
		{
			if (spPlanet->planetSize <= mapConfig.smallIcyPlanetSizes.y) 
			{
				float num = (*from0to1Dist)(*randomizer);
				if (num < mapConfig.smallPlanet1MoonChance)
					numberOfMoons = 1;
				else
					return;
			}
			else if (spPlanet->planetSize <= mapConfig.mediumIcyPlanetSizes.y)
			{
				float num = (*from0to1Dist)(*randomizer);
				if (num < mapConfig.mediumPlanet1MoonChance)
				{
					numberOfMoons = 1;
				}
				else
				{
					numberOfMoons = 2;
				}
			}
			else 
			{
				float num = (*from0to1Dist)(*randomizer);
				if(num< mapConfig.largePlanet1MoonChance)
					numberOfMoons = 1;
				else if (num < mapConfig.largePlanet1MoonChance+ mapConfig.largePlanet2MoonChance)
					numberOfMoons = 2;
				else
					numberOfMoons = 3;
			}
		}
		else if (spPlanet->planetSize >= mapConfig.smallGasSizes.x && spPlanet->planetSize <= mapConfig.smallGasSizes.y) 
		{
			float num = (*from0to1Dist)(*randomizer);
			if (num < mapConfig.smallGasSize1MoonChance)
				numberOfMoons = 1;
			else if (num < mapConfig.smallGasSize1MoonChance + mapConfig.smallGasSize2MoonChance)
				numberOfMoons = 2;
			else if (num < mapConfig.smallGasSize1MoonChance + mapConfig.smallGasSize2MoonChance + mapConfig.smallGasSize3MoonChance)
				numberOfMoons = 3;
			else
				numberOfMoons = 4;
		}
		else if (spPlanet->planetSize >= mapConfig.largeGasSizes.x && spPlanet->planetSize <= mapConfig.largeGasSizes.y)
		{
			float num = (*from0to1Dist)(*randomizer);
			if (num < mapConfig.largeGasSize1_2MoonChance)
			{
				numberOfMoons = 1;
				if ((*from0to1Dist)(*randomizer) < 0.5f)
					numberOfMoons++;
			}
			else if (num < mapConfig.largeGasSize1_2MoonChance + mapConfig.largeGasSize3_4MoonChance)
			{
				numberOfMoons = 3;
				if ((*from0to1Dist)(*randomizer) < 0.5f)
					numberOfMoons++;
			}
			else if (num < mapConfig.largeGasSize1_2MoonChance + mapConfig.largeGasSize3_4MoonChance + mapConfig.largeGasSize5_6MoonChance)
			{
				numberOfMoons = 5;
				if ((*from0to1Dist)(*randomizer) < 0.5f)
					numberOfMoons++;
			}
			else
			{
				numberOfMoons = 7;
				if ((*from0to1Dist)(*randomizer) < 0.5f)
					numberOfMoons++;
			}
		}
		else 
		{
			if (spPlanet->planetSize <= mapConfig.smallRockyPlanetSizes.y)
			{
				float num = (*from0to1Dist)(*randomizer);
				if (num < mapConfig.smallPlanet1MoonChance)
					numberOfMoons = 1;
				else
					return;
			}
			else if (spPlanet->planetSize <= mapConfig.mediumRockyPlanetSizes.y)
			{
				float num = (*from0to1Dist)(*randomizer);
				if (num < mapConfig.mediumPlanet1MoonChance)
					numberOfMoons = 1;
				else
					numberOfMoons = 2;
			}
			else
			{
				float num = (*from0to1Dist)(*randomizer);
				if (num < mapConfig.largePlanet1MoonChance)
					numberOfMoons = 1;
				else if (num < mapConfig.largePlanet1MoonChance + mapConfig.largePlanet2MoonChance)
					numberOfMoons = 2;
				else
					numberOfMoons = 3;
			}
		}

		DistanceToStar habitDistToStar = DistanceToStar::None;
		if (static_cast<float>(spPlanet->orbitRadius) < habitableZoneBoundaries.x + ((habitableZoneBoundaries.y - habitableZoneBoundaries.x) / 3.f))
			habitDistToStar = DistanceToStar::Close;
		else if (static_cast<float>(spPlanet->orbitRadius) < habitableZoneBoundaries.x + ((habitableZoneBoundaries.y - habitableZoneBoundaries.x) / 3.f * 2.f))
			habitDistToStar = DistanceToStar::Medium;
		else
			habitDistToStar = DistanceToStar::Far;

		float maxDistance = 600.f * std::pow(spPlanet->planetSize, 1.3f);
		float minDistance = mapConfig.earthDiameter * spPlanet->planetSize;
		std::shared_ptr<std::uniform_real_distribution<float>> moonOrbitRangeDist = std::make_shared<std::uniform_real_distribution<float>>(minDistance,maxDistance);
		std::shared_ptr<std::uniform_real_distribution<float>> closeMoonOrbitDist;
		if (spPlanet->planetSize >= mapConfig.smallGasSizes.x)
			closeMoonOrbitDist = std::make_shared<std::uniform_real_distribution<float>>(minDistance,maxDistance*mapConfig.gasPlanetCloseOrbitMultiplier);
		else
			closeMoonOrbitDist = std::make_shared<std::uniform_real_distribution<float>>(minDistance, maxDistance * mapConfig.rockyPlanetCloseOrbitMultiplier);
	
		moonOrbitsGenerated.clear();
		for (int i = 0; i < numberOfMoons; i++) 
		{
			if ((*from0to1Dist)(*randomizer) < mapConfig.closeMoonOrbitChance)
				CreateMoon(closeMoonOrbitDist, mapConfig.maxMoonSizeRelativeToPlanetSize*spPlanet->planetSize, orbitType, spNode, i, spPlanet->planetSize, habitDistToStar);
			else
				CreateMoon(moonOrbitRangeDist, mapConfig.maxMoonSizeRelativeToPlanetSize* spPlanet->planetSize, orbitType, spNode, i, spPlanet->planetSize, habitDistToStar);
		}
	}
	else
		return;
}



//Planet type: 0 - rocky/icy, 1 - gas
void WorldGenerator::GenerateRings(std::shared_ptr<SceneNode> spPlanetNode, float planetSize, PlanetType planetType)
{
	float ringChance = (*from0to1Dist)(*randomizer);

	bool generateRings = false;
	if (ringChance < mapConfig.gasPlanetRingChance && (planetType == PlanetType::NeptuneLike || planetType == PlanetType::HotNeptune || planetType == PlanetType::UranusLike || planetType == PlanetType::JupiterLike || planetType == PlanetType::SaturnLike || planetType == PlanetType::HotJupiter))
		generateRings = true;
	else if (ringChance < mapConfig.rockyPlanetRingChance)
		generateRings = true;

	if (generateRings)
	{
		std::shared_ptr<Entity> spRing = CreateNewEntityAt(spPlanetNode, "Rings").lock();
		std::shared_ptr<RingComponent> spRingCom = spRing->AddComponent<RingComponent>().lock();
		spRing->hidden = true;
		spRing->inheritParentPosition = false;

		spRingCom->ringSize = mapConfig.ringSizeComparedToPlanet * planetSize;
		spRingCom->ringNumber = (*RingTypeDist)(*randomizer);
	}
}



void WorldGenerator::GenerateSinglePlanet(sf::Vector2f orbitBoundaries, sf::Vector2f habitableZoneBoundaries, int num, std::shared_ptr<SceneNode> spNode, float starMass, bool inheritPosition)
{
	std::uniform_real_distribution<double> orbitDist(static_cast<double>(orbitBoundaries.x), static_cast<double>(orbitBoundaries.y));
	double orbit = orbitDist(*randomizer);
	int regeneratedCounter = 0;
	int i = 0;
	while (i<orbitsGenerated.size() && regeneratedCounter<mapConfig.maxAmountOfSystemPosRegen) 
	{
		if (orbit / orbitsGenerated[i] < 1 + mapConfig.minDistanceBetweenPlanetOrbitsInPercentage && orbit / orbitsGenerated[i] > 1 - mapConfig.minDistanceBetweenPlanetOrbitsInPercentage)
		{
			orbit = orbitDist(*randomizer);
			i = -1;
			regeneratedCounter++;
		}

		i++;
	}

	if (regeneratedCounter >= mapConfig.maxAmountOfSystemPosRegen)
	{
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
		std::cout << "Regenerated planet orbit " << regeneratedCounter << " times!\n";
#endif
		return;
	}

#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
	std::cout << "Create planet at orbit " << orbit << " \n";
#endif

	orbitsGenerated.push_back(orbit);

	std::shared_ptr<Entity> spPlanet = CreateNewEntityAt(spNode, "Planet"+std::to_string(num)).lock();
	std::shared_ptr<PlanetComponent> spPlanetCom = spPlanet->AddComponent<PlanetComponent>().lock();
	spPlanet->inheritParentPosition = inheritPosition;
	spPlanetCom->orbitRadius = orbit;

	bool generateBarrenType = false;
	bool rockyPlanet = false;
	bool icyPlanet = false;

	if (static_cast<float>(spPlanetCom->orbitRadius) < habitableZoneBoundaries.x)
	{
		//Closer than habit
		if (static_cast<float>(spPlanetCom->orbitRadius) < habitableZoneBoundaries.x / 3.f)
		{
			spPlanetCom->planetType = PlanetType::Molten;
			rockyPlanet = true;
		}
		else
		{

			switch ((*closerThanHabitableZoneDist)(*randomizer))
			{
			case 0:
				spPlanetCom->planetType = PlanetType::VenusLike;
				rockyPlanet = true;
				break;
			case 1:
				generateBarrenType = true;
				rockyPlanet = true;
				break;
			case 2:
				spPlanetCom->planetType = PlanetType::HotJupiter;
				spPlanetCom->planetSize = (*largeGiantPlanetDist)(*randomizer);
				break;
			case 3:
				spPlanetCom->planetType = PlanetType::HotNeptune;
				spPlanetCom->planetSize = (*smallGiantPlanetDist)(*randomizer);
				break;
			}
		}
	}
	else if (static_cast<float>(spPlanetCom->orbitRadius) > habitableZoneBoundaries.y)
	{
		//Further than habit
		switch ((*furtherThanHabitableZoneDist)(*randomizer))
		{
		case 0:
			spPlanetCom->planetType = PlanetType::Icy;
			icyPlanet = true;
			break;
		case 1:
			generateBarrenType = true;
			rockyPlanet = true;
			break;
		case 2:
			if ((*from0to1Dist)(*randomizer) > 0.5f)
				spPlanetCom->planetType = PlanetType::NeptuneLike;
			else
				spPlanetCom->planetType = PlanetType::UranusLike;

			spPlanetCom->planetSize = (*smallGiantPlanetDist)(*randomizer);
			break;
		case 3:
			if ((*from0to1Dist)(*randomizer) > 0.5f)
				spPlanetCom->planetType = PlanetType::JupiterLike;
			else
				spPlanetCom->planetType = PlanetType::SaturnLike;

			spPlanetCom->planetSize = (*largeGiantPlanetDist)(*randomizer);
			break;
		}
	}
	else 
	{
		//Within habit
		switch ((*withinHabitableZoneDist)(*randomizer))
		{
		case 0:
			spPlanetCom->planetType = PlanetType::VenusLike;
			rockyPlanet = true;
			break;
		case 1:
			spPlanetCom->planetType = PlanetType::Oceanic;
			icyPlanet = true;
			spPlanet->AddComponent<HabitablePlanetComponent>();
			break;
		case 2:
			spPlanetCom->planetType = PlanetType::EarthLike;
			rockyPlanet = true;
			spPlanet->AddComponent<HabitablePlanetComponent>();
			break;
		case 3:
			spPlanetCom->planetType = PlanetType::Desert;
			rockyPlanet = true;
			spPlanet->AddComponent<HabitablePlanetComponent>();
			break;
		case 4:
			generateBarrenType = true;
			rockyPlanet = true;
			break;
		case 5:
			spPlanetCom->planetType = PlanetType::HotJupiter;
			spPlanetCom->planetSize = (*largeGiantPlanetDist)(*randomizer);
			break;
		case 6:
			spPlanetCom->planetType = PlanetType::HotNeptune;
			spPlanetCom->planetSize = (*smallGiantPlanetDist)(*randomizer);
			break;
		}
	}

	if (spPlanet->HasComponent<HabitablePlanetComponent>())
	{
		std::shared_ptr<HabitablePlanetComponent> spHabPlCom = spPlanet->FindComponent<HabitablePlanetComponent>().lock();

		if (static_cast<float>(spPlanetCom->orbitRadius) < habitableZoneBoundaries.x + ((habitableZoneBoundaries.y - habitableZoneBoundaries.x) / 3.f))
			spHabPlCom->distanceToStar = DistanceToStar::Close;
		else if (static_cast<float>(spPlanetCom->orbitRadius) < habitableZoneBoundaries.x + ((habitableZoneBoundaries.y - habitableZoneBoundaries.x) / 3.f *2.f))
			spHabPlCom->distanceToStar = DistanceToStar::Medium;
		else
			spHabPlCom->distanceToStar = DistanceToStar::Far;
	}

	if (generateBarrenType) 
	{
		switch ((*oneThird)(*randomizer)) 
		{
		case 0:
			spPlanetCom->planetType = PlanetType::BarrenGrey;
			break;
		case 1:
			spPlanetCom->planetType = PlanetType::BarrenMarsLike;
			break;
		case 2:
			spPlanetCom->planetType = PlanetType::BarrenDark;
			break;
		}
	}

	if (rockyPlanet) 
	{
		float num = (*from0to1Dist)(*randomizer);
		if (num < mapConfig.smallRockyPlanetChance)
			spPlanetCom->planetSize = (*smallRockyPlanetDist)(*randomizer);
		else if (num < mapConfig.smallRockyPlanetChance + mapConfig.mediumRockyPlanetChance)
			spPlanetCom->planetSize = (*mediumRockyPlanetDist)(*randomizer);
		else
			spPlanetCom->planetSize = (*largeRockyPlanetDist)(*randomizer);
	}

	if (icyPlanet)
	{
		float num = (*from0to1Dist)(*randomizer);
		if (num < mapConfig.smallIcyPlanetChance)
			spPlanetCom->planetSize = (*smallIcyPlanetDist)(*randomizer);
		else if (num < mapConfig.smallIcyPlanetChance + mapConfig.mediumIcyPlanetChance)
			spPlanetCom->planetSize = (*mediumIcyPlanetDist)(*randomizer);
		else
			spPlanetCom->planetSize = (*largeIcyPlanetDist)(*randomizer);
	}

	spPlanetCom->rotationalVelocity = (std::sqrt(6.6743*(std::pow(10,-11)*starMass*2* std::pow(10, 30)) / (spPlanetCom->orbitRadius*1.5 * std::pow(10, 11))) * 86.4)/ (spPlanetCom->orbitRadius*1.5*std::pow(10,8));
	spPlanetCom->initialRotationPosition = (*from0to1Dist)(*randomizer) * 2 * gel::PI;

	GenerateRings(spNode->FindChild("Planet" + std::to_string(num)).lock(), spPlanetCom->planetSize, spPlanetCom->planetType);
	GenerateMoons(spPlanetCom, habitableZoneBoundaries, spNode->FindChild("Planet" + std::to_string(num)).lock());

	if(rockyPlanet || icyPlanet)
		spPlanetCom->planetDistrictsSeed = (*randomizer)() / 2;
		//GenerateDistricts(spPlanetCom, spPlanet->FindComponent<HabitablePlanetComponent>(), spNode->FindChild(*spPlanet).lock());
}



void WorldGenerator::GeneratePlanets(std::shared_ptr<SceneNode> spSystemOrStarNode, double distanceBetweenStars, bool singleStarSystem, bool inheritPosition)
{
	std::shared_ptr<StarComponent> spStarCom;
	std::shared_ptr<Entity> spEntity = spSystemOrStarNode->GetEntity().lock();
	if(!spEntity->HasComponent<StarComponent>() && !spEntity->HasComponent<ObjectSystemComponent>())
		spEntity = spSystemOrStarNode->GetParent().lock()->GetEntity().lock();
	sf::Vector2f orbitBoundaries{0.f, 0.f};
	sf::Vector2f habitableBoundaries{ -1.f, -1.f };
	bool checkMaxOrbitBounds=false;
	std::shared_ptr<std::uniform_int_distribution<int>> planetsDist;
	bool decreaseHabitableBoundaries = false;
	float starMass{0.f};

	if (spEntity->HasComponent<ObjectSystemComponent>())
	{
		std::shared_ptr<StarComponent> spStar1Com;
		std::shared_ptr<StarComponent> spStar2Com;
		if(spEntity->GetName()=="InsideSystem")
		{
			spStar1Com = spSystemOrStarNode->FindChild("Star2").lock()->GetEntity().lock()->FindComponent<StarComponent>().lock();
			spStar2Com = spSystemOrStarNode->FindChild("Star3").lock()->GetEntity().lock()->FindComponent<StarComponent>().lock();
		}
		else 
		{
			spStar1Com = spSystemOrStarNode->FindChild("Star1").lock()->GetEntity().lock()->FindComponent<StarComponent>().lock();
			spStar2Com = spSystemOrStarNode->FindChild("Star2").lock()->GetEntity().lock()->FindComponent<StarComponent>().lock();
		}


		if (spStar1Com->starType == StarType::BlackHole || spStar2Com->starType == StarType::BlackHole)
			return;

		std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();
		if (spSysCom->systemType != SpaceSystemType::BinaryClose)
			checkMaxOrbitBounds = true;

		if (spSysCom->systemType == SpaceSystemType::BinaryClose)
			decreaseHabitableBoundaries = true;

		if(spStar1Com->orbitRadius> spStar2Com->orbitRadius)
			orbitBoundaries.x = static_cast<float>(spStar1Com->orbitRadius * 1.4);
		else
			orbitBoundaries.x = static_cast<float>(spStar2Com->orbitRadius * 1.4);

		if (spStar1Com->starType>spStar2Com->starType)
			spStarCom = spStar1Com;
		else
			spStarCom = spStar2Com;

		starMass = spStar1Com->starMass + spStar2Com->starMass;
	}
	else
	{
		spStarCom = spEntity->FindComponent<StarComponent>().lock();

		if (spStarCom->starType == StarType::BlackHole)
			return;

		if (!singleStarSystem)
			checkMaxOrbitBounds = true;

		starMass = spStarCom->starMass;
	}

	//Check if this system will have planets or not
	//And set orbit boundaries and get how many planets to generate
	//And set habitable zone boundaries
	//Add boundaries
	float planetsChance = (*from0to1Dist)(*randomizer);
	switch (spStarCom->starType)
	{
	case StarType::RedSupergiant:
		if (planetsChance > mapConfig.redSupGiantPlanetChance)
			return;
		
		orbitBoundaries += mapConfig.orbitBoundariesRedSupergiant;
		planetsDist = redSupGiantPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneRedSupergiant;
		break;
	case StarType::RedGiant:
		if (planetsChance > mapConfig.redGiantPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesRedGiant;
		planetsDist = redGiantPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneRedGiant;
		break;
	case StarType::Otype:
		if (planetsChance > mapConfig.classOPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesClassO;
		planetsDist = OclassPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneClassO;
		break;
	case StarType::Btype:
		if (planetsChance > mapConfig.classBPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesClassB;
		planetsDist = BclassPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneClassB;
		break;
	case StarType::Atype:
		if (planetsChance > mapConfig.classAPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesClassA;
		planetsDist = AclassPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneClassA;
		break;
	case StarType::Ftype:
		if (planetsChance > mapConfig.classFPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesClassF;
		planetsDist = FclassPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneClassF;
		break;
	case StarType::GsunLike:
		if (planetsChance > mapConfig.classGPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesClassG;
		planetsDist = GclassPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneClassG;
		break;
	case StarType::KorangeDwarf:
		if (planetsChance > mapConfig.classKPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesClassK;
		planetsDist = KclassPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneClassK;
		break;
	case StarType::MredDwarf:
		if (planetsChance > mapConfig.classMPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesClassM;
		planetsDist = MclassPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneClassM;
		break;
	case StarType::BrownDwarf:
		if (planetsChance > mapConfig.brownDwarfPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesBrownDwarf;
		planetsDist = brownDwarfPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneBrownDwarf;
		break;
	case StarType::WhiteDwarf:
		if (planetsChance > mapConfig.whiteDwarfPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesWhiteDwarf;
		planetsDist = whiteDwarfPlanetsDist;
		habitableBoundaries = mapConfig.habitableZoneWhiteDwarf;
		break;
	case StarType::NeutronStar:
		if (planetsChance > mapConfig.neutronPlanetChance)
			return;

		orbitBoundaries += mapConfig.orbitBoundariesNeutronStar;
		planetsDist = neutronStarPlanetsDist;
		break;
	}

	//Check boundaries for binary and ternary systems
	if (checkMaxOrbitBounds) 
	{
		if (orbitBoundaries.y > static_cast<float>(distanceBetweenStars * (1.f / 3.f)))
			orbitBoundaries.y = static_cast<float>(distanceBetweenStars * (1.f / 3.f));
	}

	if (orbitBoundaries.x > orbitBoundaries.y)
		return;

	if (decreaseHabitableBoundaries) 
	{
		habitableBoundaries.y = habitableBoundaries.y - ((habitableBoundaries.y - habitableBoundaries.x)/2.f);
	}

	//Generate how many planets will orbit it
	int numOfPlanets = (*planetsDist)(*randomizer);
	orbitsGenerated.clear();
	for (int i = 0; i < numOfPlanets; i++) 
	{
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
		std::cout << "Planet " << i << ") \n";
#endif
		//I generate half of the planets close to the star and other half further away
		if (orbitBoundaries.x < habitableBoundaries.y)
		{
			if((*from0to1Dist)(*randomizer) < mapConfig.chanceOfTheClosePlanet)
				GenerateSinglePlanet(sf::Vector2f{ orbitBoundaries.x, habitableBoundaries.y }, habitableBoundaries, i, spSystemOrStarNode, starMass, inheritPosition);
			else if (habitableBoundaries.y < orbitBoundaries.y)
				GenerateSinglePlanet(sf::Vector2f{ habitableBoundaries.y, orbitBoundaries.y }, habitableBoundaries, i, spSystemOrStarNode, starMass, inheritPosition);
		}
		else
			GenerateSinglePlanet(orbitBoundaries, habitableBoundaries, i, spSystemOrStarNode, starMass, inheritPosition);
	}
}



void WorldGenerator::GenerateSystemType(std::shared_ptr<std::discrete_distribution<int>> systemTypeDist, std::shared_ptr<ObjectSystemComponent> spSystemCom, std::shared_ptr<SceneNode> ptrSystemNode, std::shared_ptr<Entity> spStar1Entity)
{
	switch ((*systemTypeDist)(*randomizer)) 
	{
	case 0:
	{
		spSystemCom->systemType = SpaceSystemType::Single;
		GeneratePlanets(ptrSystemNode->FindChild("Star1").lock(), -1.0, true, false);
		return;
	}
	case 1:
	{
		//Create star in that system
		std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(ptrSystemNode, "Star2").lock();
		GenerateStarProperties(spStar2->AddComponent<StarComponent>().lock(), spStar2);
		spStar2->inheritParentPosition = false;

		//Determine binary system type
		if ((*binarySysDistribution)(*randomizer) == 0)
		{
			spSystemCom->systemType = SpaceSystemType::BinaryClose;
			double distBetStars = (*closeStarsDistances)(*randomizer);

			if (spStar2->FindComponent<StarComponent>().lock()->starType == StarType::RedGiant && distBetStars < 1.5)
				distBetStars = 1.5;

			CalculateBinarySystemProperties(spStar1Entity, spStar2, distBetStars, (*from0to1Dist)(*randomizer), ptrSystemNode);
			GeneratePlanets(ptrSystemNode, distBetStars, false, false);
		}
		else
		{
			spSystemCom->systemType = SpaceSystemType::BinaryAfar;
			double distBetStars = (*afarStarsDistances)(*randomizer);
			CalculateBinarySystemProperties(spStar1Entity, spStar2, distBetStars, (*from0to1Dist)(*randomizer), ptrSystemNode);
			GeneratePlanets(ptrSystemNode->FindChild("Star1").lock(), distBetStars, false, true);
			GeneratePlanets(ptrSystemNode->FindChild("Star2").lock(), distBetStars, false, true);
		}
		return;
	}
	case 2:
	{
		//Determine ternary system type
		int val = (*ternarySysDistribution)(*randomizer);
		if (val==0)
		{
			//Binary + one afar

			//Create system for binary stars
			std::shared_ptr<Entity> spInsideSys = CreateNewEntityAt(ptrSystemNode, "InsideSystem").lock();
			spInsideSys->AddComponent<StarComponent>();
			std::shared_ptr<ObjectSystemComponent> spSysCom = spInsideSys->AddComponent<ObjectSystemComponent>().lock();
			spSysCom->systemType = SpaceSystemType::BinaryCloseWithin;
			spInsideSys->inheritParentPosition = false;
			std::shared_ptr<SceneNode> spInsideSysNode = ptrSystemNode->FindChild("InsideSystem").lock();

			//Create 2 stars in the inside system
			std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(spInsideSysNode, "Star2").lock();
			GenerateStarProperties(spStar2->AddComponent<StarComponent>().lock(), spStar2);
			//spStar2->inheritParentPosition = false;

			std::shared_ptr<Entity> spStar3 = CreateNewEntityAt(spInsideSysNode, "Star3").lock();
			GenerateStarProperties(spStar3->AddComponent<StarComponent>().lock(), spStar3);
			//spStar3->inheritParentPosition = false;

			spSystemCom->systemType = SpaceSystemType::TernaryTwoCloseThirdAfar;
			double closeDistBetStars = (*closeStarsDistances)(*randomizer);

			if ((spStar2->FindComponent<StarComponent>().lock()->starType == StarType::RedGiant || spStar3->FindComponent<StarComponent>().lock()->starType == StarType::RedGiant) && closeDistBetStars < 1.5)
				closeDistBetStars = 1.5;

			double afarDistBetStars = (*afarStarsDistances)(*randomizer);
			CalculateBinarySystemProperties(spStar3, spStar2, closeDistBetStars, (*from0to1Dist)(*randomizer), ptrSystemNode);
			CalculateBinarySystemProperties(spStar1Entity, spInsideSys, afarDistBetStars, (*from0to1Dist)(*randomizer), ptrSystemNode);
			
			GeneratePlanets(ptrSystemNode->FindChild("InsideSystem").lock(), afarDistBetStars, false, true);
			GeneratePlanets(ptrSystemNode->FindChild("Star1").lock(), afarDistBetStars, false, true);
		}
		else
		{
			//All equaly afar

			//Create 2 stars in that system
			std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(ptrSystemNode, "Star2").lock();
			GenerateStarProperties(spStar2->AddComponent<StarComponent>().lock(), spStar2);
			spStar2->inheritParentPosition = false;

			std::shared_ptr<Entity> spStar3 = CreateNewEntityAt(ptrSystemNode, "Star3").lock();
			GenerateStarProperties(spStar3->AddComponent<StarComponent>().lock(), spStar3);
			spStar3->inheritParentPosition = false;

			spSystemCom->systemType = SpaceSystemType::TernaryAfar;
			double distBetStars = (*afarStarsDistances)(*randomizer);
			CalculateTernaryAfarSystemProperties(spStar1Entity, spStar2, spStar3, distBetStars, (*from0to2_3Dist)(*randomizer));
			GeneratePlanets(ptrSystemNode->FindChild("Star1").lock(), distBetStars, false, true);
			GeneratePlanets(ptrSystemNode->FindChild("Star2").lock(), distBetStars, false, true);
			GeneratePlanets(ptrSystemNode->FindChild("Star3").lock(), distBetStars, false, true);
		}

		return;
	}
	}
}


void WorldGenerator::GenerateSpaceMap(std::shared_ptr<SceneNode> ptrSpaceMapNode)
{
	//Create star positions map
	std::shared_ptr<SystemPropertiesComponent> spSysPropCom = ptrSpaceMapNode->GetEntity().lock()->FindComponent<SystemPropertiesComponent>().lock();
	int gridWidth =(int)((mapConfig.horizontalPosBoundaries.y - mapConfig.horizontalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems);

	std::uniform_real_distribution<float> XpositionDist(mapConfig.horizontalPosBoundaries.x, mapConfig.horizontalPosBoundaries.y);
	std::uniform_real_distribution<float> YpositionDist(mapConfig.verticalPosBoundaries.x, mapConfig.verticalPosBoundaries.y);
	
	for (int i=0; i < mapConfig.systemAmount; i++) 
	{
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
		std::cout << i << ") \n";
#endif
		//Generate position
		bool regeneratePos = true;
		int regenCounter{0};
		sf::Vector2f newPos;
		while (regeneratePos && regenCounter<mapConfig.maxAmountOfSystemPosRegen)
		{
			newPos = sf::Vector2f{ XpositionDist(*randomizer),YpositionDist(*randomizer) };

			regeneratePos = false;
			if (GetAllSystemsNearPosition(newPos, ptrSpaceMapNode->GetEntity().lock()->FindComponent<SystemPropertiesComponent>()).size()>0)
				regeneratePos = true;

			regenCounter++;
		}

		if (regenCounter >= mapConfig.maxAmountOfSystemPosRegen)
		{
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
			std::cout << "Regenerated system position " << regenCounter << " times!\n";
#endif
			continue;
		}

		//Create new system
		std::shared_ptr<Entity> spNewSystem = CreateNewEntityAt(ptrSpaceMapNode, "System" + std::to_string(i)).lock();
		std::shared_ptr<ObjectSystemComponent> spSystemCom = spNewSystem->AddComponent<ObjectSystemComponent>().lock();
		std::shared_ptr<SceneNode> ptrNewSysNode = ptrSpaceMapNode->FindChild(*spNewSystem).lock();
		//Create node which will store all system nodes
		std::shared_ptr<Entity> spNewEn = CreateNewEntityAt(ptrNewSysNode, "Node").lock();
		std::shared_ptr<SceneNode> spNewNode = ptrSpaceMapNode->FindChild(*spNewEn).lock();
		//Set position
		int yPos = (int)((newPos.y - mapConfig.verticalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems);
		int xPos = (int)((newPos.x - mapConfig.horizontalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems);
		spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos })] = ptrNewSysNode;
		spNewSystem->SetPosition(newPos);

		//Create star in that system
		std::shared_ptr<Entity> spStar1 = CreateNewEntityAt(spNewNode, "Star1").lock();
		std::shared_ptr<StarComponent> spStar1Com = spStar1->AddComponent<StarComponent>().lock();
		spStar1->inheritParentPosition = false;

		GenerateStarProperties(spStar1Com, spStar1);
		switch (spStar1Com->starType)
		{
		case StarType::RedSupergiant:
			spSystemCom->systemType = SpaceSystemType::Single;
			GeneratePlanets(spNewNode->FindChild("Star1").lock(), -1.0, true, false);
			break;
		case StarType::RedGiant:
		case StarType::Otype:
		case StarType::Btype:
		case StarType::Atype:
			GenerateSystemType(giantSysDistribution, spSystemCom, spNewNode, spStar1);
			break;
		case StarType::Ftype:
		case StarType::GsunLike:
		case StarType::KorangeDwarf:
			GenerateSystemType(mediumSysDistribution, spSystemCom, spNewNode, spStar1);
			break;
		case StarType::MredDwarf:
		case StarType::BrownDwarf:
		case StarType::WhiteDwarf:
			GenerateSystemType(dwarfSysDistribution, spSystemCom, spNewNode, spStar1);
			break;
		case StarType::NeutronStar:
			spSystemCom->systemType = SpaceSystemType::Single;
			GeneratePlanets(spNewNode->FindChild("Star1").lock(), -1.0, true, false);
			break;
		case StarType::BlackHole:
			spSystemCom->systemType = SpaceSystemType::Single;
			break;
		}

		std::shared_ptr<ObjectSystemComponent> spNodeObjSys = spNewEn->AddComponent<ObjectSystemComponent>().lock();
		spNodeObjSys->systemType = spSystemCom->systemType;
		
		spSystemCom->spAllSystemObjectsNode = spNewNode;
		ptrNewSysNode->RemoveNode(spNewNode);

		//Calculate how far nebula is
		//ADD THIS IN THE FUTURE IF NECESSARY!
	}
}



void WorldGenerator::GenerateNebulas(std::shared_ptr<SceneNode> ptrNebulasNode, std::shared_ptr<SceneNode> spSystemNamesNode)
{
	std::uniform_int_distribution<int> numOfNebulasDist(mapConfig.numOfNebulasInWorld.x, mapConfig.numOfNebulasInWorld.y);
	std::uniform_real_distribution<float> sizeOfNebulasDist(mapConfig.nebulaSizeRange.x, mapConfig.nebulaSizeRange.y);
	float minDist = (mapConfig.horizontalPosBoundaries.y - mapConfig.horizontalPosBoundaries.x) * mapConfig.minimalDistanceFromTheMapBorder;
	std::uniform_real_distribution<float> XpositionDist(mapConfig.horizontalPosBoundaries.x+minDist, mapConfig.horizontalPosBoundaries.y- minDist);
	minDist = (mapConfig.verticalPosBoundaries.y - mapConfig.verticalPosBoundaries.x) * mapConfig.minimalDistanceFromTheMapBorder;
	std::uniform_real_distribution<float> YpositionDist(mapConfig.verticalPosBoundaries.x+minDist, mapConfig.verticalPosBoundaries.y-minDist);
	std::vector<std::string> nebulaNames = mapConfig.nebulaNames;
	std::vector<int> textureNums;
	for (int i = 0; i < mapConfig.numOfNebulaTextures; i++) 
	{
		textureNums.push_back(i);
	}
	std::vector<sf::Vector2f> positionsSelected;
	std::vector<float> sizes;

	int numOfNebulas = numOfNebulasDist(*randomizer);
	for (int i = 0; i < numOfNebulas; i++) 
	{
		sf::Vector2f newPos{ XpositionDist(*randomizer),YpositionDist(*randomizer) };
		float nebulaSize = sizeOfNebulasDist(*randomizer);
		int index = 0;
		int regeneratedPos = 0;
		while (index < positionsSelected.size() && regeneratedPos<mapConfig.maxAmountOfSystemPosRegen) 
		{
			if (gel::distanceBetween2Points(newPos, positionsSelected[index]) < (sizes[index] + nebulaSize) / 2.f) 
			{
				index = -1;
				newPos = sf::Vector2f{ XpositionDist(*randomizer),YpositionDist(*randomizer) };
				nebulaSize = sizeOfNebulasDist(*randomizer);
				regeneratedPos++;
			}
			index++;
		}
		positionsSelected.push_back(newPos);
		sizes.push_back(nebulaSize);

#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
		if (regeneratedPos >= mapConfig.maxAmountOfSystemPosRegen)
			std::cout << "Regenerated nebula position " << regeneratedPos << " times\n";
#endif

		//Create new nebula
		std::shared_ptr<Entity> spNewNeb = CreateNewEntityAt(ptrNebulasNode, "Nebula" + std::to_string(i)).lock();
		spNewNeb->SetPosition(newPos);
		spNewNeb->inheritParentPosition = false;

		//Setup nebula properties
		std::shared_ptr<NebulaComponent> spNebulaCom = spNewNeb->AddComponent<NebulaComponent>().lock();
		spNebulaCom->nebulaSize = nebulaSize;
		std::uniform_int_distribution<int> nameDist(0, (int)nebulaNames.size() - 1);
		int pos = nameDist(*randomizer);
		spNebulaCom->nebulaName = nebulaNames[pos];
		nebulaNames[pos] = nebulaNames.back();
		nebulaNames.pop_back();

		//Setup nebula texture
		std::shared_ptr<RectangleShapeComponent> spRectShapeCom = spNewNeb->AddComponent<RectangleShapeComponent>().lock();
		std::uniform_int_distribution<int> textureNumDist(0, (int)textureNums.size() - 1);
		int num = textureNumDist(*randomizer);
		SetupRectangleShape(spRectShapeCom, sf::Vector2f{ spNebulaCom->nebulaSize,spNebulaCom->nebulaSize }, "Nebula" + std::to_string(textureNums[num]));
		textureNums[num] = textureNums.back();
		textureNums.pop_back();
		spRectShapeCom->shape.setFillColor(sf::Color(150,150,150));

		//Setup nebula name text
		std::string name{ "NebulaNameText" };
		std::shared_ptr<Entity> spNebTextEn = CreateSystemText(spSystemNamesNode, ptrNebulasNode->FindChild("Nebula" + std::to_string(i)).lock(), name, false);
		std::shared_ptr<UIFollowerComponent> spUiFollower = spNebTextEn->FindComponent<UIFollowerComponent>().lock();
		spNebulaCom->wpTextFollower = spUiFollower;
	}
}