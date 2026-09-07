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
#include "TextFunctions.h"
#include "Components.h"
#include "ECSGame.h"
#include "EntitiesFunctions.h"

namespace TextUpdateFunctions
{
	void UpdateDaysPastDebugText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		spText->setString("Simulation speed: " + std::to_string(spSpaceSceneState->simulationSpeed) + "; Days past: " + std::to_string((int)spSpaceSceneState->daysPast));
	}

	void UpdateDateDebugText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		int days = 0;
		std::string month;
		int years = 0;
		GetDateFromDays((int)spSpaceSceneState->daysPast, days, month, years);
		spText->setString("Proper Date: " + std::to_string(days) + " " + month + std::to_string(years));
	}
}