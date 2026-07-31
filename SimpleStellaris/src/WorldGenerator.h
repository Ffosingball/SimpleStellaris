#pragma once
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>

class WorldGenerator 
{
private:
	static unsigned int seed;
	static std::shared_ptr<std::mt19937> randomizer;
	static float nebulaRareness;
	static int numberOfNebulas;

	WorldGenerator() = delete;                    // Prevent construction
	WorldGenerator(const WorldGenerator&) = delete;         // Prevent copying
	WorldGenerator& operator=(const WorldGenerator&) = delete;

public:

	static int getSeed() { return seed; }
	static void Initialize(unsigned int seed);
	//Min and max value are used so vector 0,3 all values 0,1,2,3 will be generated
	static std::vector<int> GenerateGridOfRandomNumbers(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
	static std::vector<int> GenerateGridOfTiles(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
};