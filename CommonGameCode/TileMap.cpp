#include <iostream>
#include <SFML/Graphics.hpp>
#include <rapidcsv.h>
#include "TileMap.h"
#include "ResourceManager.h"
#include <memory>
#include <random> //For random generation

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
void TileMap::Initialize(std::vector<int> tilemap = {}, std::vector<int> rotationMap = {})
{
	//std::cout << "Init tileMap\n";
	//Set Triangle type
	tiles.setPrimitiveType(sf::PrimitiveType::Triangles);
	renderStates.coordinateType = sf::CoordinateType::Pixels;
	//Set texture
	std::weak_ptr<sf::Texture> wTexture = ResourceManager::Instance().LoadTexture(tilesTexturePath);
	std::shared_ptr<sf::Texture> sTexture = wTexture.lock();
	renderStates.texture = sTexture.get();

	sf::Vector2i tilemapSize(0, 0);
	//std::vector<int> tilemap;

	if (loadTilesFromFile)
	{
		// Read the tilemap
		tilemap = ReadTileMapFromCSV(tileSetPath, tilemapSize);
		if (rotateTiles) 
			rotationMap = ReadTileMapFromCSV(tileSetRotationsPath, tilemapSize);
	}
	else
		tilemapSize = mapSize;

	// [ margin ][ tile0 ][ padding ][ tile 1 ][ padding ][ tile 2 ]
	//[padding] ...[tile N - 1][margin]
	//int numPixelsX = marginSize.x * 2 + (numTilesInTileset.x - 1) * paddingSize.x + numTilesInTileset.x * tileSize.x;
	//int numPixelsY = marginSize.y * 2 + (numTilesInTileset.y - 1) * paddingSize.y + numTilesInTileset.y * tileSize.y;
	//std::cout << "Num pixels estimated:" << numPixelsX << ", " << numPixelsY << "--num pixels in texture : " << tileset.getSize().x << ", " << tileset.getSize().y;
	//std::cout << "Initialize TileMap: "<<tilemapSize.x<<"; "<<tilemapSize.y<<'\n';
	//Current width and height in tilemap
	int curWidth = 0;
	int curHeight = -1;
	for (int i{0}; i<tilemapSize.x*tilemapSize.y; i++)
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
		int height{ 0 };
		int width{ 0 };

		//std::cout <<"tileNum: " << tilemap[i] <<'\n';
		height = tilemap[i] / numTilesInTileset.x;
		width = tilemap[i] - (height * numTilesInTileset.x);
		int rectWidth = (tileSize.x * width) + (paddingSize.x * width) + marginSize.x;
		int rectHeight = (tileSize.y * height) + (paddingSize.y * height) + marginSize.y;

		//Set their coordinates in the texture
		if (!rotateTiles) 
		{
			//Do not rotate tiles
			v1.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
			v2.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
			v3.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
			v4.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
			v5.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
			v6.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };
		}
		else 
		{
			//rotate randomly tiles
			switch (rotationMap[i])
			{
				//Left-Top: static_cast<float>(rectWidth), static_cast<float>(rectHeight)
				//Left-Bottom: static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y)
				//Right-Top: static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight)
				//Right-Bottom: static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y)

				//Do not rotate
			case 0:
				v1.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
				v2.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
				v3.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
				v4.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
				v5.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
				v6.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };
				break;
			case 1://Rotate 90 deg anticlockwise
				v1.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
				v2.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
				v3.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };
				v4.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
				v5.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };
				v6.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
				break;
			case 2://Rotate 180 deg
				v1.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };
				v2.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
				v3.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
				v4.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
				v5.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
				v6.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
				break;
			case 3://Rotate 90 deg clockwise
				v1.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight) };
				v2.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };
				v3.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
				v4.texCoords = sf::Vector2f{ static_cast<float>(rectWidth + tileSize.x), static_cast<float>(rectHeight + tileSize.y) };
				v5.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight) };
				v6.texCoords = sf::Vector2f{ static_cast<float>(rectWidth), static_cast<float>(rectHeight + tileSize.y) };
				break;
			}
		}

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