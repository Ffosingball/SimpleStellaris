#include <memory>
#include "Component.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "CommonGameCode.h"
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
#include "SceneNodeVisitors.h"
#include "WorldGenerator.h"

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
		spText->setString("Proper Date: " + std::to_string(spSpaceSceneState->day) + " " + spSpaceSceneState->month + std::to_string(spSpaceSceneState->year));
	}

	void UpdateNodesDebugText(std::shared_ptr<sf::Text> spText)
	{
		spText->setString("Total nodes: " + std::to_string(ECSGame::Instance().GetNodesInTheScene()) + "; rendered: " + std::to_string(ECSGame::Instance().GetNodesRendered()));
	}

	void UpdateMousePositionDebugText(std::shared_ptr<sf::Text> spText) 
	{
		sf::Vector2i mousePosition = ECSGame::Instance().GetMousePosition();
		spText->setString("Window pos: " + std::to_string(mousePosition.x) + "; " + std::to_string(mousePosition.y));
	}

	void UpdateWorldPositionDebugText(std::shared_ptr<sf::Text> spText)
	{
		sf::Vector2i mousePosition = ECSGame::Instance().GetMousePosition();
		std::shared_ptr<CameraComponent> spCamCom = GetCurrentlyActiveCamera();
		sf::Vector2f positionInWorld = ConvertWindowPositionToWorld(spCamCom->view, mousePosition);
		spText->setString("World pos: " + std::to_string(positionInWorld.x) + "; " + std::to_string(positionInWorld.y));
	}

	void UpdateMouseOverUIDebugText(std::shared_ptr<sf::Text> spText)
	{
		if (ECSGame::Instance().IsMouseOverUI())
			spText->setString("Mouse over UI: TRUE");
		else
			spText->setString("Mouse over UI: FALSE");
	}

	void UpdateFPSDebugText(std::shared_ptr<sf::Text> spText)
	{
		spText->setString(std::to_string(ECSGame::Instance().GetFPS()) + " fps");
	}




	double convertRotationalVelocityIntoPeriod(double rotationalVelocity)
	{
		return (gel::PI * 2) / rotationalVelocity;
	}

	void UpdateInfoText0(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		
		if (spSpaceSceneState->wpSelectedObjectByMouse.lock() != nullptr)
		{
			bool showPlanetInfo = false;
			std::shared_ptr<Entity> spEntity = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetEntity().lock();

			if (spSpaceSceneState->overviewType == OverviewType::Space)
			{
				std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();
				spText->setString("Name: " + spSysCom->systemName);
			}
			else if (spSpaceSceneState->overviewType == OverviewType::System)
			{
				if (spEntity->HasComponent<StarComponent>())
				{
					std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();

					if (spEntity->GetName() == "InsideSystem")
					{
						float period = (float)convertRotationalVelocityIntoPeriod(spStarCom->rotationalVelocity);
						if (period < 365 * 2)
							spText->setString("Orbital period: " + std::to_string((int)period) + " days");
						else
							spText->setString("Orbital period: " + std::to_string((int)(period / 365)) + " years");
					}
					else
					{
						spText->setString("Name: " + spStarCom->starName);
					}
				}
				else if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else if (spSpaceSceneState->overviewType == OverviewType::Planet)
			{
				if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else
				spText->setString(" ");

			if (showPlanetInfo)
			{
				std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
				spText->setString("Name: " + spPlanetCom->planetName);
			}
		}
		else
			spText->setString(" ");
	}

	void UpdateInfoText1(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		
		if (spSpaceSceneState->wpSelectedObjectByMouse.lock() != nullptr)
		{
			bool showPlanetInfo = false;
			std::shared_ptr<Entity> spEntity = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetEntity().lock();

			if (spSpaceSceneState->overviewType == OverviewType::Space)
			{
				std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();
				spText->setString("Type: " + GetSystemTypeName(spSysCom->systemType));
			}
			else if (spSpaceSceneState->overviewType == OverviewType::System)
			{
				if (spEntity->HasComponent<StarComponent>())
				{
					std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();

					if (spEntity->GetName() == "InsideSystem")
						spText->setString("Orbit radius: " + gel::roundNumberForOutput(spStarCom->orbitRadius, 2) + " AU");
					else
						spText->setString("Type: " + GetStarTypeName(spStarCom->starType));
				}
				else if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else if (spSpaceSceneState->overviewType == OverviewType::Planet)
			{
				if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else
				spText->setString(" ");

			if (showPlanetInfo)
			{
				std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
				spText->setString("Type: " + GetProperPlanetTypeName(spPlanetCom->planetType));
			}
		}
		else
			spText->setString(" ");
	}

	void UpdateInfoText2(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		
		if (spSpaceSceneState->wpSelectedObjectByMouse.lock() != nullptr)
		{
			bool showPlanetInfo = false;
			std::shared_ptr<Entity> spEntity = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetEntity().lock();

			if (spSpaceSceneState->overviewType == OverviewType::Space)
			{
				std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();
				VisitorCountPlanets visitor(false);
				spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor);
				spText->setString("Planets: " + std::to_string(visitor.counter));
			}
			else if (spSpaceSceneState->overviewType == OverviewType::System)
			{
				if (spEntity->HasComponent<StarComponent>())
				{
					std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();

					if (spEntity->GetName() == "InsideSystem")
						spText->setString(" ");
					else
						spText->setString("Mass: " + gel::roundNumberForOutput(spStarCom->starMass, 2) + " solar masses");
				}
				else if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else if (spSpaceSceneState->overviewType == OverviewType::Planet)
			{
				if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else
				spText->setString(" ");

			if (showPlanetInfo)
			{
				std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
				if (spSpaceSceneState->overviewType == OverviewType::System || !spPlanetCom->isMoon)
					spText->setString("Orbit radius: " + gel::roundNumberForOutput(spPlanetCom->orbitRadius, 2) + " AU");
				else
					spText->setString("Orbit radius: " + std::to_string((int)(spPlanetCom->orbitRadius * 1000.f)) + " km");
			}
		}
		else
			spText->setString(" ");
	}

	void UpdateInfoText3(std::shared_ptr<sf::Text> spText)
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		
		if (spSpaceSceneState->wpSelectedObjectByMouse.lock() != nullptr)
		{
			bool showPlanetInfo = false;
			std::shared_ptr<Entity> spEntity = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetEntity().lock();

			if (spSpaceSceneState->overviewType == OverviewType::Space)
			{
				std::shared_ptr<ObjectSystemComponent> spSysCom = spEntity->FindComponent<ObjectSystemComponent>().lock();
				VisitorCountHabitablePlanets visitor2;
				spSysCom->spAllSystemObjectsNode->AcceptVisitor(visitor2);
				spText->setString("Habitable planets: " + std::to_string(visitor2.counter));
			}
			else if (spSpaceSceneState->overviewType == OverviewType::System)
			{
				if (spEntity->HasComponent<StarComponent>())
				{
					std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();

					if (spEntity->GetName() == "InsideSystem")
						spText->setString(" ");
					else
						spText->setString("Radius: " + gel::roundNumberForOutput(spStarCom->starSize, 2) + " solar radiuses");
				}
				else if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else if (spSpaceSceneState->overviewType == OverviewType::Planet)
			{
				if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else
				spText->setString(" ");

			if (showPlanetInfo)
			{
				std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
				float period = (float)convertRotationalVelocityIntoPeriod(spPlanetCom->rotationalVelocity);
				if (period < 365 * 2)
					spText->setString("Orbital period: " + gel::roundNumberForOutput(period, 1) + " days");
				else
					spText->setString("Orbital period: " + gel::roundNumberForOutput(period / 365, 1) + " years");
			}
		}
		else
			spText->setString(" ");
	}

	void UpdateInfoText4(std::shared_ptr<sf::Text> spText)
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		
		if (spSpaceSceneState->wpSelectedObjectByMouse.lock() != nullptr)
		{
			bool showPlanetInfo = false;
			std::shared_ptr<Entity> spEntity = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetEntity().lock();

			if (spSpaceSceneState->overviewType == OverviewType::Space)
				spText->setString(" ");
			else if (spSpaceSceneState->overviewType == OverviewType::System)
			{
				if (spEntity->HasComponent<StarComponent>())
				{
					std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();

					if (spEntity->GetName() == "InsideSystem")
						spText->setString(" ");
					else
					{
						std::weak_ptr<ObjectSystemComponent> wpSysCom = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>();
						if (wpSysCom.lock() == nullptr)
							wpSysCom = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetParent().lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>();

						if (wpSysCom.lock() != nullptr)
						{
							if (wpSysCom.lock()->systemType != SpaceSystemType::Single)
							{
								float period = (float)convertRotationalVelocityIntoPeriod(spStarCom->rotationalVelocity);
								if (period < 365 * 2)
									spText->setString("Orbital period: " + std::to_string((int)period) + " days");
								else
									spText->setString("Orbital period: " + std::to_string((int)(period / 365)) + " years");
							}
							else
								spText->setString(" ");

						}
						else
							spText->setString(" ");
					}
				}
				else if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else if (spSpaceSceneState->overviewType == OverviewType::Planet)
			{
				if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else
				spText->setString(" ");

			if (showPlanetInfo)
			{
				std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
				spText->setString("Radius: " + std::to_string((int)(spPlanetCom->planetSize * 500.f * WorldGenerator::Instance().getSpaceMapConfig().earthDiameter)) + " km");
			}
		}
		else
			spText->setString(" ");
	}

	void UpdateInfoText5(std::shared_ptr<sf::Text> spText)
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		
		if (spSpaceSceneState->wpSelectedObjectByMouse.lock() != nullptr)
		{
			bool showPlanetInfo = false;
			std::shared_ptr<Entity> spEntity = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetEntity().lock();

			if (spSpaceSceneState->overviewType == OverviewType::Space)
				spText->setString(" ");
			else if (spSpaceSceneState->overviewType == OverviewType::System)
			{
				if (spEntity->HasComponent<StarComponent>())
				{
					std::shared_ptr<StarComponent> spStarCom = spEntity->FindComponent<StarComponent>().lock();

					if (spEntity->GetName() == "InsideSystem")
						spText->setString(" ");
					else
					{
						std::weak_ptr<ObjectSystemComponent> wpSysCom = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>();
						if (wpSysCom.lock() == nullptr)
							wpSysCom = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetParent().lock()->GetParent().lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>();

						if (wpSysCom.lock() != nullptr)
						{
							if (wpSysCom.lock()->systemType != SpaceSystemType::Single)
								spText->setString("Orbit radius: " + gel::roundNumberForOutput(spStarCom->orbitRadius, 2) + " AU");
							else
								spText->setString(" ");

						}
						else
							spText->setString(" ");
					}
				}
				else if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else if (spSpaceSceneState->overviewType == OverviewType::Planet)
			{
				if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else
				spText->setString(" ");

			if (showPlanetInfo)
			{
				std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
				spText->setString("Mass: " + gel::roundNumberForOutput(gel::sphereVolume(spPlanetCom->planetSize / 2.f) * 2.f, 2) + " earth masses");
			}
		}
		else
			spText->setString(" ");
	}

	void UpdateInfoText6(std::shared_ptr<sf::Text> spText)
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		
		if (spSpaceSceneState->wpSelectedObjectByMouse.lock() != nullptr)
		{
			bool showPlanetInfo = false;
			std::shared_ptr<Entity> spEntity = spSpaceSceneState->wpSelectedObjectByMouse.lock()->GetEntity().lock();

			if (spSpaceSceneState->overviewType == OverviewType::Space)
				spText->setString(" ");
			else if (spSpaceSceneState->overviewType == OverviewType::System)
			{
				if (spEntity->HasComponent<StarComponent>())
					spText->setString(" ");
				else if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else if (spSpaceSceneState->overviewType == OverviewType::Planet)
			{
				if (spEntity->HasComponent<PlanetComponent>())
					showPlanetInfo = true;
			}
			else
				spText->setString(" ");

			if (showPlanetInfo)
			{
				std::shared_ptr<PlanetComponent> spPlanetCom = spEntity->FindComponent<PlanetComponent>().lock();
				if (!spPlanetCom->isMoon)
				{
					VisitorCountPlanets visitor(true);
					spSpaceSceneState->wpSelectedObjectByMouse.lock()->AcceptVisitor(visitor);
					spText->setString("Moons: " + std::to_string(visitor.counter - 1));
				}
				else
					spText->setString(" ");
			}
		}
		else
			spText->setString(" ");
	}



	void UpdateDayText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		spText->setString(std::to_string(spSpaceSceneState->day));
	}

	void UpdateMonthText(std::shared_ptr<sf::Text> spText)
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		spText->setString(spSpaceSceneState->month);
	}

	void UpdateYearText(std::shared_ptr<sf::Text> spText)
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		spText->setString(std::to_string(spSpaceSceneState->year));
	}

	void UpdateSimulationStateText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		if (spSpaceSceneState->simulationState == GameState::Paused)
		{
			spText->setString("PAUSED");
			spText->setFillColor(sf::Color::Red);
		}
		else if (spSpaceSceneState->simulationState == GameState::Resumed)
		{
			spText->setString("RESUMED");
			spText->setFillColor(sf::Color::White);
		}
	}

	void UpdateSimulationSpeedText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();
		spText->setString("Simulation speed " + std::to_string((int)spSpaceSceneState->simulationSpeed) + " days/second");
	}

	void UpdateViewSizeText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();

		std::shared_ptr<CameraComponent> spCamCom = GetCurrentlyActiveCamera();
		OverviewType currentOverview = spSpaceSceneState->overviewType;
		float size = spCamCom->view.getSize().x;
		std::string part{ "." };
		if (currentOverview != OverviewType::Planet)
		{
			if (size > 100)
				part = " ";
			else if ((int)size <= 0 && (int)((size - (int)size) * 100) < 10)
				part = ".0" + std::to_string((int)((size - (int)size) * 100));
			else
				part = "." + std::to_string((int)((size - (int)size) * 100));
		}
		else
			part = " ";

		if (currentOverview == OverviewType::Space)
			spText->setString("Current camera size is " + std::to_string((int)size) + part + " light years");
		else if (currentOverview == OverviewType::System)
			spText->setString("Current camera size is " + std::to_string((int)size) + part + " astronomical units");
		else if (currentOverview == OverviewType::Planet)
			spText->setString("Current camera size is " + std::to_string((int)(size * 1000.f)) + part + " kilometers");
	}

	void UpdateOverviewText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();

		if (spSpaceSceneState->overviewType == OverviewType::Space)
			spText->setString("Space Overview");
		else if (spSpaceSceneState->overviewType == OverviewType::System)
			spText->setString(spSpaceSceneState->wpSelectedNodeIn.lock()->GetEntity().lock()->FindComponent<ObjectSystemComponent>().lock()->systemName + " System");
		else if (spSpaceSceneState->overviewType == OverviewType::Planet)
		{
			if (spSpaceSceneState->wpSelectedNodeIn.lock()->GetEntity().lock()->HasComponent<PlanetComponent>())
				spText->setString(spSpaceSceneState->wpSelectedNodeIn.lock()->GetEntity().lock()->FindComponent<PlanetComponent>().lock()->planetName + " Planet");
			else
				spText->setString("Planet Overview");
		}
	}




	void UpdatePlanetNameDistrictText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();

		if (spSpaceSceneState->wpSelectedPlanet.lock() != nullptr)
			spText->setString(spSpaceSceneState->wpSelectedPlanet.lock()->planetName);
		else
			spText->setString(" ");
	}

	void UpdateDistrictTypeText(std::shared_ptr<sf::Text> spText) 
	{
		std::shared_ptr<SpaceSceneStatesComponent> spSpaceSceneState = ECSGame::Instance().GetRoot()->GetEntity().lock()->FindComponent<SpaceSceneStatesComponent>().lock();

		if (spSpaceSceneState->wpSelectedDistrict.lock() != nullptr)
			spText->setString("Type: " + GetPlanetDistrictName(spSpaceSceneState->wpSelectedDistrict.lock()->districtType));
		else
			spText->setString("Type: ");
	}
}