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
//#include "ParticlesConfigurations.h"
#include "SpaceObjectTypes.h"
#include <unordered_map>
#include <rapidcsv.h>


constexpr double PI = 3.14159265358979323846;


//Read data from the file
std::vector<std::string> ReadStarNamesFromCSV(const std::string fname)
{
	std::vector<std::string> data;
	rapidcsv::LabelParams labelParams(-1, -1); // this says that row and column
	//data will start from index 0 --no headers expected, either horizontally or
	//	vertically
	rapidcsv::Document doc(fname, labelParams);
	const size_t numRows = doc.GetRowCount();
	size_t numColumns = 0;
	for (size_t i = 0; i < numRows; ++i)
	{
		std::vector<std::string> rowData = doc.GetRow<std::string>(i);
		if (i == 0)
			numColumns = rowData.size();
		for (size_t j = 0; j < numColumns; ++j)
			data.push_back(rowData[j]);
		// Another way, using STL: append the entire rowData at the end of "data"
		// data.insert(data.end(), rowData.begin(), rowData.end());
	}
	return data;
}



unsigned int WorldGenerator::seed = 0;
std::shared_ptr<std::mt19937> WorldGenerator::randomizer = nullptr;
//How much rarer tiles with nebula will be generated to usual tiles
float WorldGenerator::nebulaRareness = 2.f;
int WorldGenerator::numberOfNebulas = 6;

std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::starDistribution = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::giantSysDistribution = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::mediumSysDistribution = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::dwarfSysDistribution = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::binarySysDistribution = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::ternarySysDistribution = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::closeStarsDistances = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::afarStarsDistances = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::oneThird = std::make_shared<std::uniform_int_distribution<int>>(0,2);
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::from0to1Dist = std::make_shared<std::uniform_real_distribution<float>>(0.f, 1.f);
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::from0to2_3Dist = std::make_shared<std::uniform_real_distribution<float>>(0.f, 2.f / 3.f);
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::redSupGiantPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::redGiantPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::OclassPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::BclassPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::AclassPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::FclassPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::GclassPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::KclassPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::MclassPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::brownDwarfPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::whiteDwarfPlanetsDist = nullptr;
std::shared_ptr<std::uniform_int_distribution<int>> WorldGenerator::neutronStarPlanetsDist = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::closerThanHabitableZoneDist = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::withinHabitableZoneDist = nullptr;
std::shared_ptr<std::discrete_distribution<int>> WorldGenerator::furtherThanHabitableZoneDist = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::smallRockyPlanetDist = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::mediumRockyPlanetDist = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::largeRockyPlanetDist = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::smallIcyPlanetDist = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::mediumIcyPlanetDist = nullptr;
std::shared_ptr<std::uniform_real_distribution<float>> WorldGenerator::largeIcyPlanetDist = nullptr;


void WorldGenerator::Initialize(unsigned int seedOut)
{
	seed = seedOut;
	randomizer = std::make_shared<std::mt19937>(std::mt19937{seed});
}


