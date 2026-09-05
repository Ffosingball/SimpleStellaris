#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "SceneNode.h"
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Components.h"
#include <iostream>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "SpaceObjectTypes.h"
#include "SpaceMapConfiguration.h"

std::string GetSystemTextureName(StarType starType);
std::string GetPlanetIconTextureName(PlanetType planetType, float planetSize, SpaceMapConfigurations& mapConfig, std::weak_ptr<HabitablePlanetComponent> wpHabitablePlanet);
std::string GetPlanetTextureName(PlanetType planetType, std::weak_ptr<HabitablePlanetComponent> wpHabitablePlanet);
std::string GetRingIconTextureName(PlanetType planetType, float planetSize, SpaceMapConfigurations& mapConfig);