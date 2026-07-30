#include <SFML/Graphics.hpp>
#include "ParticlesAndConfigs.h"
#include <memory>
#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::Start()
{
    //Start emission
	isRunning = true;
    isFinished = false;
	emittedTotal = 0;
	emissionTimeTotal = 0.0f;
}


void ParticleSystem::Restart() 
{
    //Clear list and start emission 
    particles.clear();
    Start();
}


void ParticleSystem::Update(float dt)
{
    // Update lifetime
    for (Particle& p : particles)
        p.timeToLive -= dt;

    // Remove dead particles
    particles.remove_if(IsParticleDead());

    //Check if it is stopped
    if (isRunning)
    {
        // Emission-specific logic / particle creation
        emissionTimeTotal += dt;
        //Calculate how many particles should be emitted
        uint64_t emittedTotalExpected = (uint64_t)(config->emissionRate * emissionTimeTotal);
        //If emitted less than emit more
        if (emittedTotalExpected > emittedTotal)
        {
            uint64_t emitCountNow = emittedTotalExpected - emittedTotal;
            for (int i = 0; i < emitCountNow; ++i)
                particles.push_back(config->Make());
            emittedTotal = emittedTotalExpected;
        }
        //If duration past then stop emission
        if (emissionTimeTotal >= config->emissionDuration)
        {
            Stop();
        }
    }

    //Check if emission time finished and all particles disappeared then system finished its job
    if (emissionTimeTotal >= config->emissionDuration && particles.size()==0)
    {
        isFinished = true;
    }

    // Update non-lifetime state
    for (auto& p : particles)
        config->Update(p,dt);
}


void ParticleSystem::Draw(sf::RenderWindow& window)
{
    Draw(window,sf::Transform());
}


void ParticleSystem::Draw(sf::RenderWindow& window, sf::Transform zTransform)
{
    //Get transform of the system, to draw particles in the correct position
    sf::Transform transform = transformable.getTransform()*zTransform;
    for (Particle& particle : particles)
    {
        sf::RenderStates states;
        states.transform = transform;
        window.draw(particle.shape, states);
    }
}