std::vector<int> WorldGenerator::GenerateGridOfTiles(sf::Vector2i gridSize, sf::Vector2i minMaxValues)
{
	//Here i assumed that only first 6 tiles has nebulas and i want tiles with nebulas to generate
	//more rarely than usual ones
	std::vector<int> randomNumbers(gridSize.x * gridSize.y);

	std::uniform_int_distribution nextNumber{ minMaxValues.x, minMaxValues.y };
	std::uniform_real_distribution nebulaChance{ 0.f, nebulaRareness };
	std::uniform_int_distribution randomStep{ numberOfNebulas, minMaxValues.y - numberOfNebulas };
	for (int& val : randomNumbers)
	{
		int nextNum = nextNumber(*randomizer);
		if (nextNum < numberOfNebulas)
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


void WorldGenerator::StarTypeGenerator(std::weak_ptr<StarComponent> wpStarCom) 
{
	std::shared_ptr<StarComponent> spStarCom = wpStarCom.lock();
	switch ((*starDistribution)(*randomizer))
	{
	case 0:
		spStarCom->starType = StarType::RedGiant;
		break;
	case 1:
		spStarCom->starType = StarType::RedGiant;
		break;
	case 2:
		spStarCom->starType = StarType::Otype;
		break;
	case 3:
		spStarCom->starType = StarType::Btype;
		break;
	case 4:
		spStarCom->starType = StarType::Atype;
		break;
	case 5:
		spStarCom->starType = StarType::Ftype;
		break;
	case 6:
		spStarCom->starType = StarType::GsunLike;
		break;
	case 7:
		spStarCom->starType = StarType::KorangeDwarf;
		break;
	case 8:
		spStarCom->starType = StarType::MredDwarf;
		break;
	case 9:
		spStarCom->starType = StarType::BrownDwarf;
		break;
	case 10:
		spStarCom->starType = StarType::WhiteDwarf;
		break;
	case 11:
		spStarCom->starType = StarType::NeutronStar;
		break;
	case 12:
		spStarCom->starType = StarType::WhiteDwarf;
		break;
	}
}



float GetStarMass(StarType starType) 
{
	switch (starType) 
	{
	case StarType::RedSupergiant:
		return 20.f;
	case StarType::RedGiant:
		return 4.f;
	case StarType::Otype:
		return 25.f;
	case StarType::Btype:
		return 9.f;
	case StarType::Atype:
		return 1.7f;
	case StarType::Ftype:
		return 1.2f;
	case StarType::GsunLike:
		return 1.f;
	case StarType::KorangeDwarf:
		return 0.6f;
	case StarType::MredDwarf:
		return 0.3f;
	case StarType::BrownDwarf:
		return 0.044f;
	case StarType::WhiteDwarf:
		return 0.6f;
	case StarType::NeutronStar:
		return 1.6f;
	case StarType::BlackHole:
		return 50.f;
	}
}



void CalculateBinarySystemProperties(std::shared_ptr<Entity> star1Sp, std::shared_ptr<Entity> star2Sp, float distanceBetweenStars, float randomPosition, std::shared_ptr<SceneNode> ptrSystemNode)
{
	std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*star1Sp);
	std::shared_ptr<StarComponent> spStar2Com = GetStarComponent(*star2Sp);

	float mass1{0.f};
	if (star1Sp->HasComponent(ComponentType::ObjectSystem))
	{
		std::shared_ptr<SceneNode> insideSysSp = ptrSystemNode->FindChild(*star1Sp).lock();
		std::vector<std::shared_ptr<SceneNode>> children = insideSysSp->GetAllChildren();
		for (std::shared_ptr<SceneNode> child : children) 
		{
			if (child->GetEntity().lock()->HasComponent(ComponentType::Star))
				mass1 += GetStarMass(GetStarComponent(*child->GetEntity().lock())->starType);
		}
	}
	else
		mass1 = GetStarMass(spStar1Com->starType);


	float mass2{ 0.f };
	if (star2Sp->HasComponent(ComponentType::ObjectSystem))
	{
		std::shared_ptr<SceneNode> insideSysSp = ptrSystemNode->FindChild(*star2Sp).lock();
		std::vector<std::shared_ptr<SceneNode>> children = insideSysSp->GetAllChildren();
		for (std::shared_ptr<SceneNode> child : children)
		{
			if (child->GetEntity().lock()->HasComponent(ComponentType::Star))
				mass2 += GetStarMass(GetStarComponent(*child->GetEntity().lock())->starType);
		}
	}
	else
		mass2 = GetStarMass(spStar2Com->starType);

	spStar1Com->orbitRadius = distanceBetweenStars * (mass2/(mass1+mass2));
	spStar2Com->orbitRadius = distanceBetweenStars * (mass1 / (mass1 + mass2));

	float T = std::sqrtf(std::powf(distanceBetweenStars,3)/(mass1+mass2));
	//velocity in radians per year
	float omega = (2*PI) / T;

	//Assuming that in my game every year has 365 days
	spStar1Com->rotationalVelocity = omega / 365;
	spStar2Com->rotationalVelocity = omega / 365;

	spStar1Com->initialRotationPosition = PI*randomPosition;
	spStar2Com->initialRotationPosition = PI * (1+randomPosition);
}


void CalculateTernaryAfarSystemProperties(std::shared_ptr<Entity> star1Sp, std::shared_ptr<Entity> star2Sp, std::shared_ptr<Entity> star3Sp, float distanceBetweenStars, float randomPosition)
{
	std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*star1Sp);
	std::shared_ptr<StarComponent> spStar2Com = GetStarComponent(*star2Sp);
	std::shared_ptr<StarComponent> spStar3Com = GetStarComponent(*star3Sp);

	spStar1Com->orbitRadius = distanceBetweenStars / 2.f;
	spStar2Com->orbitRadius = distanceBetweenStars / 2.f;
	spStar3Com->orbitRadius = distanceBetweenStars / 2.f;

	float mass1 = GetStarMass(spStar1Com->starType);
	float mass2 = GetStarMass(spStar2Com->starType);
	float mass3 = GetStarMass(spStar3Com->starType);

	float T = std::sqrtf(std::powf(distanceBetweenStars, 3) / (mass1 + mass2 + mass3));
	//velocity in radians per year
	float omega = (2 * PI) / T;

	spStar1Com->rotationalVelocity = omega / 365;
	spStar2Com->rotationalVelocity = omega / 365;
	spStar3Com->rotationalVelocity = omega / 365;

	spStar1Com->initialRotationPosition = PI * randomPosition;
	spStar2Com->initialRotationPosition = PI * ((2.f/3.f) + randomPosition);
	spStar3Com->initialRotationPosition = PI * ((4.f / 3.f) + randomPosition);
}



