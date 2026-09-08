#include <memory>
#include "Systems.h"
#include <SFML/Graphics.hpp>
#include "EntitiesFunctions.h"
#include "SceneNodeVisitors.h"
#include <iostream>
#include "CommonGameCode.h"
#include <SFML/Audio.hpp>
#include "ResourceManager.h"
#include "SpaceObjectTypes.h"
#include "WorldGenerator.h"
#include "ButtonsFunctions.h"

namespace ButtonSignals
{
	void ButtonHovered(std::shared_ptr<Entity> spEntity)
	{
		//std::cout << "Change hovered texture!\n";
		std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
		std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();

		if (spButton->isPressed)
		{
			spRecShape->shape.setTexture(spButton->hoveredPressedTexture.lock().get());
			spRecShape->shape.setTextureRect(spButton->hoveredPressedIntRect);
		}
		else
		{
			spRecShape->shape.setTexture(spButton->hoveredTexture.lock().get());
			spRecShape->shape.setTextureRect(spButton->hoveredIntRect);
		}
	}


	void ButtonUnhovered(std::shared_ptr<Entity> spEntity)
	{
		std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
		std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();

		if (spButton->isPressed)
		{
			spRecShape->shape.setTexture(spButton->pressedTexture.lock().get());
			spRecShape->shape.setTextureRect(spButton->pressedIntRect);
		}
		else
		{
			spRecShape->shape.setTexture(spButton->unhoveredTexture.lock().get());
			spRecShape->shape.setTextureRect(spButton->unhoveredIntRect);
		}
	}


	void ButtonReleased(std::shared_ptr<Entity> spEntity)
	{
		std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
		std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
		spRecShape->shape.setTexture(spButton->unhoveredTexture.lock().get());
		spRecShape->shape.setTextureRect(spButton->unhoveredIntRect);
	}


	void ButtonClicked(std::shared_ptr<Entity> spEntity)
	{
		std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
		std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
		spRecShape->shape.setTexture(spButton->hoveredTexture.lock().get());
		spRecShape->shape.setTextureRect(spButton->hoveredIntRect);
	}

	void ExitToMainMenuButtonPressed(std::shared_ptr<Entity> spEntity)
	{
		signals::onLoadSceneAsync("MainMenuScene");
	}


	void StartGameButtonPressed(std::shared_ptr<Entity> spEntity)
	{
		signals::onLoadSceneAsync("SpaceWorldScene");
	}


	void ExitButtonPressed(std::shared_ptr<Entity> spEntity)
	{
		ECSGame::Instance().CloseGame();
	}


	void InputBoxHovered(std::shared_ptr<Entity> spEntity) 
	{
		float outlineThikness = 4.f;
		sf::Color outlineColor = sf::Color{ 255,255,255 };

		std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setOutlineThickness(outlineThikness);
		spRectShape->shape.setOutlineColor(outlineColor);
	}


	void InputBoxUnhovered(std::shared_ptr<Entity> spEntity) 
	{
		std::shared_ptr<InputBoxComponent> spInputBox = spEntity->FindComponent<InputBoxComponent>().lock();

		if (!spInputBox->focused)
		{
			std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
			spRectShape->shape.setOutlineThickness(0.f);
			spRectShape->shape.setFillColor(sf::Color::White);
		}
	}


	void InputBoxPressed(std::shared_ptr<Entity> spEntity) 
	{
		signals::onInputBoxSelected(spEntity);
	}


	void InputBoxUnselected(std::weak_ptr<Entity> wpEntity) 
	{
		std::shared_ptr<RectangleShapeComponent> spRectShape = wpEntity.lock()->FindComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setOutlineThickness(0.f);
		spRectShape->shape.setFillColor(sf::Color::White);
	}
}

void InputSystem::DistrictHovered(std::shared_ptr<Entity> spEntity)
{
	float outlineThikness = 4.f;
	sf::Color outlineColor = sf::Color{ 255,255,255 };
	sf::Color fillColor = sf::Color{ 200,200,200 };

	std::shared_ptr<DistrictComponent> spDistrict = spEntity->FindComponent<DistrictComponent>().lock();
	wpSpaceSceneStates.lock()->wpSelectedDistrict = spDistrict;

	std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
	spRectShape->shape.setOutlineThickness(outlineThikness);
	spRectShape->shape.setOutlineColor(outlineColor);
	spRectShape->shape.setFillColor(fillColor);

	//std::cout << "District is hovered\n";

	currentDistrictShown = spDistrict->districtID;
}


void InputSystem::DistrictUnhovered(std::shared_ptr<Entity> spEntity)
{
	std::shared_ptr<DistrictComponent> spDistrict = spEntity->FindComponent<DistrictComponent>().lock();
	if (currentDistrictShown == spDistrict->districtID)
	{
		wpSpaceSceneStates.lock()->wpSelectedDistrict = {};
		currentDistrictShown = -1;
	}

	std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
	spRectShape->shape.setOutlineThickness(0.f);
	spRectShape->shape.setFillColor(sf::Color::White);
}


void InputSystem::ResumeButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	if(ECSGame::Instance().GetGameState()!=GameState::Loading)
		ChangeEscapeScreen();
}


void SimulationSystem::Slower3ButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	wpSpaceSceneStates.lock()->simulationSpeed -= 100;
	if (wpSpaceSceneStates.lock()->simulationSpeed < 1)
		wpSpaceSceneStates.lock()->simulationSpeed = 1;
}


void SimulationSystem::Slower2ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	wpSpaceSceneStates.lock()->simulationSpeed -= 10;
	if (wpSpaceSceneStates.lock()->simulationSpeed < 1)
		wpSpaceSceneStates.lock()->simulationSpeed = 1;
}


void SimulationSystem::Slower1ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	wpSpaceSceneStates.lock()->simulationSpeed -= 1;
	if (wpSpaceSceneStates.lock()->simulationSpeed < 1)
		wpSpaceSceneStates.lock()->simulationSpeed = 1;
}


void InputSystem::PlayingButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	PauseSimulation();
}


void InputSystem::StoppedButtonPressed(std::shared_ptr<Entity> spEntity)
{
	ResumeSimulation();
}


void SimulationSystem::Faster3ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	wpSpaceSceneStates.lock()->simulationSpeed += 100;
}


void SimulationSystem::Faster2ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	wpSpaceSceneStates.lock()->simulationSpeed += 10;
}


void SimulationSystem::Faster1ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	wpSpaceSceneStates.lock()->simulationSpeed += 1;
}


void MusicSystem::PreviousMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	PlayPreviousMusic();
}


void MusicSystem::NextMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	PlayNextMusic();
}


void MusicSystem::StopMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	StopMusic();
}


void MusicSystem::ResumeMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	ResumeMusic();
}


void MusicSystem::MixMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	MixMusicList();
}