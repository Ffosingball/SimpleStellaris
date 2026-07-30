#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourceManager.h"
#include <memory>
#include <iostream>

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


const std::weak_ptr<sf::SoundBuffer> ResourceManager::LoadSoundBuffer(const std::string& filename)
{
	//Find soundBuf in the map by name
	auto it1 = soundBuffers.find(filename);
	std::weak_ptr<sf::SoundBuffer> soundBufW;

	if (it1 != soundBuffers.end())
	{
		//If found then return it
		soundBufW = it1->second;
		return soundBufW;
	}

	//Else load it
	std::shared_ptr<sf::SoundBuffer> soundBuf = std::make_shared<sf::SoundBuffer>();
	//If it failed to load soundBuf then return null
	if (!soundBuf->loadFromFile(filename))
		return soundBufW;

	//Else add it to the map and return it
	auto it2 = soundBuffers.emplace(filename, soundBuf);
	auto it3 = it2.first;
	soundBufW = it3->second;
	return soundBufW;
}


const std::weak_ptr<sf::Font> ResourceManager::LoadFont(const std::string& filename)
{
	//Find font in the map by name
	auto it1 = fonts.find(filename);
	std::weak_ptr<sf::Font> fontPtr;

	if (it1 != fonts.end())
	{
		//If found then return it
		fontPtr = it1->second;
		return fontPtr;
	}

	//Else load it
	std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
	//If it failed to load soundBuf then return null
	if (!font->openFromFile(filename))
		return fontPtr;

	//Else add it to the map and return it
	auto it2 = fonts.emplace(filename, font);
	auto it3 = it2.first;
	fontPtr = it3->second;
	return fontPtr;
}


const std::weak_ptr<sf::Music> ResourceManager::LoadMusic(const std::string& filename)
{
	//Find music in the map by name
	auto it1 = musics.find(filename);
	std::weak_ptr<sf::Music> musicPtr;

	if (it1 != musics.end())
	{
		//If found then return it
		musicPtr = it1->second;
		return musicPtr;
	}

	//Else open stream for it
	std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
	//If it failed to open stream then return null
	if (!music->openFromFile(filename))
		return musicPtr;

	//Else add it to the map and return it
	auto it2 = musics.emplace(filename, music);
	auto it3 = it2.first;
	musicPtr = it3->second;
	return musicPtr;
}