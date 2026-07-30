#include <iostream>
#include <SFML/Graphics.hpp>
#include <rapidcsv.h>
#include "TileMap.h"
#include "ResourceManager.h"
#include <memory>

//Read data from the file
std::vector<int> ReadTileMapFromCSV(const std::string fname, sf::Vector2i& tilemapSize)
{
	std::vector<int> data;
	rapidcsv::LabelParams labelParams(-1, -1); // this says that row and column
	//data will start from index 0 --no headers expected, either horizontally or
	//	vertically
	rapidcsv::Document doc(fname, labelParams);
	const size_t numRows = doc.GetRowCount();
	size_t numColumns = 0;
	for (size_t i = 0; i < numRows; ++i)
	{
		std::vector<int> rowData = doc.GetRow<int>(i);
		if (i == 0)
			numColumns = rowData.size();
		for (size_t j = 0; j < numColumns; ++j)
			data.push_back(rowData[j]);
		// Another way, using STL: append the entire rowData at the end of "data"
		// data.insert(data.end(), rowData.begin(), rowData.end());
	}
	tilemapSize = sf::Vector2i(numColumns, numRows);
	return data;
}

sf::Vector2i TileMap::getMapSize() const
{
	return mapSize;
}

//Create a tilemap
void TileMap::Initialize()
{
	//Set Triangle type
	tiles.setPrimitiveType(sf::PrimitiveType::Triangles);
	renderStates.coordinateType = sf::CoordinateType::Pixels;
	//Set texture
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().LoadTexture("media/textures/kenney_shmup_tiles.png");
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	renderStates.texture = sTexture.get();

	// Read the tilemap
	sf::Vector2i tilemapSize(0, 0);
	std::vector<int> tilemap = ReadTileMapFromCSV(tileSetPath, tilemapSize);
	// [ margin ][ tile0 ][ padding ][ tile 1 ][ padding ][ tile 2 ]
	//[padding] ...[tile N - 1][margin]
	//int numPixelsX = marginSize.x * 2 + (numTilesInTileset.x - 1) * paddingSize.x + numTilesInTileset.x * tileSize.x;
	//int numPixelsY = marginSize.y * 2 + (numTilesInTileset.y - 1) * paddingSize.y + numTilesInTileset.y * tileSize.y;
	//std::cout << "Num pixels estimated:" << numPixelsX << ", " << numPixelsY << "--num pixels in texture : " << tileset.getSize().x << ", " << tileset.getSize().y;
	//std::cout << "Initialize TileMap: "<<tilemapSize.x<<"; "<<tilemapSize.y<<'\n';
	//Current width and height in tilemap
	int curWidth = 0;
	int curHeight = -1;
	for (int tileNum : tilemap)
	{
		//Check if we reached the end of the tilemap width
		if (curWidth % tilemapSize.x == 0)
		{
			curWidth = 0;
			curHeight++;
		}

		//Initialize vertices
		sf::Vertex v1;
		sf::Vertex v2;
		sf::Vertex v3;
		sf::Vertex v4;
		sf::Vertex v5;
		sf::Vertex v6;

		//Set their coordinates in the world
		v1.position = sf::Vector2f{ static_cast<float>(tileSize.x * curWidth)-shiftBy, static_cast<float>(tileSize.y * curHeight)- shiftBy };
		v2.position = sf::Vector2f{ static_cast<float>(tileSize.x * (curWidth + 1))+shiftBy, static_cast<float>(tileSize.y * curHeight)- shiftBy };
		v3.position = sf::Vector2f{ static_cast<float>(tileSize.x * curWidth)- shiftBy, static_cast<float>(tileSize.y * (curHeight + 1))+ shiftBy };
		v4.position = sf::Vector2f{ static_cast<float>(tileSize.x * (curWidth + 1))+ shiftBy, static_cast<float>(tileSize.y * curHeight)- shiftBy };
		v5.position = sf::Vector2f{ static_cast<float>(tileSize.x * curWidth)- shiftBy, static_cast<float>(tileSize.y * (curHeight + 1))+ shiftBy };
		v6.position = sf::Vector2f{ static_cast<float>(tileSize.x * (curWidth + 1))+ shiftBy, static_cast<float>(tileSize.y * (curHeight + 1))+ shiftBy };

		//Calculate texture coordinates
		int height = tileNum / numTilesInTileset.x;
		int width = tileNum - (height * numTilesInTileset.x);
		int rectWidth = (tileSize.x * width) + (paddingSize.x * width) + marginSize.x;
		int rectHeight = (tileSize.y * height) + (paddingSize.y * height) + marginSize.y;

		//Set their coordinates in the texture
		v1.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
		v2.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
		v3.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
		v4.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
		v5.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
		v6.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };

		//Add them to the VertexArray
		tiles.append(v1);
		tiles.append(v2);
		tiles.append(v3);
		tiles.append(v4);
		tiles.append(v5);
		tiles.append(v6);

		curWidth++;
	}
	//Set map size
	mapSize = sf::Vector2i{ tilemapSize.x * tileSize.x,tilemapSize.y * tileSize.y };
}

//Render tilemap
void TileMap::Render(sf::RenderWindow& window, sf::Transform transform)
{
	//std::cout << "Render TileMap\n";
	renderStates.transform = transform;
	window.draw(tiles, renderStates);
}