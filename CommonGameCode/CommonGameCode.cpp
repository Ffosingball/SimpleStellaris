#include "CommonGameCode.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <cmath>

namespace gel
{
	//Worst case: O(1)
	void CentreText(sf::Text& text, const sf::Vector2f& position)
	{
		//Calculate correct origin
		sf::Vector2f centre = text.getGlobalBounds().size / 2.f;
		sf::Vector2f localBounds = centre + text.getLocalBounds().position;
		//Then set origin and position
		text.setOrigin(localBounds);
		text.setPosition(position);
	}

	//Worst case: O(1)
	void SetTextOrigin(sf::Text& text, const sf::Vector2f& position)
	{
		//Calculate correct origin
		sf::Vector2f centre = text.getGlobalBounds().size / 2.f;
		sf::Vector2f localBounds = centre + text.getLocalBounds().position;
		//Then set origin and position
		text.setOrigin(localBounds);
	}

	//Returns postion between two points using percentage of how far from the left point
	//Worst case: O(1)
	float linearInterpolation(float min, float max, float range0to1)
	{
		return min + ((max - min) * range0to1);
	}

	//Get random number from 0 to 1
	//Worst case: O(1)
	float Randf01()
	{
		return rand() / float(RAND_MAX);
	}

	//Get random number in specific range
	//Worst case: O(1)
	float Randf(float zMin, float zMax)
	{
		return linearInterpolation(zMin, zMax, Randf01());
	}

	//Return value which will be between this two values
	//Worst case: O(1)
	float clamp(float value, float minValue, float maxValue) 
	{
		return std::min(std::max(value, minValue), maxValue);
	}

	//This function assumes that it receives position of top left corner
	//Worst case: O(1)
	bool DetectCollision(const sf::Vector2f& entity1Pos, const sf::Vector2f& entity1Size, const sf::Vector2f& entity2Pos, const sf::Vector2f& entity2Size)
	{
		//Calculate distance 
		sf::Vector2f distance;
		distance.x = abs(entity1Pos.x - entity2Pos.x);
		distance.y = abs(entity1Pos.y - entity2Pos.y);

		//Check if we are way too far away
		if (distance.x > (entity2Size.x / 2.f + entity1Size.x/2.f))
			return false;
		if (distance.y > (entity2Size.y / 2.f + entity1Size.y/2.f))
			return false;

		return true;
	}

	//Worst case: O(1)
	float distanceBetween2Points(sf::Vector2f pos1, sf::Vector2f pos2) 
	{
		return std::sqrt(std::pow(std::abs(pos1.x-pos2.x), 2)+ std::pow(std::abs(pos1.y - pos2.y), 2));
	}
}