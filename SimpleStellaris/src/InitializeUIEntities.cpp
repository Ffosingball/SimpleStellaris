#include "EntitiesFunctions.h"
#include "ECSGame.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SceneNode.h"
#include <memory>
#include "Components.h"
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "ResourceManager.h"
#include "CommonGameCode.h"
#include "GameState.h"
//#include "ParticlesConfigurations.h"
#include "SpaceObjectTypes.h"

//Setup text properties, where it should move. Also either xPos or yPos
//should be not null!
void SetupMoveTextProperties(const std::string textName, const float* xPos, const float* yPos, const sf::Vector2f velocity, const bool destroyAtTarget) 
{
	//Get text by name
	std::weak_ptr<Entity> wGText = ECSGame::Instance().GetEntityManager().FindEntity(textName);
	std::shared_ptr<Entity> sGText = wGText.lock();
	//Get components
	std::shared_ptr<UIPartComponent> sGTextCom = GetUIPartComponent(*sGText);
	std::shared_ptr<MovementComponent> sGMovCom = GetMovementComponent(*sGText);

	//Set animation properties for gameText
	sGTextCom->moveIt = true;
	sGTextCom->destroyAtTarget = destroyAtTarget;
	//Set target position
	if(xPos==nullptr)
		sGTextCom->targetPosition = {sGText->GetTransformable().getPosition().x, *yPos};
	else if(yPos == nullptr)
		sGTextCom->targetPosition = { *xPos, sGText->GetTransformable().getPosition().y };
	else
		sGTextCom->targetPosition = { *xPos, *yPos };
	//Set velocity
	sGMovCom->velocity = velocity;
}

//This function creates a text
std::shared_ptr<Entity> CreateGenericText(const std::string textName, const int fontSize) 
{
	//create entity
	std::shared_ptr<Entity> spUI = CreateNewEntityAt("UI", textName).lock();
	//Add component
	spUI->AddComponent(ComponentType::Text);
	spUI->AddComponent(ComponentType::UIPart);
	//Get component
	std::shared_ptr<TextComponent> spUICom = GetTextComponent(*spUI);
	//Get font from the resource manager
	std::shared_ptr<sf::Font> fontPtr = ResourceManager::Instance().LoadFont("media/fonts/PixelOperator8-Bold.ttf").lock();
	//Set text properties
	spUICom->text = std::make_shared<sf::Text>(*fontPtr);
	spUICom->text->setFillColor(sf::Color::White);
	//spUICom->text->setOutlineColor(sf::Color(100,100,100));
	//spUICom->text->setOutlineThickness(1.2f);
	spUICom->text->setCharacterSize(fontSize);

	return spUI;
}


//Creates text without moving animation
void InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position)
{
	//Check if text exist then use existing one, otherwise create new one
	std::shared_ptr<Entity> spUI = ECSGame::Instance().GetEntityManager().FindEntity(name).lock();
	//Check if this text exist
	if (spUI == nullptr)
	{
		spUI = CreateGenericText(name, fontSize);
		spUI->AddComponent(ComponentType::Movement);
	}
	//Get component
	std::shared_ptr<UIPartComponent> spUICom = GetUIPartComponent(*spUI);
	std::shared_ptr<TextComponent> spTextCom = GetTextComponent(*spUI);
	std::shared_ptr<MovementComponent> spMovCom = GetMovementComponent(*spUI);
	//set text
	spTextCom->text->setString(text);
	//Set text position
	spUI->SetPosition(position);
}


//Creates text with moving animation
void InitializeMovingText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const bool isBlinking=false, const bool isMoving = false, const float* targetX=nullptr, const float* targetY=nullptr, const sf::Vector2f velocity = {0.f,0.f}, const bool skipOriginReset=false)
{
	//Check if text exist then use existing one, otherwise create new one
	std::shared_ptr<Entity> spUI = ECSGame::Instance().GetEntityManager().FindEntity(name).lock();
	//Check if this text exist
	if (spUI == nullptr)
	{
		spUI = CreateGenericText(name, fontSize);
		spUI->AddComponent(ComponentType::Movement);
	}
	//Get component
	std::shared_ptr<UIPartComponent> spUICom = GetUIPartComponent(*spUI);
	std::shared_ptr<TextComponent> spTextCom = GetTextComponent(*spUI);
	std::shared_ptr<MovementComponent> spMovCom = GetMovementComponent(*spUI);
	//set text
	spTextCom->text->setString(text);
	//Set text position
	spUI->SetPosition(position);
	//Check if text should blink
	if (isBlinking)
	{
		//Set blinking properties
		spUICom->isBlinking = true;
		spUICom->decreasingVisibility = false;
		spUICom->blinkTime = spUICom->blinkingPeriod;
		spUICom->flatLine = true;
	}
	//Check if text should move
	if (isMoving)
	{
		//Set moving animation properties
		SetupMoveTextProperties(name, targetX, targetY, velocity, false);
	}
	//Reset text origin to center of the text if needed
	if(!skipOriginReset)
		gel::SetTextOrigin(*(spTextCom->text), position);
}


//Create debugging text at the top right corner
void CreateDebugText() 
{
	float fontSize = 20;

	InitializeText("MouseCoordsText", " ", fontSize, sf::Vector2f{0.f, 0.f});
	InitializeText("WorldCoordsText", " ", fontSize, sf::Vector2f{ 0.f, 25.f });
	InitializeText("SystemsNearByText", " ", fontSize, sf::Vector2f{ 0.f, 50.f });
}


//Creates UI of the game
void CreateUI() 
{
	sf::Vector2f iconSize{100.f, 100.f};

	//Create icon
	std::shared_ptr<Entity> spSSIcon = CreateNewEntityAt("UI", "SelectedSystemIcon").lock();

	//Add component
	spSSIcon->AddComponent(ComponentType::UIPart);
	spSSIcon->AddComponent(ComponentType::UIFollower);
	spSSIcon->AddComponent(ComponentType::RectangleShape);

	//Get component
	std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spSSIcon);
	sf::IntRect intRect({0,0}, {32,32});
	SetupRectangleShape(spRectShape, iconSize, "media/textures/selectionIcon.png", intRect);
	spRectShape->hidden = true;
}