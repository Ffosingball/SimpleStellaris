#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

//ResourceManager is a singleton pattern, so there is only one object of it can
//exist in the program
class ResourceManager 
{
public:
	//Call it only once
	const void LoadAllResources();

	//Left it for the tileMap
	//Worst case: O(1)
	const std::weak_ptr<sf::Texture> LoadTexture(const std::string& filename);
	//Worst case: O(1)
	const std::weak_ptr<sf::Texture> GetTexture(const std::string& texturename, sf::IntRect& textureIntRect);
	//Worst case: O(1)
	const std::weak_ptr<sf::Font> GetFont(const std::string& fontname);
	//Worst case: O(1)
	const std::weak_ptr<sf::SoundBuffer> GetSoundBuffer(const std::string& soundname);
	//Worst case: O(1)
	const std::weak_ptr<sf::Music> GetMusic(const std::string& musicname);

	//Disallow copying the resourceManager
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	//The only way to access resource manager
	//Worst case: O(1)
	static ResourceManager& Instance() 
	{
		//static keyword allows to create this object only at the first call of
		//this method, at all next calls it will just exist in this method
		static ResourceManager resourceManager;
		return resourceManager;
	}

private:

	//This class can be constructed and destroyed only within this class
	ResourceManager() = default;
	~ResourceManager() = default;

	bool resourcesLoaded{false};

	//Unordered maps which stores all resources
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
	std::unordered_map < std::string, sf::IntRect> texturesIntRects;
	std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;
	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
	std::unordered_map<std::string, std::shared_ptr<sf::Music>> musics;
};