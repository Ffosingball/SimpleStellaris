#pragma once

#include <memory>
#include "Component.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "TileMap.h"
//#include "ParticleSystem.h"
#include "SpaceObjectTypes.h"
#include <unordered_map>
#include "SpaceMapConfiguration.h"
#include "SceneNode.h"
#include "GameState.h"
#include <functional>
#include <sigslot/signal.hpp>

namespace TextUpdateFunctions
{
	void UpdateDaysPastDebugText(std::shared_ptr<sf::Text> spText);
	void UpdateDateDebugText(std::shared_ptr<sf::Text> spText);
}