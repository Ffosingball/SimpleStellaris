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


void InputSystem::DistrictHovered(std::shared_ptr<Entity> spEntity)
{
	float outlineThikness = 4.f;
	sf::Color outlineColor = sf::Color{ 255,255,255 };
	sf::Color fillColor = sf::Color{ 200,200,200 };

	std::shared_ptr<DistrictComponent> spDistrict = spEntity->FindComponent<DistrictComponent>().lock();
	districtTypeText.lock()->text->setString("Type: " + GetPlanetDistrictName(spDistrict->districtType));
	gel::AlignTextToLeftSide(*districtTypeText.lock()->text, sf::Vector2 { 0.f, 0.f });

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
		districtTypeText.lock()->text->setString("Type: ");
		gel::AlignTextToLeftSide(*districtTypeText.lock()->text, sf::Vector2 { 0.f, 0.f });
		currentDistrictShown = -1;
	}

	std::shared_ptr<RectangleShapeComponent> spRectShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
	spRectShape->shape.setOutlineThickness(0.f);
	spRectShape->shape.setFillColor(sf::Color::White);
}


void InputSystem::ButtonHovered(std::shared_ptr<Entity> spEntity) 
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


void InputSystem::ButtonUnhovered(std::shared_ptr<Entity> spEntity)
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


void InputSystem::ButtonReleased(std::shared_ptr<Entity> spEntity)
{
	std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
	std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
	spRecShape->shape.setTexture(spButton->unhoveredTexture.lock().get());
	spRecShape->shape.setTextureRect(spButton->unhoveredIntRect);
}


void InputSystem::ButtonClicked(std::shared_ptr<Entity> spEntity)
{
	std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
	std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
	spRecShape->shape.setTexture(spButton->hoveredTexture.lock().get());
	spRecShape->shape.setTextureRect(spButton->hoveredIntRect);
}


void SetupPressedButtonTexture(std::shared_ptr<Entity> spEntity)
{
	std::shared_ptr<ButtonComponent> spButton = spEntity->FindComponent<ButtonComponent>().lock();
	std::shared_ptr<RectangleShapeComponent> spRecShape = spEntity->FindComponent<RectangleShapeComponent>().lock();
	spRecShape->shape.setTexture(spButton->hoveredPressedTexture.lock().get());
	spRecShape->shape.setTextureRect(spButton->hoveredPressedIntRect);
}


void InputSystem::ResumeButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	SetupPressedButtonTexture(spEntity);
	ChangeEscapeScreen();
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::ExitButtonButtonPressed(std::shared_ptr<Entity> spEntity)
{
	ECSGame::Instance().CloseGame();
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::Slower3ButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	SetupPressedButtonTexture(spEntity);
	ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 100);
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::Slower2ButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	SetupPressedButtonTexture(spEntity);
	ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 10);
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::Slower1ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() - 1);
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::PlayingButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	SetupPressedButtonTexture(spEntity);
	PauseSimulation();
}


void InputSystem::StoppedButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	ResumeSimulation();
}


void InputSystem::Faster3ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 100);
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::Faster2ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 10);
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::Faster1ButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	ECSGame::Instance().SetSimulationSpeed(ECSGame::Instance().GetSimulationSpeed() + 1);
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::PreviousMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	musicSystem->PlayPreviousMusic();
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::NextMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	musicSystem->PlayNextMusic();
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::StopMusicButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	SetupPressedButtonTexture(spEntity);
	musicSystem->StopMusic();
	wpStopMusicButton.lock()->hidden = true;
	wpResumeMusicButton.lock()->hidden = false;
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::ResumeMusicButtonPressed(std::shared_ptr<Entity> spEntity) 
{
	SetupPressedButtonTexture(spEntity);
	musicSystem->ResumeMusic();
	wpStopMusicButton.lock()->hidden = false;
	wpResumeMusicButton.lock()->hidden = true;
	musicSystem->PlayPressedButtonSFX();
}


void InputSystem::MixMusicButtonPressed(std::shared_ptr<Entity> spEntity)
{
	SetupPressedButtonTexture(spEntity);
	musicSystem->MixMusicList();
	musicSystem->PlayPressedButtonSFX();
}