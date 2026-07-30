#include <SFML/Graphics.hpp>
#include "ParticlesAndConfigs.h"

Particle ParticleConfig::Make() const
{
	Particle p;
	p.pConfig = this;
	Init(p);
	return p;
}