void WorldGenerator::GenerateSinglePlanet(sf::Vector2f orbitBoundaries, sf::Vector2f habitableZoneBoundaries, int num, std::shared_ptr<SceneNode> spNode, SpaceMapConfigurations& mapConfig, float starMass)
{
	std::shared_ptr<Entity> spPlanet = CreateNewEntityAt(spNode, "Planet"+std::to_string(num)).lock();
	spPlanet->AddComponent(ComponentType::Planet);
	std::shared_ptr<PlanetComponent> spPlanetCom = GetPlanetComponent(*spPlanet);

	std::uniform_int_distribution<int> orbitDist(orbitBoundaries.x, orbitBoundaries.y);
	spPlanetCom->orbitRadius = orbitDist(*randomizer);
	bool generateBarrenType = false;
	bool rockyPlanet = false;
	bool icyPlanet = false;

	if (spPlanetCom->orbitRadius < habitableZoneBoundaries.x) 
	{
		//Closer than habit
		if (spPlanetCom->orbitRadius < habitableZoneBoundaries.x / 3.f)
		{
			spPlanetCom->planetType = PlanetType::Molten;
			rockyPlanet = true;
		}

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
			break;
		case 3:
			spPlanetCom->planetType = PlanetType::HotNeptune;
			break;
		}
	}
	else if (spPlanetCom->orbitRadius > habitableZoneBoundaries.y)
	{
		//Further than habit
		switch ((*withinHabitableZoneDist)(*randomizer))
		{
		case 0:
			spPlanetCom->planetType = PlanetType::VenusLike;
			rockyPlanet = true;
			break;
		case 1:
			spPlanetCom->planetType = PlanetType::Oceanic;
			icyPlanet = true;
			break;
		case 2:
			spPlanetCom->planetType = PlanetType::EarthLike;
			rockyPlanet = true;
			break;
		case 3:
			spPlanetCom->planetType = PlanetType::Desert;
			rockyPlanet = true;
			break;
		case 4:
			generateBarrenType = true;
			rockyPlanet = true;
			break;
		case 5:
			spPlanetCom->planetType = PlanetType::HotJupiter;
			break;
		case 6:
			spPlanetCom->planetType = PlanetType::HotNeptune;
			break;
		}
	}
	else 
	{
		//Within habit
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
			if((*from0to1Dist)(*randomizer)>0.5f)
				spPlanetCom->planetType = PlanetType::NeptuneLike;
			else
				spPlanetCom->planetType = PlanetType::UranusLike;
			break;
		case 3:
			if ((*from0to1Dist)(*randomizer) > 0.5f)
				spPlanetCom->planetType = PlanetType::JupiterLike;
			else
				spPlanetCom->planetType = PlanetType::SaturnLike;
			break;
		}
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

	spPlanetCom->rotationalVelocity = std::sqrtf(6.6417f*(std::powf(10,21)*starMass) / spPlanetCom->orbitRadius)/ spPlanetCom->orbitRadius;
	spPlanetCom->initialRotationPosition = (*from0to1Dist)(*randomizer) * 2 * PI;
}



void WorldGenerator::GeneratePlanets(std::shared_ptr<SceneNode> spSystemOrStarNode, SpaceMapConfigurations& mapConfig, float distanceBetweenStars)
{
	std::shared_ptr<StarComponent> spStarCom;
	std::shared_ptr<Entity> spEntity = spSystemOrStarNode->GetEntity().lock();
	sf::Vector2f orbitBoundaries{0.f, 0.f};
	sf::Vector2f habitableBoundaries{ -1.f, -1.f };
	bool checkMaxOrbitBounds=false;
	std::shared_ptr<std::uniform_int_distribution<int>> planetsDist;
	bool decreaseHabitableBoundaries = false;
	float starMass{0.f};

	if (spEntity->HasComponent(ComponentType::ObjectSystem)) 
	{
		std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*spSystemOrStarNode->FindChild("Star1").lock()->GetEntity().lock());
		std::shared_ptr<StarComponent> spStar2Com = GetStarComponent(*spSystemOrStarNode->FindChild("Star2").lock()->GetEntity().lock());

		if (spStar1Com->starType == StarType::BlackHole || spStar2Com->starType == StarType::BlackHole)
			return;

		std::shared_ptr<ObjectSystemComponent> spSysCom = GetObjectSystemComponent(*spEntity);
		if (spSysCom->systemType != SpaceSystemType::BinaryClose)
			checkMaxOrbitBounds = true;

		if (spSysCom->systemType == SpaceSystemType::BinaryClose)
			decreaseHabitableBoundaries = true;

		if(spStar1Com->orbitRadius> spStar1Com->orbitRadius)
			orbitBoundaries.x = spStar1Com->orbitRadius;
		else
			orbitBoundaries.x = spStar2Com->orbitRadius;

		if (spStar1Com->starType>spStar2Com->starType)
			spStarCom = spStar1Com;
		else
			spStarCom = spStar2Com;

		starMass = GetStarMass(spStar1Com->starType) + GetStarMass(spStar2Com->starType);
	}
	else
	{
		spStarCom = GetStarComponent(*spEntity);

		if (spStarCom->starType == StarType::BlackHole)
			return;

		starMass = GetStarMass(spStarCom->starType);
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
		if (orbitBoundaries.y > distanceBetweenStars * (1.f / 3.f))
			orbitBoundaries.y = distanceBetweenStars * (1.f / 3.f);

		if (orbitBoundaries.x > orbitBoundaries.y)
			return;
	}

	if (decreaseHabitableBoundaries) 
	{
		habitableBoundaries.y = habitableBoundaries.y - ((habitableBoundaries.y - habitableBoundaries.x)/2.f);
	}

	//Generate how many planets will orbit it
	int numOfPlanets = (*planetsDist)(*randomizer);
	for (int i = 0; i < numOfPlanets; i++) 
	{
		GenerateSinglePlanet(orbitBoundaries, habitableBoundaries, i, spSystemOrStarNode, mapConfig, starMass);
	}
}



