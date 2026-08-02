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


void WorldGenerator::GenerateSystemType(std::shared_ptr<std::discrete_distribution<int>> systemTypeDist, std::shared_ptr<ObjectSystemComponent> spSystemCom, SceneNode* ptrSystemNode, std::shared_ptr<Entity> spStar1Entity)
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


void WorldGenerator::GenerateSpaceMap(SceneNode* ptrSpaceMapNode, SpaceMapConfigurations& mapConfig)
{
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

	//Create star positions grid
	//False empty, true not empty
	std::vector<bool> starPosGrid(((mapConfig.verticalPosBoundaries.y- mapConfig.verticalPosBoundaries.x)/mapConfig.minDistanceBetweenSystems) * ((mapConfig.horizontalPosBoundaries.y- mapConfig.horizontalPosBoundaries.x) / mapConfig.minDistanceBetweenSystems) + 1);
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
		SceneNode* ptrNewSysNode = ptrSpaceMapNode->FindChild(*spNewSystem);
		
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
			if (starPosGrid[(yPos * gridWidth) + xPos])
				regeneratePos = true;
			
			if (!regeneratePos && yPos < (int)(mapConfig.verticalPosBoundaries.y / mapConfig.minDistanceBetweenSystems) - 1)
			{
				if (!regeneratePos &&  xPos < (int)(mapConfig.horizontalPosBoundaries.y / mapConfig.minDistanceBetweenSystems) - 1)
				{
					if (starPosGrid[((yPos + 1) * gridWidth) + xPos + 1])
						regeneratePos = true;
				}
				
				if (!regeneratePos &&  starPosGrid[((yPos + 1) * gridWidth) + xPos])
					regeneratePos = true;
				
				if (!regeneratePos &&  xPos > 0)
				{
					if (starPosGrid[((yPos + 1) * gridWidth) + xPos - 1])
						regeneratePos = true;
				}
			}
			
			if (!regeneratePos &&  xPos < (int)(mapConfig.horizontalPosBoundaries.y / mapConfig.minDistanceBetweenSystems) - 1)
			{
				if (starPosGrid[(yPos * gridWidth) + xPos + 1])
					regeneratePos = true;
			}
			
			if (!regeneratePos &&  xPos > 0)
			{
				if (starPosGrid[(yPos * gridWidth) + xPos - 1])
					regeneratePos = true;
			}
			
			if (!regeneratePos &&  yPos > 0)
			{
				if (!regeneratePos &&  xPos < (int)(mapConfig.horizontalPosBoundaries.y / mapConfig.minDistanceBetweenSystems) - 1)
				{
					if (starPosGrid[((yPos - 1) * gridWidth) + xPos + 1])
						regeneratePos = true;
				}
				
				if (!regeneratePos &&  starPosGrid[((yPos - 1) * gridWidth) + xPos])
					regeneratePos = true;

				if (!regeneratePos &&  xPos > 0)
				{
					if (starPosGrid[((yPos - 1) * gridWidth) + xPos - 1])
						regeneratePos = true;
				}
			}

			if(!regeneratePos)
			{
				//std::cout << "Set pos\n";
				starPosGrid[(yPos * gridWidth) + xPos] = true;
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
	sf::Vector2i pictureSize{17,17};
	//sf::Vector2i textureGrid{ 3,4 };
	std::string texturePath{ "media/textures/starsPicture.png" };

	sf::IntRect intRect;
	intRect.size = pictureSize;
	switch (starType) 
	{
	case StarType::BlackHole:
		intRect.position = sf::Vector2i{34,0};
		break;
	case StarType::NeutronStar:
		intRect.position = sf::Vector2i{ 0,0 };
		break;
	case StarType::WhiteDwarf:
		intRect.position = sf::Vector2i{ 17,0 };
		break;
	case StarType::BrownDwarf:
		intRect.position = sf::Vector2i{ 34,0 };
		break;
	case StarType::MredDwarf:
		intRect.position = sf::Vector2i{ 0,17 };
		break;
	case StarType::KorangeDwarf:
		intRect.position = sf::Vector2i{ 17,17 };
		break;
	case StarType::GsunLike:
		intRect.position = sf::Vector2i{ 34,17 };
		break;
	case StarType::Ftype:
		intRect.position = sf::Vector2i{ 0,34 };
		break;
	case StarType::Atype:
		intRect.position = sf::Vector2i{ 17,34 };
		break;
	case StarType::Btype:
		intRect.position = sf::Vector2i{ 34,34 };
		break;
	case StarType::Otype:
		intRect.position = sf::Vector2i{ 0,51 };
		break;
	case StarType::RedGiant:
		intRect.position = sf::Vector2i{ 17,51 };
		break;
	case StarType::RedSupergiant:
		intRect.position = sf::Vector2i{ 34,51 };
		break;
	}

	SetupRectangleShape(spRectShape, mapConfig.systemEntitySize, texturePath, intRect);
	//"media/textures/starsPicture.png"
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
			
			SceneNode* ptrSysNode = ptrSpaceMapNode->FindChild(*spEntity);
			if (spComSys->systemType == SpaceSystemType::Single) 
			{
				SceneNode* ptrStar1Node = ptrSysNode->FindChild("Star1");
				std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
				SetSystemTexture(spRectShape, spStar1Com->starType);
			}
			else if (spComSys->systemType == SpaceSystemType::BinaryClose || spComSys->systemType == SpaceSystemType::BinaryAfar) 
			{
				SceneNode* ptrStar1Node = ptrSysNode->FindChild("Star1");
				std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
				SceneNode* ptrStar2Node = ptrSysNode->FindChild("Star2");
				std::shared_ptr<StarComponent> spStar2Com = GetStarComponent(*ptrStar2Node->GetEntity().lock());

				SetSystemTexture(spRectShape, std::max(spStar1Com->starType, spStar2Com->starType));
			}
			else 
			{
				SceneNode* ptrStar1Node = ptrSysNode->FindChild("Star1");
				std::shared_ptr<StarComponent> spStar1Com = GetStarComponent(*ptrStar1Node->GetEntity().lock());
				SceneNode* ptrStar2Node = ptrSysNode->FindChild("Star2");
				std::shared_ptr<StarComponent> spStar2Com = GetStarComponent(*ptrStar2Node->GetEntity().lock());
				SceneNode* ptrStar3Node = ptrSysNode->FindChild("Star3");
				std::shared_ptr<StarComponent> spStar3Com = GetStarComponent(*ptrStar3Node->GetEntity().lock());

				SetSystemTexture(spRectShape, std::max(std::max(spStar1Com->starType, spStar2Com->starType), spStar3Com->starType));
			}
		}
	}
}