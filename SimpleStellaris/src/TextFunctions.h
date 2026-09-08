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
	//Debug textes
	void UpdateDaysPastDebugText(std::shared_ptr<sf::Text> spText);
	void UpdateDateDebugText(std::shared_ptr<sf::Text> spText);
	void UpdateNodesDebugText(std::shared_ptr<sf::Text> spText);
	void UpdateMousePositionDebugText(std::shared_ptr<sf::Text> spText);
	void UpdateWorldPositionDebugText(std::shared_ptr<sf::Text> spText);
	void UpdateMouseOverUIDebugText(std::shared_ptr<sf::Text> spText);
	void UpdateFPSDebugText(std::shared_ptr<sf::Text> spText);

	//Info panel
	void UpdateInfoText0(std::shared_ptr<sf::Text> spText);
	void UpdateInfoText1(std::shared_ptr<sf::Text> spText);
	void UpdateInfoText2(std::shared_ptr<sf::Text> spText);
	void UpdateInfoText3(std::shared_ptr<sf::Text> spText);
	void UpdateInfoText4(std::shared_ptr<sf::Text> spText);
	void UpdateInfoText5(std::shared_ptr<sf::Text> spText);
	void UpdateInfoText6(std::shared_ptr<sf::Text> spText);

	//Lower and upper part
	void UpdateDayText(std::shared_ptr<sf::Text> spText);
	void UpdateMonthText(std::shared_ptr<sf::Text> spText);
	void UpdateYearText(std::shared_ptr<sf::Text> spText);
	void UpdateSimulationStateText(std::shared_ptr<sf::Text> spText);
	void UpdateSimulationSpeedText(std::shared_ptr<sf::Text> spText);
	void UpdateViewSizeText(std::shared_ptr<sf::Text> spText);
	void UpdateOverviewText(std::shared_ptr<sf::Text> spText);

	//District panel
	void UpdatePlanetNameDistrictText(std::shared_ptr<sf::Text> spText);
	void UpdateDistrictTypeText(std::shared_ptr<sf::Text> spText);
}