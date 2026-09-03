#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourceManager.h"
#include <memory>
#include <iostream>

//This only exist for the tileMap
//Worst case: O(1)
const std::weak_ptr<sf::Texture> ResourceManager::LoadTexture(const std::string& filename)
{
	//find() in unordered list returns an iterator
	//Iterator is used to traverse through the data structur
	auto it1 = textures.find(filename);

	std::weak_ptr<sf::Texture> wp;

	//If there is no item with this name then it will be equal to the
	//textures.end()
	if (it1 != textures.end()) 
	{
		//it contains key at first and value at second
		wp = it1->second;
		return wp;
	}
	
	//If there is no texture then load from the file
	std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
	if (!texture->loadFromFile(filename))
	{
		std::cout << "No texture found!\n";
		return wp;
	}

	//Put new item in the unordered_map, it will return iterator
	//if succeded or end() if failed
	auto it2 = textures.emplace(filename, texture);
	auto it3 = it2.first;
	wp = it3->second;
	return wp;
}

//Worst case: O(1)
const std::weak_ptr<sf::Texture> ResourceManager::GetTexture(const std::string& texturename, sf::IntRect& textIntRect)
{
	//Find texture in the map by name
	auto it1 = textures.find(texturename);
	std::weak_ptr<sf::Texture> textureW;

	if (it1 != textures.end())
	{
		//If found then return it
		textureW = it1->second;
		textIntRect = texturesIntRects[texturename];
		return textureW;
	}
	else
	{
		std::cout << "Failed to find Texture: " << texturename << '\n';
		textIntRect = texturesIntRects["Placeholder"];
		return textures["Placeholder"];
	}
}

//Worst case: O(1)
const std::weak_ptr<sf::SoundBuffer> ResourceManager::GetSoundBuffer(const std::string& soundname)
{
	//Find soundBuf in the map by name
	auto it1 = soundBuffers.find(soundname);
	std::weak_ptr<sf::SoundBuffer> soundBufW;

	if (it1 != soundBuffers.end())
	{
		//If found then return it
		soundBufW = it1->second;
		return soundBufW;
	}
	else 
		std::cout<<"Failed to find SoundBuffer: " << soundname<<'\n';
}

//Worst case: O(1)
const std::weak_ptr<sf::Font> ResourceManager::GetFont(const std::string& fontname)
{
	//Find font in the map by name
	auto it1 = fonts.find(fontname);
	std::weak_ptr<sf::Font> fontW;

	if (it1 != fonts.end())
	{
		//If found then return it
		fontW = it1->second;
		return fontW;
	}
	else
		std::cout << "Failed to find Font: " << fontname << '\n';
}

//Worst case: O(1)
const std::weak_ptr<sf::Music> ResourceManager::GetMusic(const std::string& musicname)
{
	//Find music in the map by name
	auto it1 = musics.find(musicname);
	std::weak_ptr<sf::Music> musicW;

	if (it1 != musics.end())
	{
		//If found then return it
		musicW = it1->second;
		return musicW;
	}
	else
		std::cout << "Failed to find Music: " << musicname << '\n';
}

