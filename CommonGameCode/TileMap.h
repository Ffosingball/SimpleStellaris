#pragma once

#include <SFML/Graphics.hpp>
#include "SFMLApplication.h"

class TileMap
{
private:
	//TileMap properties
	sf::VertexArray tiles;
	sf::Vector2i mapSize;
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

	void Initialize();

	void Update(const float deltaTime) {}

	void Render(sf::RenderWindow& window, sf::Transform transform);

	sf::Vector2i getMapSize() const;
};