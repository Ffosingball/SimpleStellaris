#include "WorldGenerator.h"
#include "EntitiesFunctions.h"
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SceneNode.h"
#include <memory>
#include "Components.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "SpaceObjectTypes.h"
#include <unordered_map>
#include <rapidcsv.h>
#include <functional>
#include "SceneNodeVisitors.h"
#include "GetTextureNameFunctions.h"


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


void TextureAndNameSetter::SetSystemTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType)
{
	SetupRectangleShape(spRectShape, WorldGenerator::Instance().getSpaceMapConfig().systemEntitySize, GetSystemTextureName(starType));
}


void TextureAndNameSetter::SetStarTexture(std::shared_ptr<RectangleShapeComponent> spRectShape, StarType starType, float starSize)
{
	sf::Vector2i pictureSize{ 300,300 };
	std::string textureName{ "Placeholder" };

	//float starSizeMultiplier = 9.f;
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
		textureName = "RedGiant";
		break;
	case StarType::RedSupergiant:
		textureName = "RedGiant";
		break;
	}

	float starSizeMultiplier = 1.34f;
	SetupRectangleShape(spRectShape, sf::Vector2f{ static_cast<float>(WorldGenerator::Instance().getSpaceMapConfig().sunDiameter), static_cast<float>(WorldGenerator::Instance().getSpaceMapConfig().sunDiameter) } * starSize * starSizeMultiplier, textureName);
}


void TextureAndNameSetter::SetSystemName(std::shared_ptr<ObjectSystemComponent> spSpaceSys, StarType starType)
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
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
			std::cout << "Run out of names for dim systems!\n";