const void ResourceManager::LoadAllResources() 
{
	if (!resourcesLoaded)
	{
		//LOAD TEXTURES
		{
			std::string textureName{ "Placeholder" };
			//Create texture pointer
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			//Load textures and check if it succeded or failed
			if (!texture->loadFromFile("media/textures/placeholder.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				//Add it to map and its size
				textures.emplace(textureName, texture);
				sf::IntRect textureSizes{ {0,0},{32,32} };
				texturesIntRects.emplace(textureName, textureSizes);
			}
		}
		{
			std::string textureName{ "MouseIcon" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/MouseIcon.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				textures.emplace(textureName, texture);
				sf::IntRect textureSizes{ {0,0},{32,32} };
				texturesIntRects.emplace(textureName, textureSizes);
			}
		}
		{
			std::string textureName{ "SelectionIcon" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/SelectionIcon.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				textures.emplace(textureName, texture);
				sf::IntRect textureSizes{ {0,0},{32,32} };
				texturesIntRects.emplace(textureName, textureSizes);
			}
		}
		{
			std::string textureName{ "systemPictures" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/starsPicture.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				textures.emplace("NeutronStarSystem", texture);
				sf::IntRect textureSizes{ {0,0},{17,17} };
				texturesIntRects.emplace("NeutronStarSystem", textureSizes);

				textures.emplace("WhiteDwarfSystem", texture);
				sf::IntRect textureSizes1{ {17,0},{17,17} };
				texturesIntRects.emplace("WhiteDwarfSystem", textureSizes1);

				textures.emplace("BrownDwarfSystem", texture);
				sf::IntRect textureSizes2{ {34,0},{17,17} };
				texturesIntRects.emplace("BrownDwarfSystem", textureSizes2);

				textures.emplace("MclassSystem", texture);
				sf::IntRect textureSizes3{ {0,17},{17,17} };
				texturesIntRects.emplace("MclassSystem", textureSizes3);

				textures.emplace("KclassSystem", texture);
				sf::IntRect textureSizes4{ {17,17},{17,17} };
				texturesIntRects.emplace("KclassSystem", textureSizes4);

				textures.emplace("GclassSystem", texture);
				sf::IntRect textureSizes5{ {34,17},{17,17} };
				texturesIntRects.emplace("GclassSystem", textureSizes5);

				textures.emplace("FclassSystem", texture);
				sf::IntRect textureSizes6{ {0,34},{17,17} };
				texturesIntRects.emplace("FclassSystem", textureSizes6);

				textures.emplace("AclassSystem", texture);
				sf::IntRect textureSizes7{ {17,34},{17,17} };
				texturesIntRects.emplace("AclassSystem", textureSizes7);

				textures.emplace("BclassSystem", texture);
				sf::IntRect textureSizes8{ {34,34},{17,17} };
				texturesIntRects.emplace("BclassSystem", textureSizes8);

				textures.emplace("OclassSystem", texture);
				sf::IntRect textureSizes9{ {0,51},{17,17} };
				texturesIntRects.emplace("OclassSystem", textureSizes9);

				textures.emplace("RedGiantSystem", texture);
				sf::IntRect textureSizes10{ {17,51},{17,17} };
				texturesIntRects.emplace("RedGiantSystem", textureSizes10);

				textures.emplace("RedSupergiantSystem", texture);
				sf::IntRect textureSizes11{ {34,51},{17,17} };
				texturesIntRects.emplace("RedSupergiantSystem", textureSizes11);
			}
		}
		{
			std::string textureName{ "starPictures" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/starsTextures.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				textures.emplace("BlackHole", texture);
				sf::IntRect textureSize{ {700,1050},{350,350} };
				texturesIntRects.emplace("BlackHole", textureSize);

				textures.emplace("NeutronStar", texture);
				textureSize.position = sf::Vector2i{ 350,1050 };
				texturesIntRects.emplace("NeutronStar", textureSize);

				textures.emplace("WhiteDwarf", texture);
				textureSize.position = sf::Vector2i{ 0,1050 };
				texturesIntRects.emplace("WhiteDwarf", textureSize);

				textures.emplace("BrownDwarf", texture);
				textureSize.position = sf::Vector2i{ 700,1050 };
				texturesIntRects.emplace("BrownDwarf", textureSize);

				textures.emplace("Mclass", texture);
				textureSize.position = sf::Vector2i{ 350,700 };
				texturesIntRects.emplace("Mclass", textureSize);

				textures.emplace("Kclass", texture);
				textureSize.position = sf::Vector2i{ 0,700 };
				texturesIntRects.emplace("Kclass", textureSize);

				textures.emplace("Gclass", texture);
				textureSize.position = sf::Vector2i{ 700,350 };
				texturesIntRects.emplace("Gclass", textureSize);

				textures.emplace("Fclass", texture);
				textureSize.position = sf::Vector2i{ 350,350 };;
				texturesIntRects.emplace("Fclass", textureSize);

				textures.emplace("Aclass", texture);
				textureSize.position = sf::Vector2i{ 0,350 };
				texturesIntRects.emplace("Aclass", textureSize);

				textures.emplace("Bclass", texture);
				textureSize.position = sf::Vector2i{ 700,0 };
				texturesIntRects.emplace("Bclass", textureSize);

				textures.emplace("Oclass", texture);
				textureSize.position = sf::Vector2i{ 350,0 };
				texturesIntRects.emplace("Oclass", textureSize);

				textures.emplace("RedGiant", texture);
				textureSize.position = sf::Vector2i{ 0,0 };
				texturesIntRects.emplace("RedGiant", textureSize);
			}
		}
		{
			std::string textureName{ "CenterOfMassIcon" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/centerOfMass.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				textures.emplace(textureName, texture);
				sf::IntRect textureSizes{ {0,0},{64,64} };
				texturesIntRects.emplace(textureName, textureSizes);
			}
		}
		{
			std::string textureName{ "PlanetIcons" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/planetsIcons.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "SmallMoltenPlanetIcon";
				sf::IntRect textureSize{ {0,0},{16,16} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumMoltenPlanetIcon";
				textureSize.position = sf::Vector2i{ 16,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeMoltenPlanetIcon";
				textureSize.position = sf::Vector2i{ 32,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallVenusLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 48,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumVenusLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 64,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeVenusLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 80,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallGreyBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 0,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumGreyBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 16,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeGreyBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 32,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallOceanicPlanetIcon";
				textureSize.position = sf::Vector2i{ 48,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumOceanicPlanetIcon";
				textureSize.position = sf::Vector2i{ 64,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeOceanicPlanetIcon";
				textureSize.position = sf::Vector2i{ 80,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 0,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 16,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 32,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallAfarEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 48,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumAfarEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 64,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeAfarEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 80,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallCloseEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 0,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumCloseEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 16,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeCloseEarthLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 32,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallRedBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 48,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumRedBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 64,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeRedBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 80,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallDesertPlanetIcon";
				textureSize.position = sf::Vector2i{ 0,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumDesertPlanetIcon";
				textureSize.position = sf::Vector2i{ 16,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeDesertPlanetIcon";
				textureSize.position = sf::Vector2i{ 32,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallIcyPlanetIcon";
				textureSize.position = sf::Vector2i{ 48,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumIcyPlanetIcon";
				textureSize.position = sf::Vector2i{ 64,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeIcyPlanetIcon";
				textureSize.position = sf::Vector2i{ 80,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SmallDarkBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 0,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumDarkBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 16,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "LargeDarkBarrenPlanetIcon";
				textureSize.position = sf::Vector2i{ 32,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "TitanLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 48,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "VoulcanicPlanetIcon";
				textureSize.position = sf::Vector2i{ 64,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "UranusLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 80,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "NeptuneLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 0,96 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "HotNeptunePlanetIcon";
				textureSize.position = sf::Vector2i{ 16,96 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "JupiterLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 32,96 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SaturnLikePlanetIcon";
				textureSize.position = sf::Vector2i{ 48,96 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "HotJupiterPlanetIcon";
				textureSize.position = sf::Vector2i{ 64,96 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}

		{
			std::string textureName{ "Nebulas" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/Nebulas.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "Nebula0";
				sf::IntRect textureSize{ {0,0},{100,100} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Nebula1";
				textureSize.position = sf::Vector2i{ 100,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Nebula2";
				textureSize.position = sf::Vector2i{ 200,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Nebula3";
				textureSize.position = sf::Vector2i{ 300,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}
		{
			std::string textureName{ "UI parts" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/UIParts.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "BottomUIPart";
				sf::IntRect textureSize{ {0,21},{100,19} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "TopUIPart";
				textureSize.position = sf::Vector2i{ 0,0 };
				textureSize.size = sf::Vector2i{ 100,11 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}
		{
			std::string textureName{ "PlanetTextures" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/PlanetTextures.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "NeptuneLike";
				sf::IntRect textureSize{ {0,0},{40,40} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "HotNeptune";
				textureSize.position = sf::Vector2i{ 40,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Icy";
				textureSize.position = sf::Vector2i{ 80,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "AfarEarthLike";
				textureSize.position = sf::Vector2i{ 120,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "RedBarren";
				textureSize.position = sf::Vector2i{ 160,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "UranusLike";
				textureSize.position = sf::Vector2i{ 0,40 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "HotJupiter";
				textureSize.position = sf::Vector2i{ 40,40 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Molten";
				textureSize.position = sf::Vector2i{ 80,40 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "EarthLike";
				textureSize.position = sf::Vector2i{ 120,40 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "GreyBarren";
				textureSize.position = sf::Vector2i{ 160,40 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SaturnLike";
				textureSize.position = sf::Vector2i{ 0,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Desert";
				textureSize.position = sf::Vector2i{ 40,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "TitanLike";
				textureSize.position = sf::Vector2i{ 80,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Oceanic";
				textureSize.position = sf::Vector2i{ 120,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "DarkBarren";
				textureSize.position = sf::Vector2i{ 160,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "JupiterLike";
				textureSize.position = sf::Vector2i{ 0,120 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Voulcanic";
				textureSize.position = sf::Vector2i{ 40,120 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "CloseEarthLike";
				textureSize.position = sf::Vector2i{ 80,120 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "VenusLike";
				textureSize.position = sf::Vector2i{ 120,120 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}
		{
			std::string textureName{ "RingsTexture" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/Rings.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "Ring0";
				sf::IntRect textureSize{ {0,0},{100,100} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring1";
				textureSize.position = sf::Vector2i{ 100,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring2";
				textureSize.position = sf::Vector2i{ 200,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring3";
				textureSize.position = sf::Vector2i{ 0,100 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring4";
				textureSize.position = sf::Vector2i{ 100,100 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring5";
				textureSize.position = sf::Vector2i{ 200,100 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring6";
				textureSize.position = sf::Vector2i{ 0,200 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring7";
				textureSize.position = sf::Vector2i{ 100,200 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Ring8";
				textureSize.position = sf::Vector2i{ 200,200 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}
		{
			std::string textureName{ "RingsIcons" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/ringsIcons.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "RingIconSmallRocky";
				sf::IntRect textureSize{ {0,0},{32,32} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "RingIconMediumRocky";
				textureSize.position = sf::Vector2i{ 32,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "RingIconLargeRocky";
				textureSize.position = sf::Vector2i{ 64,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "RingIconSmallGas";
				textureSize.position = sf::Vector2i{ 96,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "RingIconLargeGas";
				textureSize.position = sf::Vector2i{ 128,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}
		{
			std::string textureName{ "UIPartSide" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/UIPart2.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				textures.emplace(textureName, texture);
				sf::IntRect textureSizes{ {0,0},{100,100} };
				texturesIntRects.emplace(textureName, textureSizes);
			}
		}
		{
			std::string textureName{ "Districts Textures" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/Districts_textures.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "BarrenGreyBarrenDistrict";
				sf::IntRect textureSize{ {0,0},{16,16} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenDarkBarrenDistrict";
				textureSize.position = sf::Vector2i{ 16,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenRedBarrenDistrict";
				textureSize.position = sf::Vector2i{ 32,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "TitanLikeBarrenDistrict";
				textureSize.position = sf::Vector2i{ 48,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "IcyBarrenDistrict";
				textureSize.position = sf::Vector2i{ 64,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "DesertBarrenDistrict";
				textureSize.position = sf::Vector2i{ 80,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "VenusLikeBarrenDistrict";
				textureSize.position = sf::Vector2i{ 96,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "VoulcanicBarrenDistrict";
				textureSize.position = sf::Vector2i{ 0,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenGreyCratorDistrict";
				textureSize.position = sf::Vector2i{ 16,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenDarkCratorDistrict";
				textureSize.position = sf::Vector2i{ 32,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenRedCratorDistrict";
				textureSize.position = sf::Vector2i{ 48,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenGreyMountainsDistrict";
				textureSize.position = sf::Vector2i{ 64,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenDarkMountainsDistrict";
				textureSize.position = sf::Vector2i{ 80,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenRedMountainsDistrict";
				textureSize.position = sf::Vector2i{ 96,16 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "VenusLikeMountainsDistrict";
				textureSize.position = sf::Vector2i{ 0,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "EarthLikeMountainsDistrict";
				textureSize.position = sf::Vector2i{ 16,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "TitanLikeMountainsDistrict";
				textureSize.position = sf::Vector2i{ 32,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "IcyMountainsDistrict";
				textureSize.position = sf::Vector2i{ 48,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "DesertMountainsDistrict";
				textureSize.position = sf::Vector2i{ 64,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenGreyDesertDistrict";
				textureSize.position = sf::Vector2i{ 80,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenDarkDesertDistrict";
				textureSize.position = sf::Vector2i{ 96,32 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "BarrenRedDesertDistrict";
				textureSize.position = sf::Vector2i{ 0,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "DesertDistrict";
				textureSize.position = sf::Vector2i{ 16,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "OceanDistrict";
				textureSize.position = sf::Vector2i{ 32,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "IslandsDistrict";
				textureSize.position = sf::Vector2i{ 48,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MethanOceanDistrict";
				textureSize.position = sf::Vector2i{ 64,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "VenusLikeVoulcanoDistrict";
				textureSize.position = sf::Vector2i{ 80,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "EarthLikeVoulcanoDistrict";
				textureSize.position = sf::Vector2i{ 96,48 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "TitanLikeVoulcanoDistrict";
				textureSize.position = sf::Vector2i{ 0,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MoltenVoulcanoDistrict";
				textureSize.position = sf::Vector2i{ 16,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "IcyVoulcanoDistrict";
				textureSize.position = sf::Vector2i{ 32,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "VoulcanicVoulcanoDistrict";
				textureSize.position = sf::Vector2i{ 48,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "DesertVoulcanoDistrict";
				textureSize.position = sf::Vector2i{ 64,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "SteppeDistrict";
				textureSize.position = sf::Vector2i{ 80,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "IceSheetDistrict";
				textureSize.position = sf::Vector2i{ 96,64 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MoltenLandDistrict";
				textureSize.position = sf::Vector2i{ 0,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumRainforestDistrict";
				textureSize.position = sf::Vector2i{ 16,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MediumForestDistrict";
				textureSize.position = sf::Vector2i{ 32,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "FarRainforestDistrict";
				textureSize.position = sf::Vector2i{ 48,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "FarForestDistrict";
				textureSize.position = sf::Vector2i{ 64,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "CloseRainforestDistrict";
				textureSize.position = sf::Vector2i{ 80,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "CloseForestDistrict";
				textureSize.position = sf::Vector2i{ 96,80 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "EarthLikeFarSteppeDistrict";
				textureSize.position = sf::Vector2i{ 0,96 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "EarthLikeCloseSteppeDistrict";
				textureSize.position = sf::Vector2i{ 16,96 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}
		{
			std::string textureName{ "UIPartPlanetDistricts" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/UIPart3.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				textures.emplace(textureName, texture);
				sf::IntRect textureSizes{ {0,0},{200,140} };
				texturesIntRects.emplace(textureName, textureSizes);
			}
		}
		{
			std::string textureName{ "Buttons Textures" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/Buttons_texture.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "ResumeButton";
				sf::IntRect textureSize{ {0,0},{179,17} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "ResumeHoveredButton";
				textureSize.position = sf::Vector2i{ 0,17 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "ResumeHoveredPressedButton";
				textureSize.position = sf::Vector2i{ 0,34 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "ResumePressedButton";
				textureSize.position = sf::Vector2i{ 0,51 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "ExitGameButton";
				textureSize.position = sf::Vector2i{ 0,68 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "ExitGameHoveredButton";
				textureSize.position = sf::Vector2i{ 0,85 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "ExitGameHoveredPressedButton";
				textureSize.position = sf::Vector2i{ 0,102 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "ExitGamePressedButton";
				textureSize.position = sf::Vector2i{ 0,119 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}
		{
			std::string textureName{ "Players Texture" };
			std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile("media/textures/playerParts.png"))
				std::cout << "No texture found: " + textureName + "\n";
			else
			{
				std::string name = "Slower3Button";
				sf::IntRect textureSize{ {0,0},{23,10} };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower3HoveredButton";
				textureSize.position = sf::Vector2i{ 0,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower3PressedButton";
				textureSize.position = sf::Vector2i{ 0,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower2Button";
				textureSize.position = sf::Vector2i{ 23,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower2HoveredButton";
				textureSize.position = sf::Vector2i{ 23,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower2PressedButton";
				textureSize.position = sf::Vector2i{ 23,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower1Button";
				textureSize.position = sf::Vector2i{ 46,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower1HoveredButton";
				textureSize.position = sf::Vector2i{ 46,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Slower1PressedButton";
				textureSize.position = sf::Vector2i{ 46,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "StoppedButton";
				textureSize.position = sf::Vector2i{ 69,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "StoppedHoveredButton";
				textureSize.position = sf::Vector2i{ 69,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "StoppedPressedButton";
				textureSize.position = sf::Vector2i{ 69,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "PlayingButton";
				textureSize.position = sf::Vector2i{ 92,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "PlayingHoveredButton";
				textureSize.position = sf::Vector2i{ 92,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "PlayingPressedButton";
				textureSize.position = sf::Vector2i{ 92,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster1Button";
				textureSize.position = sf::Vector2i{ 115,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster1HoveredButton";
				textureSize.position = sf::Vector2i{ 115,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster1PressedButton";
				textureSize.position = sf::Vector2i{ 115,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster2Button";
				textureSize.position = sf::Vector2i{ 138,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster2HoveredButton";
				textureSize.position = sf::Vector2i{ 138,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster2PressedButton";
				textureSize.position = sf::Vector2i{ 138,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster3Button";
				textureSize.position = sf::Vector2i{ 161,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster3HoveredButton";
				textureSize.position = sf::Vector2i{ 161,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "Faster3PressedButton";
				textureSize.position = sf::Vector2i{ 161,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MixButton";
				textureSize.position = sf::Vector2i{ 184,0 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MixHoveredButton";
				textureSize.position = sf::Vector2i{ 184,10 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);

				name = "MixPressedButton";
				textureSize.position = sf::Vector2i{ 184,20 };
				textures.emplace(name, texture);
				texturesIntRects.emplace(name, textureSize);
			}
		}


		//LOAD FONTS
		{
			std::string fontName{ "Pixel" };
			std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
			if (!font->openFromFile("media/fonts/PixelOperator8.ttf"))
				std::cout << "No font found: " + fontName + "\n";
			else
				fonts.emplace(fontName, font);
		}
		{
			std::string fontName{ "PixelBold" };
			std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
			if (!font->openFromFile("media/fonts/PixelOperator8-Bold.ttf"))
				std::cout << "No font found: " + fontName + "\n";
			else
				fonts.emplace(fontName, font);
		}


		//LOAD MUSIC
		{
			std::string musicName{ "Ambient1" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/alexgrohl-horror.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient2" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/everything_is_dead-dark-mysterious-dreamscape.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient3" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/leberch-cinematic-space.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient4" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/playstarz_music-space-ambient.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient5" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/solarflex-space-1.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient6" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/tunetank-dark-space-ambient.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient7" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/geoffharvey-future-worlds-space-theme.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient8" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/juliush-universe-space-sounds.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient9" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/konstantinpazuzustudio-blind-fear-horror-piano-ambience.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient10" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/solarflex-space-ambient.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient11" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/solarflex-space-2.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Ambient12" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/Ambient/the_mountain-space-study.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "A Class Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Aclass_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Dark Barren Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/BarrenDark_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Grey Barren Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/BarrenGrey_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "B Class Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Bclass_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Black Hole Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/BlackHole_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Brown Dwarf Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/BrownDwarf_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Desert Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Desert_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Earth Like Close Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/EarthLikeClose_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Earth Like Far Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/EarthLikeFar_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Earth Like Medium Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/EarthLikeMedium_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "F Class Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Fclass_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "G Class Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Gclass_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Hot Gas Planet Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/HotJupiterAndNeptune_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Icy Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Icy_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Jupiter Like Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/JupiterLike_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "K Class Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Kclass_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "M Class Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Mclass_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Molten Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Molten_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Neptune Like Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/NeptuneLike_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Oceanic Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Oceanic_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "O Class Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Oclass_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Pulsar0 Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Pulsar1_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Pulsar1 Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Pulsar2_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Pulsar2 Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Pulsar3_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Pulsar3 Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Pulsar4_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Pulsar4 Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Pulsar5_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Red Barren Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/RedBarren_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Red Giant Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/RedGiant_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Red Supergiant Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/RedSupergiant_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Saturn Like Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/SaturnLike_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Titan Like Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/TitanLike_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Uranus Like Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/UranusLike_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Venus Like Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/VenusLike_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "Voulcanic Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/Voulcanic_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}
		{
			std::string musicName{ "White Dwarf Sound" };
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile("media/music/StarsAndPlanetsSounds/WhiteDwarf_sound.ogg"))
				std::cout << "No music found: " + musicName + "\n";
			else
				musics.emplace(musicName, music);
		}



		//LOAD SOUNDS
		{
			std::string sfxName{ "EnterSFX" };
			std::shared_ptr<sf::SoundBuffer> soundBuf = std::make_shared<sf::SoundBuffer>();
			if (!soundBuf->loadFromFile("media/sfx/Enter_sfx.mp3"))
				std::cout << "No sfx found: " + sfxName + "\n";
			else
				soundBuffers.emplace(sfxName, soundBuf);
		}
		{
			std::string sfxName{ "ExitSFX" };
			std::shared_ptr<sf::SoundBuffer> soundBuf = std::make_shared<sf::SoundBuffer>();
			if (!soundBuf->loadFromFile("media/sfx/Exit_sfx.mp3"))
				std::cout << "No sfx found: " + sfxName + "\n";
			else
				soundBuffers.emplace(sfxName, soundBuf);
		}
		{
			std::string sfxName{ "ResumeSFX" };
			std::shared_ptr<sf::SoundBuffer> soundBuf = std::make_shared<sf::SoundBuffer>();
			if (!soundBuf->loadFromFile("media/sfx/Resume_sfx.mp3"))
				std::cout << "No sfx found: " + sfxName + "\n";
			else
				soundBuffers.emplace(sfxName, soundBuf);
		}
		{
			std::string sfxName{ "PauseSFX" };
			std::shared_ptr<sf::SoundBuffer> soundBuf = std::make_shared<sf::SoundBuffer>();
			if (!soundBuf->loadFromFile("media/sfx/Pause_sfx.mp3"))
				std::cout << "No sfx found: " + sfxName + "\n";
			else
				soundBuffers.emplace(sfxName, soundBuf);
		}
		{
			std::string sfxName{ "LockCameraSFX" };
			std::shared_ptr<sf::SoundBuffer> soundBuf = std::make_shared<sf::SoundBuffer>();
			if (!soundBuf->loadFromFile("media/sfx/LockCamera_sfx.mp3"))
				std::cout << "No sfx found: " + sfxName + "\n";
			else
				soundBuffers.emplace(sfxName, soundBuf);
		}
		{
			std::string sfxName{ "UnlockCameraSFX" };
			std::shared_ptr<sf::SoundBuffer> soundBuf = std::make_shared<sf::SoundBuffer>();
			if (!soundBuf->loadFromFile("media/sfx/UnlockCamera_sfx.mp3"))
				std::cout << "No sfx found: " + sfxName + "\n";
			else
				soundBuffers.emplace(sfxName, soundBuf);
		}

		resourcesLoaded = true;
	}
}