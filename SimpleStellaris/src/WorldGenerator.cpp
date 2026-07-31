#include "WorldGenerator.h"


unsigned int WorldGenerator::seed = 0;
std::shared_ptr<std::mt19937> WorldGenerator::randomizer = nullptr;
//How much rarer tiles with nebula will be generated to usual tiles
float WorldGenerator::nebulaRareness = 2.f;
int WorldGenerator::numberOfNebulas = 6;


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