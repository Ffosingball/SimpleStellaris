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


	bool ResetWorldGenerator()
	{
		SpaceMapConfigurations mapConfig;
		std::shared_ptr<InputBoxComponent> spInputBoxCom = ECSGame::Instance().GetUINode()->FindChild("GenerationConfigScreen").lock()->FindChild("SeedInputBox").lock()->GetEntity().lock()->FindComponent<InputBoxComponent>().lock();
		int seed = 0;
		bool success = true;
		try 
		{
			seed = std::stoi(spInputBoxCom->text);
		}
		catch (const std::out_of_range&)
		{
			return false;
		}

		WorldGenerator::Instance().ResetGenerator(seed, mapConfig);
		return true;
	}

	void CreateWorldButtonPressed(std::shared_ptr<Entity> spEntity)
	{
		if(ResetWorldGenerator())
			signals::onLoadSceneAsync("SpaceWorldScene");
		else
		{
			std::shared_ptr<TextComponent> spTextCom = ECSGame::Instance().GetUINode()->FindChild("GenerationConfigScreen").lock()->FindChild("ErrorText").lock()->GetEntity().lock()->FindComponent<TextComponent>().lock();
			spTextCom->text->setString("Number is out of range!");
			gel::AlignTextToLeftSide(*spTextCom->text, sf::Vector2f{ 0.f,0.f });
		}
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
		}
	}


	void InputBoxPressed(std::shared_ptr<Entity> spEntity) 
	{
		float outlineThikness = 4.f;
		sf::Color outlineColor = sf::Color{ 255,255,255 };

		std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setOutlineThickness(outlineThikness);
		spRectShape->shape.setOutlineColor(outlineColor);
		signals::onInputBoxSelected(spEntity);
	}


	void InputBoxUnselected(std::weak_ptr<Entity> wpEntity) 
	{
		std::shared_ptr<RectangleShapeComponent> spRectShape = wpEntity.lock()->FindComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setOutlineThickness(0.f);
	}


	void ChangeSeedButtonPressed(std::shared_ptr<Entity> spEntity) 
	{
		if (ECSGame::Instance().GetGameState() != GameState::Loading)
		{
			std::shared_ptr<InputBoxComponent> spInputBoxCom = ECSGame::Instance().GetUINode()->FindChild("GenerationConfigScreen").lock()->FindChild("SeedInputBox").lock()->GetEntity().lock()->FindComponent<InputBoxComponent>().lock();
			std::random_device rd;
			spInputBoxCom->text = std::to_string((int)rd());
		}
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


void InputSystem::BackToMainMenuButtonPressed(std::shared_ptr<Entity> spEntity)
{
	if (ECSGame::Instance().GetGameState() != GameState::Loading)
		ChangeMainMenuScreen();
}


void InputSystem::StartGameButtonPressed(std::shared_ptr<Entity> spEntity)
{
	if (ECSGame::Instance().GetGameState() != GameState::Loading)
	{
		std::shared_ptr<InputBoxComponent> spInputBoxCom = ECSGame::Instance().GetUINode()->FindChild("GenerationConfigScreen").lock()->FindChild("SeedInputBox").lock()->GetEntity().lock()->FindComponent<InputBoxComponent>().lock();
		std::random_device rd;
		spInputBoxCom->text = std::to_string((int)rd());

		std::shared_ptr<TextComponent> spTextCom = ECSGame::Instance().GetUINode()->FindChild("GenerationConfigScreen").lock()->FindChild("ErrorText").lock()->GetEntity().lock()->FindComponent<TextComponent>().lock();
		spTextCom->text->setString(" ");

		ChangeMainMenuScreen();
	}
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