void WorldGenerator::GenerateSystemType(std::shared_ptr<std::discrete_distribution<int>> systemTypeDist, std::shared_ptr<ObjectSystemComponent> spSystemCom, std::shared_ptr<SceneNode> ptrSystemNode, std::shared_ptr<Entity> spStar1Entity, SpaceMapConfigurations& mapConfig)
{
	switch ((*systemTypeDist)(*randomizer)) 
	{
	case 0:
	{
		spSystemCom->systemType = SpaceSystemType::Single;
		GeneratePlanets(ptrSystemNode, mapConfig, -1.f);
		return;
	}
	case 1:
	{
		//Create star in that system
		std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(ptrSystemNode, "Star2").lock();
		spStar2->AddComponent(ComponentType::Star);
		StarTypeGenerator(GetStarComponent(*spStar2));
		spStar2->inheritParentPosition = false;

		//Determine binary system type
		if ((*binarySysDistribution)(*randomizer) == 0)
		{
			spSystemCom->systemType = SpaceSystemType::BinaryClose;
			float distBetStars = (*closeStarsDistances)(*randomizer);
			CalculateBinarySystemProperties(spStar1Entity, spStar2, distBetStars, (*from0to1Dist)(*randomizer), ptrSystemNode);
			GeneratePlanets(ptrSystemNode, mapConfig, distBetStars);
		}
		else
		{
			spSystemCom->systemType = SpaceSystemType::BinaryAfar;
			float distBetStars = (*afarStarsDistances)(*randomizer);
			CalculateBinarySystemProperties(spStar1Entity, spStar2, distBetStars, (*from0to1Dist)(*randomizer), ptrSystemNode);
			GeneratePlanets(ptrSystemNode->FindChild("Star1").lock(), mapConfig, distBetStars);
			GeneratePlanets(ptrSystemNode->FindChild("Star2").lock(), mapConfig, distBetStars);
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
			spInsideSys->AddComponent(ComponentType::ObjectSystem);
			spInsideSys->AddComponent(ComponentType::Star);
			std::shared_ptr<ObjectSystemComponent> spSysCom = GetObjectSystemComponent(*spInsideSys);
			//std::shared_ptr<StarComponent> spInsideSysStarCom = GetStarComponent(*spInsideSys);
			spSysCom->systemType = SpaceSystemType::BinaryCloseWithin;
			spInsideSys->inheritParentPosition = false;
			std::shared_ptr<SceneNode> spInsideSysNode = ptrSystemNode->FindChild("InsideSystem").lock();

			//Create 2 stars in the inside system
			std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(spInsideSysNode, "Star2").lock();
			spStar2->AddComponent(ComponentType::Star);
			StarTypeGenerator(GetStarComponent(*spStar2));
			//spStar2->inheritParentPosition = false;

			std::shared_ptr<Entity> spStar3 = CreateNewEntityAt(spInsideSysNode, "Star3").lock();
			spStar3->AddComponent(ComponentType::Star);
			StarTypeGenerator(GetStarComponent(*spStar3));
			//spStar3->inheritParentPosition = false;

			spSystemCom->systemType = SpaceSystemType::TernaryTwoCloseThirdAfar;
			float closeDistBetStars = (*closeStarsDistances)(*randomizer);
			float afarDistBetStars = (*afarStarsDistances)(*randomizer);
			CalculateBinarySystemProperties(spStar3, spStar2, closeDistBetStars, (*from0to1Dist)(*randomizer), ptrSystemNode);
			CalculateBinarySystemProperties(spStar1Entity, spInsideSys, (*afarStarsDistances)(*randomizer), (*from0to1Dist)(*randomizer), ptrSystemNode);
			
			GeneratePlanets(ptrSystemNode->FindChild("InsideSystem").lock(), mapConfig, closeDistBetStars);
			GeneratePlanets(ptrSystemNode->FindChild("Star1").lock(), mapConfig, afarDistBetStars);
		}
		else
		{
			//All equaly afar

			//Create 2 stars in that system
			std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(ptrSystemNode, "Star2").lock();
			spStar2->AddComponent(ComponentType::Star);
			StarTypeGenerator(GetStarComponent(*spStar2));
			spStar2->inheritParentPosition = false;

			std::shared_ptr<Entity> spStar3 = CreateNewEntityAt(ptrSystemNode, "Star3").lock();
			spStar3->AddComponent(ComponentType::Star);
			StarTypeGenerator(GetStarComponent(*spStar3));
			spStar3->inheritParentPosition = false;

			spSystemCom->systemType = SpaceSystemType::TernaryAfar;
			float distBetStars = (*afarStarsDistances)(*randomizer);
			CalculateTernaryAfarSystemProperties(spStar1Entity, spStar2, spStar3, distBetStars, (*from0to2_3Dist)(*randomizer));
			GeneratePlanets(ptrSystemNode->FindChild("Star1").lock(), mapConfig, distBetStars);
			GeneratePlanets(ptrSystemNode->FindChild("Star2").lock(), mapConfig, distBetStars);
			GeneratePlanets(ptrSystemNode->FindChild("Star3").lock(), mapConfig, distBetStars);
		}

		return;
	}
	}
}


