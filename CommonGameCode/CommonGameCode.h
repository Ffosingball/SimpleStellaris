#pragma once
#include <SFML/Graphics.hpp>

namespace gel
{
	//This function sets text at the provided position
	//Worst case: O(1)
	void CentreText(sf::Text& text, const sf::Vector2f& position);
	//Worst case: O(1)
	void SetTextOrigin(sf::Text& text, const sf::Vector2f& position);
	//Worst case: O(1)
	float Randf(float zMin, float zMax);
	//Worst case: O(1)
	float Randf01();
	//Worst case: O(1)
	float linearInterpolation(float min, float max, float pos);
	//Worst case: O(1)
	float distanceBetween2Points(sf::Vector2f pos1, sf::Vector2f pos2);
	//Worst case: O(1)
	float clamp(float value, float minValue, float maxValue);
	//Worst case: O(1)
	bool DetectCollision(const sf::Vector2f& entity1Pos, const sf::Vector2f& entity1Size, const sf::Vector2f& entity2Pos, const sf::Vector2f& entity2Size);
}

