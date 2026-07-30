#pragma once
#include <SFML/Graphics.hpp>

namespace gel
{
	//This function sets text at the provided position
	void CentreText(sf::Text& text, const sf::Vector2f& position);
	void SetTextOrigin(sf::Text& text, const sf::Vector2f& position);

	float Randf(float zMin, float zMax);
	float Randf01();

	float linearInterpolation(float min, float max, float pos);

	float clamp(float value, float minValue, float maxValue);

	bool DetectCollision(const sf::Vector2f& entity1Pos, const sf::Vector2f& entity1Size, const sf::Vector2f& entity2Pos, const sf::Vector2f& entity2Size);
}

