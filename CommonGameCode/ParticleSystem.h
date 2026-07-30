#pragma once
#include <SFML/Graphics.hpp>
#include "ParticlesAndConfigs.h"
#include <memory>
#include <iostream>

class ParticleSystem 
{
public:
	//Position where particles will be emitted
	sf::Transformable transformable;
	//List of all particle
	std::list<Particle> particles;
	//Pointer to configuration
	std::shared_ptr<ParticleConfig> config;

	//Getter
	bool IsRunning() const { return isRunning; }
	//Getter
	bool IsFinished() const { return isFinished; }
	//I can stop, continue, start and restart particle emission
	void Stop() { isRunning = false; }
	void Continue() { if(!isFinished) isRunning = true; }
	void Start();
	void Restart();

	//Update all particles
	void Update(float dt);
	//Draw all particles
	void Draw(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window,sf::Transform transform);

private:
	//How long it emits particles
	float emissionTimeTotal{ 0.0f };
	int emittedTotal{ 0 };
	//Is it stopped or not
	bool isRunning{ false };
	bool isFinished{ false };
};

//This is a FUNCTOR
//It overloads parentheses(), so it can be used as a function
//It can be used instead of the anonymous or lambda functions
//It is usefull because it can store some parameters
struct IsParticleDead
{
	IsParticleDead() = default;

	bool operator() (const Particle& p) const
	{
		return p.timeToLive <= 0.0f;
	}
};