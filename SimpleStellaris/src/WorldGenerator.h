#pragma once
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

class WorldGenerator 
{
private:
	int seed;
	std::mt19937 randomizer;
	std::uniform_int_distribution nextTile;
	std::uniform_int_distribution nextRotation{ 0, 3 };

	WorldGenerator() = delete;                    // Prevent construction
	WorldGenerator(const WorldGenerator&) = delete;         // Prevent copying
	WorldGenerator& operator=(const WorldGenerator&) = delete;

public:

	void SetSeed(int seed);
	std::vector GenerateGridOfRandomNumbers(sf::Vector2i gridSize, sf::Vector2i minMaxValues);
};