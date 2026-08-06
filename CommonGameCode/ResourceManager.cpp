#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourceManager.h"
#include <memory>
#include <iostream>

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
	//Find soundBuf in the map by name
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
	//LOAD TEXTURES
	{
		std::string textureName{ "Placeholder" };
		//Create texture pointer
		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
		//Load textures and check if it succeded or failed
		if (!texture->loadFromFile("media/textures/placeholder.png"))
			std::cout << "No texture found: "+textureName+"\n";
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
			sf::IntRect textureSizes11{ {600,900},{300,300} };
			texturesIntRects.emplace("BlackHole", textureSizes11);

			textures.emplace("NeutronStar", texture);
			sf::IntRect textureSizes{ {300,900},{300,300} };
			texturesIntRects.emplace("NeutronStar", textureSizes);

			textures.emplace("WhiteDwarf", texture);
			sf::IntRect textureSizes1{ {0,900},{300,300} };
			texturesIntRects.emplace("WhiteDwarf", textureSizes1);

			textures.emplace("BrownDwarf", texture);
			sf::IntRect textureSizes2{ {600,600},{300,300} };
			texturesIntRects.emplace("BrownDwarf", textureSizes2);

			textures.emplace("Mclass", texture);
			sf::IntRect textureSizes3{ {300,600},{300,300} };
			texturesIntRects.emplace("Mclass", textureSizes3);

			textures.emplace("Kclass", texture);
			sf::IntRect textureSizes4{ {0,600},{300,300} };
			texturesIntRects.emplace("Kclass", textureSizes4);

			textures.emplace("Gclass", texture);
			sf::IntRect textureSizes5{ {600,300},{300,300} };
			texturesIntRects.emplace("Gclass", textureSizes5);

			textures.emplace("Fclass", texture);
			sf::IntRect textureSizes6{ {300,300},{300,300} };
			texturesIntRects.emplace("Fclass", textureSizes6);

			textures.emplace("Aclass", texture);
			sf::IntRect textureSizes7{ {0,300},{300,300} };
			texturesIntRects.emplace("Aclass", textureSizes7);

			textures.emplace("Bclass", texture);
			sf::IntRect textureSizes8{ {600,0},{300,300} };
			texturesIntRects.emplace("Bclass", textureSizes8);

			textures.emplace("Oclass", texture);
			sf::IntRect textureSizes9{ {300,0},{300,300} };
			texturesIntRects.emplace("Oclass", textureSizes9);

			textures.emplace("RedGiant", texture);
			sf::IntRect textureSizes10{ {0,0},{300,300} };
			texturesIntRects.emplace("RedGiant", textureSizes10);
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
	


	//LOAD SOUNDS
}