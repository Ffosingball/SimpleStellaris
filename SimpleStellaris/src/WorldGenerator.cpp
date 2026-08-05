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


/*void CreateSystemInStar(std::weak_ptr<SceneNode> wpSystemNode, std::shared_ptr<Entity> spStarEntity, std::string name)
{
	//Create system for star
	std::shared_ptr<SceneNode> spStar1Node = std::make_shared<SceneNode>(wpSystemNode.lock()->FindChild(*spStarEntity));
	std::shared_ptr<Entity> spNewSystem1 = CreateNewEntityAt(spStar1Node, name).lock();
	spNewSystem1->AddComponent(ComponentType::ObjectSystem);
	std::shared_ptr<ObjectSystemComponent> spSystem1Com = GetObjectSystemComponent(*spNewSystem1);
	spSystem1Com->systemType = SpaceSystemType::Single;
}*/


void WorldGenerator::GenerateSystemType(std::shared_ptr<std::discrete_distribution<int>> systemTypeDist, std::shared_ptr<ObjectSystemComponent> spSystemCom, std::shared_ptr<SceneNode> ptrSystemNode, std::shared_ptr<Entity> spStar1Entity)
{
	switch ((*systemTypeDist)(*randomizer)) 
	{
	case 0:
	{
		spSystemCom->systemType = SpaceSystemType::Single;
		return;
	}
	case 1:
	{
		//Create star in that system
		std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(ptrSystemNode, "Star2").lock();
		spStar2->AddComponent(ComponentType::Star);
		StarTypeGenerator(GetStarComponent(*spStar2));

		//Determine binary system type
		if ((*binarySysDistribution)(*randomizer) == 0)
		{
			spSystemCom->systemType = SpaceSystemType::BinaryClose;
			spSystemCom->distTo2ndStar = (*closeStarsDistances)(*randomizer);
		}
		else
		{
			spSystemCom->systemType = SpaceSystemType::BinaryAfar;
			spSystemCom->distTo3rdStar = (*afarStarsDistances)(*randomizer);
		}
		return;
	}
	case 2:
	{
		//Create 2 stars in that system
		std::shared_ptr<Entity> spStar2 = CreateNewEntityAt(ptrSystemNode, "Star2").lock();
		spStar2->AddComponent(ComponentType::Star);
		StarTypeGenerator(GetStarComponent(*spStar2));

		std::shared_ptr<Entity> spStar3 = CreateNewEntityAt(ptrSystemNode, "Star3").lock();
		spStar3->AddComponent(ComponentType::Star);
		StarTypeGenerator(GetStarComponent(*spStar3));

		//Determine ternary system type
		int val = (*ternarySysDistribution)(*randomizer);
		if (val==0)
		{
			spSystemCom->systemType = SpaceSystemType::TernaryClose;
			spSystemCom->distTo2ndStar = (*closeStarsDistances)(*randomizer);
			spSystemCom->distTo3rdStar = (*closeStarsDistances)(*randomizer);
		}
		else if (val == 1) 
		{
			spSystemCom->systemType = SpaceSystemType::TernaryTwoCloseThirdAfar;
			spSystemCom->distTo2ndStar = (*closeStarsDistances)(*randomizer);
			spSystemCom->distTo3rdStar = (*afarStarsDistances)(*randomizer);

			spSystemCom->starAfar = (*oneThird)(*randomizer);
		}
		else 
		{
			spSystemCom->systemType = SpaceSystemType::TernaryAfar;
			spSystemCom->distTo2ndStar = (*afarStarsDistances)(*randomizer);
			spSystemCom->distTo3rdStar = (*afarStarsDistances)(*randomizer);
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

	std::vector<float> ternarySystemWeights(3);
	ternarySystemWeights[0] = mapConfig.closeTernaryChance;
	ternarySystemWeights[1] = mapConfig.afarCloseBinaryThirdAfarChance;
	ternarySystemWeights[2] = mapConfig.afarTernaryChance;

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

		switch ((*starDistribution)(*randomizer)) 
		{
		case 0:
			spStar1Com->starType = StarType::RedSupergiant;
			spSystemCom->systemType = SpaceSystemType::Single;
			break;
		case 1:
			spStar1Com->starType = StarType::RedGiant;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 2:
			spStar1Com->starType = StarType::Otype;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 3:
			spStar1Com->starType = StarType::Btype;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 4:
			spStar1Com->starType = StarType::Atype;
			GenerateSystemType(giantSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 5:
			spStar1Com->starType = StarType::Ftype;
			GenerateSystemType(mediumSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 6:
			spStar1Com->starType = StarType::GsunLike;
			GenerateSystemType(mediumSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 7:
			spStar1Com->starType = StarType::KorangeDwarf;
			GenerateSystemType(mediumSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 8:
			spStar1Com->starType = StarType::MredDwarf;
			GenerateSystemType(dwarfSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 9:
			spStar1Com->starType = StarType::BrownDwarf;
			GenerateSystemType(dwarfSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 10:
			spStar1Com->starType = StarType::WhiteDwarf;
			GenerateSystemType(dwarfSysDistribution, spSystemCom, ptrNewSysNode, spStar1);
			break;
		case 11:
			spStar1Com->starType = StarType::NeutronStar;
			spSystemCom->systemType = SpaceSystemType::Single;
			break;
		case 12:
			spStar1Com->starType = StarType::BlackHole;
			spSystemCom->systemType = SpaceSystemType::Single;
			break;
		}
	}
}



void TextureSetter::SetSystemTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType)
{
	std::string textureName{ "Placeholder" };
	switch (starType) 
	{
	case StarType::BlackHole:
		textureName = "BrownDwarfSystem";
		break;
	case StarType::NeutronStar:
		textureName = "NeutronStarSystem";
		break;
	case StarType::WhiteDwarf:
		textureName = "WhiteDwarfSystem";
		break;
	case StarType::BrownDwarf:
		textureName = "BrownDwarfSystem";
		break;
	case StarType::MredDwarf:
		textureName = "MclassSystem";
		break;
	case StarType::KorangeDwarf:
		textureName = "KclassSystem";
		break;
	case StarType::GsunLike:
		textureName = "GclassSystem";
		break;
	case StarType::Ftype:
		textureName = "FclassSystem";
		break;
	case StarType::Atype:
		textureName = "AclassSystem";
		break;
	case StarType::Btype:
		textureName = "BclassSystem";
		break;
	case StarType::Otype:
		textureName = "OclassSystem";
		break;
	case StarType::RedGiant:
		textureName = "RedGiantSystem";
		break;
	case StarType::RedSupergiant:
		textureName = "RedSupergiantSystem";
		break;
	}

	SetupRectangleShape(spRectShape, mapConfig.systemEntitySize, textureName, OverviewType::Space);
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

	SetupRectangleShape(spRectShape, sf::Vector2f{ mapConfig.sunDiameter*starSizeMultiplier,mapConfig.sunDiameter }, textureName, OverviewType::System);
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

	ptrSystemNamesNode = ECSGame::Instance().GetUIRoot()->FindChild("SystemNames").lock();
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
		if (spEntity->HasComponent(ComponentType::ObjectSystem))
		{
			std::shared_ptr<ObjectSystemComponent> spComSys = GetObjectSystemComponent(*spEntity);

			spEntity->AddComponent(ComponentType::RectangleShape);
			std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spEntity);
			
			std::shared_ptr<SceneNode> ptrSysNode = ptrSpaceMapNode->FindChild(*spEntity).lock();
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
				std::shared_ptr<SceneNode> ptrStar1Node = ptrSysNode->FindChild("Star1").lock();
				std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
				std::shared_ptr<SceneNode> ptrStar2Node = ptrSysNode->FindChild("Star2").lock();
				std::shared_ptr<StarComponent> spStar2Com = GetStarComponent(*ptrStar2Node->GetEntity().lock());
				std::shared_ptr<SceneNode> ptrStar3Node = ptrSysNode->FindChild("Star3").lock();
				std::shared_ptr<StarComponent> spStar3Com = GetStarComponent(*ptrStar3Node->GetEntity().lock());

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
			CreateSystemText(ptrSystemNamesNode, spEntity, OverviewType::Space, name);
		}
		else if (spEntity->HasComponent(ComponentType::Star))
		{
			std::shared_ptr<StarComponent> spStar = GetStarComponent(*spEntity);
			spEntity->AddComponent(ComponentType::RectangleShape);
			std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spEntity);

			SetStarTexture(spRectShape, spStar->starType);
			//Create text name entity for star
			std::string name{ "StarNameText" };
			CreateSystemText(ptrSystemNamesNode, spEntity, OverviewType::System, name);
		}
	}
}