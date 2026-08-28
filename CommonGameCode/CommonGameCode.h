#pragma once
#include <SFML/Graphics.hpp>

namespace gel
{
	constexpr double PI = 3.14159265358979323846;

	//This function sets text at the provided position
	//Worst case: O(1)
	void CentreText(sf::Text& text, const sf::Vector2f& position);
	//Worst case: O(1)
	void SetTextOrigin(sf::Text& text, const sf::Vector2f& position);
	//Get random number in specific range
	//Worst case: O(1)
	float Randf(float zMin, float zMax);
	//Get random number from 0 to 1
	//Worst case: O(1)
	float Randf01();
	//right number is not included
	int RandInt(int zMin, int zMax);
	//Returns postion between two points using percentage of how far from the left point
	//Worst case: O(1)
	float linearInterpolation(float min, float max, float pos);
	//Calculates distance between two points
	//Worst case: O(1)
	float distanceBetween2Points(sf::Vector2f pos1, sf::Vector2f pos2);
	//Return value which will be between this two values
	//Worst case: O(1)
	float clamp(float value, float minValue, float maxValue);
	//This function assumes that it receives position of top left corner
	//Worst case: O(1)
	bool DetectCollision(const sf::Vector2f& entity1Pos, const sf::Vector2f& entity1Size, const sf::Vector2f& entity2Pos, const sf::Vector2f& entity2Size);
	//Calculates sphere volume
	//Worst case: O(1)
	float sphereVolume(float radius);
	//Calculates sphere volume
	//Worst case: O(1)
	double sphereVolume(double radius);
	//Aligns text at left side
	//Worst case: O(1)
	void AlignTextToLeftSide(sf::Text& text, const sf::Vector2f& position);
	//Formats float number to string
	//Worst case: O(N) where N is number of digits in the number
	std::string roundNumberForOutput(float n, int digitsAfterSeparator);
	//Formats double number to string
	//Worst case: O(N) where N is number of digits in the number
	std::string roundNumberForOutput(double n, int digitsAfterSeparator);
	//Check if point within the provided area or not
	//Worst case: O(1)
	bool IsPointInTheArea(sf::Vector2f point, sf::Vector2f areaPosition, sf::Vector2f areaSize);
}

