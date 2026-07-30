#pragma once
#include <SFML/Graphics.hpp>

class ParticleConfig;

//General particle structure
struct Particle 
{
	const ParticleConfig* pConfig = nullptr;
	sf::RectangleShape shape;
	sf::Vector2f position;
	sf::Vector2f velocity;
	float timeToLive = 0.f;
};

//Skeleton for the particle configurations
class ParticleConfig 
{
public:
	//How many particles to emit per second
	float emissionRate{ 50.f };
	//How long to emit particles
	float emissionDuration = std::numeric_limits<float>::infinity();
	sf::Texture* pTexture;

	virtual ~ParticleConfig() = default;
	//Initialize particle in the init
	virtual void Init(Particle& p) const {}
	//Update particle in specific way
	virtual void Update(Particle& p, float dt) const {}
	//Create a new particle
	Particle Make() const;
};