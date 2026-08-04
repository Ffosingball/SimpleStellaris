#pragma once

#include <SFML/Graphics.hpp>
#include "SFMLApplication.h"

class TileMap
{
private:
	//TileMap properties
	sf::VertexArray tiles;
	sf::RenderStates renderStates;
	//This field is important, I add it to sides of the tilemap, so 
	//size of the tile is not tileSize but tileSize+2*shiftBy, however distance
	//between tiles is still tileSize, so tiles overlap each other by 0.003, which
	//ensures that those wierd black lines will not appear between them for a single frame
	//when camera is moving
	float shiftBy{ 0.003f };

public:
	//TileSet properties
	sf::Vector2i tileSize;
	sf::Vector2i paddingSize;
	sf::Vector2i marginSize;
	sf::Vector2i numTilesInTileset;
	std::string tileSetPath;
	std::string tileSetRotationsPath;
	std::string tilesTexturePath;
	sf::Vector2i mapSize;
	bool rotateTiles{ false };
	bool loadTilesFromFile{ true };

	//Worst case: O((2*N*M)+K) where N is number of rows and M number of columns in provided file
	//and K number of tiles to create
	void Initialize(std::vector<int> tilemap, std::vector<int> rotationMap);

	void Update(const float deltaTime) {}
	//Worst case: O(N) where N is number of tiles
	void Render(sf::RenderWindow& window, sf::Transform transform);

	sf::Vector2i getMapSize() const;
};