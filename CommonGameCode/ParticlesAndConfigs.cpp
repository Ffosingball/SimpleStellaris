#include <SFML/Graphics.hpp>
#include "ParticlesAndConfigs.h"

//Worst case: O(1)
Particle ParticleConfig::Make() const
{
	Particle p;
	p.pConfig = this;
	Init(p);
	return p;
}