#endif
			spSpaceSys->systemName = "N_D";
		}
		else
		{
			nameDist = std::make_shared<std::uniform_int_distribution<int>>(0, listOfDimStarNames.size() - 1);
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
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
			std::cout << "Run out of names for medium systems!\n";
#endif
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
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
			std::cout << "Run out of names for bright systems!\n";
#endif
			if (listOfMediumStarNames.size() <= 0)
			{
#ifdef OUTPUT_WORLD_GENERATION_MESSAGES
				std::cout << "Run out of names for medium systems!\n";
#endif
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


TextureAndNameSetter::TextureAndNameSetter(unsigned int seedOut, std::weak_ptr<SceneNode> wpSystemNamesNode) : seed{ seedOut }, wpSystemNamesNode{ wpSystemNamesNode }
{
	processHiddenNode = true;

	seed = seedOut;
	randomizer = std::make_shared<std::mt19937>(std::mt19937{ seed });

	listOfBrightStarNames = ReadStarNamesFromCSV("media/other/big_stars_names_1240.csv");
	listOfMediumStarNames = ReadStarNamesFromCSV("media/other/star_names_5000.csv");
	listOfDimStarNames = ReadStarNamesFromCSV("media/other/small_stars_names_1000.csv");
}



void SetPlanetName(std::shared_ptr<SceneNode> spNodeWithPlanets, std::string name, int firstASCIIchar)
{
	std::vector<std::shared_ptr<SceneNode>> children = spNodeWithPlanets->GetAllChildren();
	if (!children.empty())
	{
		SortedPlanetComponentsList sortedPlanets;
		for (std::shared_ptr<SceneNode> child : children)
		{
			if (child->GetEntity().lock()->HasComponent<PlanetComponent>())
				sortedPlanets.AddPlanetComponent(child->GetEntity().lock()->FindComponent<PlanetComponent>().lock());
		}

		int counter = firstASCIIchar;
		while (sortedPlanets.Size() > 0)
		{
			sortedPlanets.DequeuePlanetComponent().lock()->planetName = name + '-' + static_cast<char>(counter);
			counter++;
		}
	}
}


std::string GetRomanNumber(int number)
{
	switch (number)
	{
	case 1:
		return "I";
	case 2:
		return "II";
	case 3:
		return "III";
	case 4:
		return "IV";
	case 5:
		return "V";
	case 6:
		return "VI";
	case 7:
		return "VII";
	case 8:
		return "VIII";
	}

	return "UNDEFINED";
}


void TextureAndNameSetter::ProcessNode(SceneNode& node)
{
	std::shared_ptr<Entity> spEntity = node.GetEntity().lock();
	//Check if pointer is valid
	if (spEntity != nullptr)
	{
		//Check if entity has system component
		if (spEntity->HasComponent<ObjectSystemComponent>())
		{
			std::shared_ptr<ObjectSystemComponent> spComSys = spEntity->FindComponent<ObjectSystemComponent>().lock();
			if (spComSys->spAllSystemObjectsNode != nullptr)
			{
				std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->AddComponent<RectangleShapeComponent>().lock();

				if (spComSys->systemType == SpaceSystemType::Single)
				{
					std::shared_ptr<SceneNode> ptrStar1Node = spComSys->spAllSystemObjectsNode->FindChild("Star1").lock();
					std::shared_ptr<StarComponent> spStar1Com = ptrStar1Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
					SetSystemTexture(spRectShape, spStar1Com->starType);
					SetSystemName(spComSys, spStar1Com->starType);

					//Now set star names
					spStar1Com->starName = spComSys->systemName;

					//Now set planet names
					SetPlanetName(ptrStar1Node, spStar1Com->starName, 97);
				}
				else if (spComSys->systemType == SpaceSystemType::BinaryClose || spComSys->systemType == SpaceSystemType::BinaryAfar)
				{
					std::shared_ptr<SceneNode> ptrStar1Node = spComSys->spAllSystemObjectsNode->FindChild("Star1").lock();
					std::shared_ptr<StarComponent> spStar1Com = ptrStar1Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
					std::shared_ptr<SceneNode> ptrStar2Node = spComSys->spAllSystemObjectsNode->FindChild("Star2").lock();
					std::shared_ptr<StarComponent> spStar2Com = ptrStar2Node->GetEntity().lock()->FindComponent<StarComponent>().lock();

					SetSystemTexture(spRectShape, std::max(spStar1Com->starType, spStar2Com->starType));
					SetSystemName(spComSys, std::max(spStar1Com->starType, spStar2Com->starType));

					//Now set star names
					if (spStar1Com->starType > spStar2Com->starType)
					{
						spStar1Com->starName = spComSys->systemName + " A";
						spStar2Com->starName = spComSys->systemName + " B";
					}
					else
					{
						spStar2Com->starName = spComSys->systemName + " A";
						spStar1Com->starName = spComSys->systemName + " B";
					}

					if (spComSys->systemType == SpaceSystemType::BinaryClose)
					{
						//Now set planet names
						SetPlanetName(spComSys->spAllSystemObjectsNode, spComSys->systemName, 99);
					}
					else
					{
						//Now set planet names
						SetPlanetName(ptrStar1Node, spStar1Com->starName, 97);
						SetPlanetName(ptrStar2Node, spStar2Com->starName, 97);
					}
				}
				else
				{
					std::shared_ptr<StarComponent> spStar1Com;
					std::shared_ptr<StarComponent> spStar2Com;
					std::shared_ptr<StarComponent> spStar3Com;

					if (spComSys->systemType == SpaceSystemType::TernaryAfar)
					{
						std::shared_ptr<SceneNode> ptrStar1Node = spComSys->spAllSystemObjectsNode->FindChild("Star1").lock();
						spStar1Com = ptrStar1Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
						std::shared_ptr<SceneNode> ptrStar2Node = spComSys->spAllSystemObjectsNode->FindChild("Star2").lock();
						spStar2Com = ptrStar2Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
						std::shared_ptr<SceneNode> ptrStar3Node = spComSys->spAllSystemObjectsNode->FindChild("Star3").lock();
						spStar3Com = ptrStar3Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
					}
					else
					{
						std::shared_ptr<SceneNode> ptrStar1Node = spComSys->spAllSystemObjectsNode->FindChild("Star1").lock();
						spStar1Com = ptrStar1Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
						std::shared_ptr<SceneNode> ptrInsideSysNode = spComSys->spAllSystemObjectsNode->FindChild("InsideSystem").lock();
						std::shared_ptr<SceneNode> ptrStar2Node = ptrInsideSysNode->FindChild("Star2").lock();
						spStar2Com = ptrStar2Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
						std::shared_ptr<SceneNode> ptrStar3Node = ptrInsideSysNode->FindChild("Star3").lock();
						spStar3Com = ptrStar3Node->GetEntity().lock()->FindComponent<StarComponent>().lock();
					}

					SetSystemTexture(spRectShape, std::max(std::max(spStar1Com->starType, spStar2Com->starType), spStar3Com->starType));
					SetSystemName(spComSys, std::max(std::max(spStar1Com->starType, spStar2Com->starType), spStar3Com->starType));

					//Now set star names
					if (spStar1Com->starType > spStar2Com->starType && spStar1Com->starType > spStar3Com->starType)
					{
						spStar1Com->starName = spComSys->systemName + " A";

						if (spStar2Com->starType > spStar3Com->starType)
						{
							spStar2Com->starName = spComSys->systemName + " B";
							spStar3Com->starName = spComSys->systemName + " C";
						}
						else
						{
							spStar3Com->starName = spComSys->systemName + " B";
							spStar2Com->starName = spComSys->systemName + " C";
						}
					}
					else if (spStar2Com->starType > spStar1Com->starType && spStar2Com->starType > spStar3Com->starType)
					{
						spStar2Com->starName = spComSys->systemName + " A";

						if (spStar1Com->starType > spStar3Com->starType)
						{
							spStar1Com->starName = spComSys->systemName + " B";
							spStar3Com->starName = spComSys->systemName + " C";
						}
						else
						{
							spStar3Com->starName = spComSys->systemName + " B";
							spStar1Com->starName = spComSys->systemName + " C";
						}
					}
					else
					{
						spStar3Com->starName = spComSys->systemName + " A";

						if (spStar1Com->starType > spStar2Com->starType)
						{
							spStar1Com->starName = spComSys->systemName + " B";
							spStar2Com->starName = spComSys->systemName + " C";
						}
						else
						{
							spStar2Com->starName = spComSys->systemName + " B";
							spStar1Com->starName = spComSys->systemName + " C";
						}
					}


					if (spComSys->systemType == SpaceSystemType::TernaryAfar)
					{
						std::shared_ptr<SceneNode> ptrStar1Node = spComSys->spAllSystemObjectsNode->FindChild("Star1").lock();
						std::shared_ptr<SceneNode> ptrStar2Node = spComSys->spAllSystemObjectsNode->FindChild("Star2").lock();
						std::shared_ptr<SceneNode> ptrStar3Node = spComSys->spAllSystemObjectsNode->FindChild("Star3").lock();

						//Now set planet names
						SetPlanetName(ptrStar1Node, spStar1Com->starName, 97);
						SetPlanetName(ptrStar2Node, spStar2Com->starName, 97);
						SetPlanetName(ptrStar3Node, spStar3Com->starName, 97);
					}
					else
					{
						std::shared_ptr<SceneNode> ptrStar1Node = spComSys->spAllSystemObjectsNode->FindChild("Star1").lock();
						std::shared_ptr<SceneNode> ptrInsideSysNode = spComSys->spAllSystemObjectsNode->FindChild("InsideSystem").lock();

						//Now set planet names
						SetPlanetName(ptrStar1Node, spStar1Com->starName, 97);
						SetPlanetName(ptrInsideSysNode, spComSys->systemName, 100);
					}
				}

				//Create text name entity for system
				std::string name{ "SystemNameText" };
				CreateSystemText(wpSystemNamesNode.lock(), node.GetSharedPtrToItself(), name, true);

				//Create habitable planet icon
				VisitorCountHabitablePlanets visitor;
				spComSys->spAllSystemObjectsNode->AcceptVisitor(visitor);
				if (visitor.counter > 0)
				{
					std::shared_ptr<Entity> spIcEn = CreateIconForSystemOverview(node.GetSharedPtrToItself(), wpSystemNamesNode.lock(), std::to_string(visitor.counter) + "HabitablePlanetIcon", spComSys->systemName + "HabitableIcon", true, WorldGenerator::Instance().getSpaceMapConfig().habitablePlanetIconSize);
					spIcEn->FindComponent<UIFollowerComponent>().lock()->offset = WorldGenerator::Instance().getSpaceMapConfig().habitablePlanetIconOffset;
				}

				spComSys->spAllSystemObjectsNode->AcceptVisitor(*this);
			}
		}
		else if (spEntity->HasComponent<StarComponent>())
		{
			std::shared_ptr<StarComponent> spStar = spEntity->FindComponent<StarComponent>().lock();
			std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->AddComponent<RectangleShapeComponent>().lock();
			spEntity->hidden = true;

			SetStarTexture(spRectShape, spStar->starType, spStar->starSize);
		}
		else if (spEntity->HasComponent<PlanetComponent>())
		{
			std::shared_ptr<PlanetComponent> spPlanet = spEntity->FindComponent<PlanetComponent>().lock();
			spPlanet->planetIconTextureName = GetPlanetIconTextureName(spPlanet->planetType, spPlanet->planetSize, WorldGenerator::Instance().getSpaceMapConfig(), spEntity->FindComponent<HabitablePlanetComponent>().lock());
			spEntity->hidden = true;

			if (spPlanet->isMoon)
			{
				std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->AddComponent<RectangleShapeComponent>().lock();
				SetupRectangleShape(spRectShape, sf::Vector2f{ 1.f,1.f } * spPlanet->planetSize * WorldGenerator::Instance().getSpaceMapConfig().earthDiameter, GetPlanetTextureName(spPlanet->planetType, spEntity->FindComponent<HabitablePlanetComponent>().lock()));
			}
			else
			{
				std::vector<std::shared_ptr<SceneNode>> children = node.GetAllChildren();
				if (!children.empty())
				{
					SortedPlanetComponentsList sortedPlanets;
					for (std::shared_ptr<SceneNode> child : children)
					{
						if (child->GetEntity().lock()->HasComponent<PlanetComponent>())
							sortedPlanets.AddPlanetComponent(child->GetEntity().lock()->FindComponent<PlanetComponent>().lock());
					}

					int counter = 1;
					while (sortedPlanets.Size() > 0)
					{
						//sortedPlanets.DequeuePlanetComponent().lock()->planetName = "Nem";
						//sortedPlanets.DequeuePlanetComponent().lock()->planetName = name + ' ' + GetRomanNumber(counter);
						sortedPlanets.DequeuePlanetComponent().lock()->planetName = spPlanet->planetName + ' ' + GetRomanNumber(counter);
						counter++;
					}
				}
			}
		}
		else if (spEntity->HasComponent<RingComponent>())
		{
			std::shared_ptr<RingComponent> spRingCom = spEntity->FindComponent<RingComponent>().lock();

			std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->AddComponent<RectangleShapeComponent>().lock();
			SetupRectangleShape(spRectShape, sf::Vector2f{ 1.f,1.f } * spRingCom->ringSize * WorldGenerator::Instance().getSpaceMapConfig().earthDiameter, "Ring" + std::to_string(spRingCom->ringNumber));

			std::shared_ptr<PlanetComponent> spPlanetCom = node.GetParent().lock()->GetEntity().lock()->FindComponent<PlanetComponent>().lock();
			spRingCom->ringIconTextureName = GetRingIconTextureName(spPlanetCom->planetType, spPlanetCom->planetSize, WorldGenerator::Instance().getSpaceMapConfig());
		}
	}
}



//Sorted by distance to the star from smallest to largest
void SortedPlanetComponentsList::AddPlanetComponent(std::shared_ptr<PlanetComponent> spPlanCom)
{
	if (sortedListOfPlanetCom.size() == 0)
	{
		sortedListOfPlanetCom.push_back(spPlanCom);
		return;
	}

	for (int i = 0; i < sortedListOfPlanetCom.size(); i++)
	{
		if (spPlanCom->orbitRadius > sortedListOfPlanetCom[i]->orbitRadius)
			continue;
		else
		{
			sortedListOfPlanetCom.insert(sortedListOfPlanetCom.begin() + i, spPlanCom);
			return;
		}
	}

	sortedListOfPlanetCom.push_back(spPlanCom);
}



std::weak_ptr<PlanetComponent> SortedPlanetComponentsList::DequeuePlanetComponent()
{
	if (sortedListOfPlanetCom.size() > 0)
	{
		std::weak_ptr wpFirst = sortedListOfPlanetCom[0];
		sortedListOfPlanetCom.erase(sortedListOfPlanetCom.begin());
		return wpFirst;
	}
	else
		return {};
}


int SortedPlanetComponentsList::Size() { return (int)sortedListOfPlanetCom.size(); }