void WorldGenerator::checkRandomDistribution()
{
	std::vector<int> numsDist(13);
	for (int i = 0; i < 1500; i++)
	{
		int num = (*starDistribution)(*randomizer);
		//std::cout << num << "  ";
		numsDist[num]++;
	}
	std::cout <<'\n';
	std::cout << " --- Results ---\n";

	for (int i = 0; i < numsDist.size(); i++) 
	{
		std::cout << i << ") " << numsDist[i] << '\n';
	}
}


void WorldGenerator::GenerateSpaceMap(std::shared_ptr<SceneNode> ptrSpaceMapNode, SpaceMapConfigurations& mapConfig)
{
	if (ptrSpaceMapNode == nullptr)
		std::cout << "Why spaceNode is nullptr?\n";

	//Create all weights
	// Floating point weights for stars
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
	afarOrbitWeights[2] = mapConfig.afarOrbitBarrenChance;
	afarOrbitWeights[3] = mapConfig.afarOrbitNeptuneLikeChance;
	afarOrbitWeights[4] = mapConfig.afarOrbitJupiterLikeChance;

	//Create all distributions
	starDistribution = std::make_shared<std::discrete_distribution<int>>(starWeights.begin(), starWeights.end());
	giantSysDistribution = std::make_shared<std::discrete_distribution<int>>(giantSystemWeights.begin(), giantSystemWeights.end());
	mediumSysDistribution = std::make_shared<std::discrete_distribution<int>>(mediumSystemWeights.begin(), mediumSystemWeights.end());
	dwarfSysDistribution = std::make_shared<std::discrete_distribution<int>>(dwarfsSystemWeights.begin(), dwarfsSystemWeights.end());
	binarySysDistribution = std::make_shared<std::discrete_distribution<int>>(binarySystemWeights.begin(), binarySystemWeights.end());
	ternarySysDistribution = std::make_shared<std::discrete_distribution<int>>(ternarySystemWeights.begin(), ternarySystemWeights.end());
	closeStarsDistances = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.closeStarsBoundaries.x, mapConfig.closeStarsBoundaries.y);
	afarStarsDistances = std::make_shared<std::uniform_real_distribution<float>>(mapConfig.afarStarsBoundaries.x, mapConfig.afarStarsBoundaries.y);
	std::uniform_real_distribution<float> XpositionDist(mapConfig.horizontalPosBoundaries.x,mapConfig.horizontalPosBoundaries.y);
	std::uniform_real_distribution<float> YpositionDist(mapConfig.verticalPosBoundaries.x, mapConfig.verticalPosBoundaries.y);
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

	//Create star positions map
	std::shared_ptr<SystemPropertiesComponent> spSysPropCom = GetSystemPropertiesComponent(*ptrSpaceMapNode->GetEntity().lock());
	//std::unordered_map<int, std::shared_ptr<SceneNode>> systemsPositions;
	//std::vector<bool> starPosGrid(((mapConfig.verticalPosBoundaries.y- mapConfig.verticalPosBoundaries.x)/mapConfig.minDistanceBetweenSystems) * ((mapConfig.horizontalPosBoundaries.y- mapConfig.horizontalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems) + 1);
	int gridWidth = (mapConfig.horizontalPosBoundaries.y - mapConfig.horizontalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems;
	//std::cout << "Grid size: " << starPosGrid.size()<<'\n';
	//std::cout << "Grid width: " << gridWidth << '\n';

	//checkRandomDistribution();

	for (int i=0; i < mapConfig.systemAmount; i++) 
	{
		//std::cout << i <<") " << '\n';
		//Create new system
		std::shared_ptr<Entity> spNewSystem = CreateNewEntityAt(ptrSpaceMapNode, "System"+ std::to_string(i)).lock();
		spNewSystem->AddComponent(ComponentType::ObjectSystem);
		std::shared_ptr<ObjectSystemComponent> spSystemCom = GetObjectSystemComponent(*spNewSystem);
		std::shared_ptr<SceneNode> ptrNewSysNode = ptrSpaceMapNode->FindChild(*spNewSystem).lock();
		
		//Generate position
		bool regeneratePos = true;
		int regenCounter{0};
		while (regeneratePos && regenCounter<mapConfig.maxAmountOfSystemPosRegen)
		{
			sf::Vector2f newPos = sf::Vector2f{ XpositionDist(*randomizer),YpositionDist(*randomizer) };
			
			int yPos = (int)((newPos.y-mapConfig.verticalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems);
			int xPos = (int)((newPos.x- mapConfig.horizontalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems);
			//std::cout << "Ypos: " << yPos <<"; Xpos: " <<xPos<< '\n';

			regeneratePos = false;
			if (GetAllSystemsNearPosition(newPos).size()>0)
				regeneratePos = true;
			else 
			{
				//std::cout << "Set pos\n";
				spSysPropCom->systemsPositions[GetKeyForSystemsPosition(sf::Vector2i{ xPos, yPos })] = ptrNewSysNode;
				spNewSystem->SetPosition(newPos);
			}

			regenCounter++;
		}

		if (regenCounter >= mapConfig.maxAmountOfSystemPosRegen)
			std::cout << i <<") Regenerated system position "<< regenCounter<< " times!\n";

		//Create star in that system
		std::shared_ptr<Entity> spStar1 = CreateNewEntityAt(ptrNewSysNode, "Star1").lock();
		spStar1->AddComponent(ComponentType::Star);
		std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*spStar1);
		spStar1->inheritParentPosition = false;

		switch ((*starDistribution)(*randomizer)) 
		{
		case 0:
			spStar1Com->starType = StarType::RedSupergiant;
			spSystemCom->systemType = SpaceSystemType::Single;
			GeneratePlanets(ptrNewSysNode, mapConfig, -1.f);
			break;
		case 1:
			spStar1Com->starType = StarType::RedGiant;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 2:
			spStar1Com->starType = StarType::Otype;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 3:
			spStar1Com->starType = StarType::Btype;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 4:
			spStar1Com->starType = StarType::Atype;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 5:
			spStar1Com->starType = StarType::Ftype;
			GenerateSystemType(mediumSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 6:
			spStar1Com->starType = StarType::GsunLike;
			GenerateSystemType(mediumSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 7:
			spStar1Com->starType = StarType::KorangeDwarf;
			GenerateSystemType(mediumSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 8:
			spStar1Com->starType = StarType::MredDwarf;
			GenerateSystemType(dwarfSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 9:
			spStar1Com->starType = StarType::BrownDwarf;
			GenerateSystemType(dwarfSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 10:
			spStar1Com->starType = StarType::WhiteDwarf;
			GenerateSystemType(dwarfSysDistribution, spSystemCom, ptrNewSysNode, spStar1, mapConfig);
			break;
		case 11:
			spStar1Com->starType = StarType::NeutronStar;
			spSystemCom->systemType = SpaceSystemType::Single;
			GeneratePlanets(ptrNewSysNode, mapConfig, -1.f);
			break;
		case 12:
			spStar1Com->starType = StarType::BlackHole;
			spSystemCom->systemType = SpaceSystemType::Single;
			break;
		}
	}
}



std::string GetSystemTextureName(StarType starType) 
{
	switch (starType)
	{
	case StarType::BlackHole:
		return "BlackHole";
		break;
	case StarType::NeutronStar:
		return "NeutronStarSystem";
		break;
	case StarType::WhiteDwarf:
		return "WhiteDwarfSystem";
		break;
	case StarType::BrownDwarf:
		return "BrownDwarfSystem";
		break;
	case StarType::MredDwarf:
		return "MclassSystem";
		break;
	case StarType::KorangeDwarf:
		return "KclassSystem";
		break;
	case StarType::GsunLike:
		return "GclassSystem";
		break;
	case StarType::Ftype:
		return "FclassSystem";
		break;
	case StarType::Atype:
		return "AclassSystem";
		break;
	case StarType::Btype:
		return "BclassSystem";
		break;
	case StarType::Otype:
		return "OclassSystem";
		break;
	case StarType::RedGiant:
		return "RedGiantSystem";
		break;
	case StarType::RedSupergiant:
		return "RedSupergiantSystem";
		break;
	}

	return "Placeholder";
}



void TextureSetter::SetSystemTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType)
{
	SetupRectangleShape(spRectShape, mapConfig.systemEntitySize, GetSystemTextureName(starType));
	//"media/textures/starsPicture.png"
}


void TextureSetter::SetStarTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType)
{
	sf::Vector2i pictureSize{ 300,300 };
	//sf::Vector2i textureGrid{ 3,4 };
	std::string textureName{ "Placeholder" };

	float starSizeMultiplier = 9.f;
	switch (starType)
	{
	case StarType::BlackHole:
		textureName = "BlackHole";
		break;
	case StarType::NeutronStar:
		textureName = "NeutronStar";
		break;
	case StarType::WhiteDwarf:
		textureName = "WhiteDwarf";
		break;
	case StarType::BrownDwarf:
		textureName = "BrownDwarf";
		break;
	case StarType::MredDwarf:
		textureName = "Mclass";
		break;
	case StarType::KorangeDwarf:
		textureName = "Kclass";
		break;
	case StarType::GsunLike:
		textureName = "Gclass";
		break;
	case StarType::Ftype:
		textureName = "Fclass";
		break;
	case StarType::Atype:
		textureName = "Aclass";
		break;
	case StarType::Btype:
		textureName = "Bclass";
		break;
	case StarType::Otype:
		textureName = "Oclass";
		break;
	case StarType::RedGiant:
		starSizeMultiplier *= 20.f;
		textureName = "RedGiant";
		break;
	case StarType::RedSupergiant:
		starSizeMultiplier *= 100.f;
		textureName = "RedGiant";
		break;
	}

	SetupRectangleShape(spRectShape, sf::Vector2f{ mapConfig.sunDiameter,mapConfig.sunDiameter } * starSizeMultiplier, textureName);
	//"media/textures/starsPicture.png"
}


void TextureSetter::SetSystemName(std::shared_ptr<ObjectSystemComponent> spSpaceSys, StarType starType) 
{
	//0 - dim, 1 - medium, 2 - bright
	int starBrightness{ 1 };
	switch (starType)
	{
	case StarType::BlackHole:
		starBrightness = 0;
		break;
	case StarType::NeutronStar:
		starBrightness = 0;
		break;
	case StarType::WhiteDwarf:
		starBrightness = 0;
		break;
	case StarType::BrownDwarf:
		starBrightness = 0;
		break;
	case StarType::MredDwarf:
		starBrightness = 1;
		break;
	case StarType::KorangeDwarf:
		starBrightness = 2;
		break;
	case StarType::GsunLike:
		starBrightness = 2;
		break;
	case StarType::Ftype:
		starBrightness = 2;
		break;
	case StarType::Atype:
		starBrightness = 2;
		break;
	case StarType::Btype:
		starBrightness = 2;
		break;
	case StarType::Otype:
		starBrightness = 2;
		break;
	case StarType::RedGiant:
		starBrightness = 2;
		break;
	case StarType::RedSupergiant:
		starBrightness = 2;
		break;
	}

	std::shared_ptr<std::uniform_int_distribution<int>> nameDist;

	if (starBrightness == 0) 
	{
		if (listOfDimStarNames.size() <= 0) 
		{
			std::cout << "Run out of names for dim systems!\n";
			spSpaceSys->systemName = "N_D";
		}
		else 
		{
			nameDist = std::make_shared<std::uniform_int_distribution<int>>(0, listOfDimStarNames.size()-1);
			int pos = (*nameDist)(*randomizer);
			spSpaceSys->systemName = listOfDimStarNames[pos];

			// Move last element into removed position
			listOfDimStarNames[pos] = listOfDimStarNames.back();
			listOfDimStarNames.pop_back();
		}
	}
	else if (starBrightness == 1) 
	{
		if (listOfMediumStarNames.size() <= 0)
		{
			std::cout << "Run out of names for medium systems!\n";
			spSpaceSys->systemName = "N_M";
		}
		else
		{
			nameDist = std::make_shared<std::uniform_int_distribution<int>>(0, listOfMediumStarNames.size() - 1);
			int pos = (*nameDist)(*randomizer);
			spSpaceSys->systemName = listOfMediumStarNames[pos];

			// Move last element into removed position
			listOfMediumStarNames[pos] = listOfMediumStarNames.back();
			listOfMediumStarNames.pop_back();
		}
	}
	else
	{
		if (listOfBrightStarNames.size() <= 0)
		{
			std::cout << "Run out of names for bright systems!\n";
			if (listOfMediumStarNames.size() <= 0)
			{
				std::cout << "Run out of names for medium systems!\n";
				spSpaceSys->systemName = "N_M";
			}
			else
			{
				nameDist = std::make_shared<std::uniform_int_distribution<int>>(0, listOfMediumStarNames.size() - 1);
				int pos = (*nameDist)(*randomizer);
				spSpaceSys->systemName = listOfMediumStarNames[pos];

				// Move last element into removed position
				listOfMediumStarNames[pos] = listOfMediumStarNames.back();
				listOfMediumStarNames.pop_back();
			}
		}
		else
		{
			nameDist = std::make_shared<std::uniform_int_distribution<int>>(0, listOfBrightStarNames.size() - 1);
			int pos = (*nameDist)(*randomizer);
			spSpaceSys->systemName = listOfBrightStarNames[pos];

			// Move last element into removed position
			listOfBrightStarNames[pos] = listOfBrightStarNames.back();
			listOfBrightStarNames.pop_back();
		}
	}
}


TextureSetter::TextureSetter(unsigned int seedOut) : seed{seedOut}
{
	seed = seedOut;
	randomizer = std::make_shared<std::mt19937>(std::mt19937{ seed });

	listOfBrightStarNames = ReadStarNamesFromCSV("media/other/big_stars_names_240.csv");
	listOfMediumStarNames = ReadStarNamesFromCSV("media/other/star_names_5000.csv");
	listOfDimStarNames = ReadStarNamesFromCSV("media/other/small_stars_names_1000.csv");

	wpSystemNamesNode = ECSGame::Instance().GetUIRoot()->FindChild("SystemNames").lock();
	//std::cout << "Num of bright names:" << listOfBrightStarNames.size() << '\n';
	//std::cout << "Num of medium names:" << listOfMediumStarNames.size() << '\n';
	//std::cout << "Num of dim names:" << listOfDimStarNames.size() << '\n';
}



void TextureSetter::ProcessNode(SceneNode& node) 
{
	std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
	//Check if pointer is valid
	if (spEntity != nullptr)
	{
		//Check if entity has system component
		if (spEntity->HasComponent(ComponentType::ObjectSystem) && spEntity->GetName()!= "InsideSystem")
		{
			std::shared_ptr<ObjectSystemComponent> spComSys = GetObjectSystemComponent(*spEntity);

			spEntity->AddComponent(ComponentType::RectangleShape);
			std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spEntity);
			
			std::shared_ptr<SceneNode> ptrSysNode = wpSpaceMapNode.lock()->FindChild(*spEntity).lock();
			if (spComSys->systemType == SpaceSystemType::Single) 
			{
				std::shared_ptr<SceneNode> ptrStar1Node = ptrSysNode->FindChild("Star1").lock();
				std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
				SetSystemTexture(spRectShape, spStar1Com->starType);
				SetSystemName(spComSys, spStar1Com->starType);

				//Now set star names
				spStar1Com->starName = spComSys->systemName;
			}
			else if (spComSys->systemType == SpaceSystemType::BinaryClose || spComSys->systemType == SpaceSystemType::BinaryAfar) 
			{
				//std::cout << "Binary\n";
				std::shared_ptr<SceneNode> ptrStar1Node = ptrSysNode->FindChild("Star1").lock();
				std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
				std::shared_ptr<SceneNode> ptrStar2Node = ptrSysNode->FindChild("Star2").lock();
				std::shared_ptr<StarComponent> spStar2Com = GetStarComponent(*ptrStar2Node->GetEntity().lock());

				SetSystemTexture(spRectShape, std::max(spStar1Com->starType, spStar2Com->starType));
				SetSystemName(spComSys, std::max(spStar1Com->starType, spStar2Com->starType));
			
				//Now set star names
				if (spStar1Com->starType > spStar2Com->starType) 
				{
					spStar1Com->starName = spComSys->systemName+"-A";
					spStar2Com->starName = spComSys->systemName + "-B";
				}
				else 
				{
					spStar2Com->starName = spComSys->systemName + "-A";
					spStar1Com->starName = spComSys->systemName + "-B";
				}
			}
			else 
			{
				std::shared_ptr<StarComponent> spStar1Com;
				std::shared_ptr<StarComponent> spStar2Com;
				std::shared_ptr<StarComponent> spStar3Com;

				if (spComSys->systemType == SpaceSystemType::TernaryAfar) 
				{
					//std::cout << "Ternary afar\n";
					std::shared_ptr<SceneNode> ptrStar1Node = ptrSysNode->FindChild("Star1").lock();
					spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
					std::shared_ptr<SceneNode> ptrStar2Node = ptrSysNode->FindChild("Star2").lock();
					spStar2Com = GetStarComponent(*ptrStar2Node->GetEntity().lock());
					std::shared_ptr<SceneNode> ptrStar3Node = ptrSysNode->FindChild("Star3").lock();
					spStar3Com = GetStarComponent(*ptrStar3Node->GetEntity().lock());
				}
				else 
				{
					//std::cout << "Ternary binary\n";
					std::shared_ptr<SceneNode> ptrStar1Node = ptrSysNode->FindChild("Star1").lock();
					spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
					std::shared_ptr<SceneNode> ptrInsideSysNode = ptrSysNode->FindChild("InsideSystem").lock();
					std::shared_ptr<SceneNode> ptrStar2Node = ptrInsideSysNode->FindChild("Star2").lock();
					spStar2Com = GetStarComponent(*ptrStar2Node->GetEntity().lock());
					std::shared_ptr<SceneNode> ptrStar3Node = ptrInsideSysNode->FindChild("Star3").lock();
					spStar3Com = GetStarComponent(*ptrStar3Node->GetEntity().lock());
				}

				SetSystemTexture(spRectShape, std::max(std::max(spStar1Com->starType, spStar2Com->starType), spStar3Com->starType));
				SetSystemName(spComSys, std::max(std::max(spStar1Com->starType, spStar2Com->starType), spStar3Com->starType));

				//Now set star names
				if (spStar1Com->starType > spStar2Com->starType && spStar1Com->starType > spStar3Com->starType)
				{
					spStar1Com->starName = spComSys->systemName + "-A";

					if (spStar2Com->starType > spStar3Com->starType)
					{
						spStar2Com->starName = spComSys->systemName + "-B";
						spStar3Com->starName = spComSys->systemName + "-C";
					}
					else
					{
						spStar3Com->starName = spComSys->systemName + "-B";
						spStar2Com->starName = spComSys->systemName + "-C";
					}
				}
				else if(spStar2Com->starType > spStar1Com->starType && spStar2Com->starType > spStar3Com->starType)
				{
					spStar2Com->starName = spComSys->systemName + "-A";

					if (spStar1Com->starType > spStar3Com->starType)
					{
						spStar1Com->starName = spComSys->systemName + "-B";
						spStar3Com->starName = spComSys->systemName + "-C";
					}
					else
					{
						spStar3Com->starName = spComSys->systemName + "-B";
						spStar1Com->starName = spComSys->systemName + "-C";
					}
				}
				else
				{
					spStar3Com->starName = spComSys->systemName + "-A";

					if (spStar1Com->starType > spStar2Com->starType)
					{
						spStar1Com->starName = spComSys->systemName + "-B";
						spStar2Com->starName = spComSys->systemName + "-C";
					}
					else
					{
						spStar2Com->starName = spComSys->systemName + "-B";
						spStar1Com->starName = spComSys->systemName + "-C";
					}
				}
			}

			//Create text name entity for system
			std::string name{ "SystemNameText" };
			CreateSystemText(wpSystemNamesNode.lock(), node.GetSharedPtrToItself(), name, true);
		}
		else if (spEntity->HasComponent(ComponentType::Star))
		{
			std::shared_ptr<StarComponent> spStar = GetStarComponent(*spEntity);
			spEntity->AddComponent(ComponentType::RectangleShape);
			std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spEntity);
			spEntity->hidden = true;

			SetStarTexture(spRectShape, spStar->starType);
		}
	}
}