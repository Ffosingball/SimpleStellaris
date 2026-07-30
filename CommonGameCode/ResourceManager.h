#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

//ResourceManager is a singleton pattern, so there is only one object of it can
//exist in the program
class ResourceManager 
{
public:
	const std::weak_ptr<sf::Texture> LoadTexture(const std::string& filename);

	const std::weak_ptr<sf::Font> LoadFont(const std::string& filename);

	const std::weak_ptr<sf::SoundBuffer> LoadSoundBuffer(const std::string& filename);

	const std::weak_ptr<sf::Music> LoadMusic(const std::string& filename);

	//Disallow copying the resourceManager
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//The only way to access resource manager
	static ResourceManager& Instance() 
	{
		//static keyword allows to create this object only at the first call of
		//this method, at all next calls it will jus exist in this method
		static ResourceManager resourceManager;
		return resourceManager;
	}

private:

	//This class can be constructed and destroyed only within this class
	ResourceManager() = default;
	~ResourceManager() = default;

	//Unordered maps which stores all resources
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

	std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;

	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;

	std::unordered_map<std::string, std::shared_ptr<sf::Music>> musics;
};