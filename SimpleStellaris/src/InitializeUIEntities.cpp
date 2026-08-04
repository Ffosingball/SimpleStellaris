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
	std::shared_ptr<Entity> spUI = CreateNewEntityAtUIRoot(textName).lock();
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
std::shared_ptr<Entity> InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, OverviewType overviewType)
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
	//set text
	spTextCom->text->setString(text);
	spTextCom->drawAt = overviewType;
	//gel::CentreText(*spTextCom->text, sf::Vector2f{ 0,0 });
	//Set text position
	spUI->SetPosition(position);

	return spUI;
}


//Creates text without moving animation at provided node
std::shared_ptr<Entity> InitializeTextAt(std::shared_ptr<SceneNode> spNode, const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, OverviewType overviewType)
{
	//create entity
	std::shared_ptr<Entity> spUI = CreateNewEntityAt(spNode, name).lock();

	//Add component
	spUI->AddComponent(ComponentType::Text);
	spUI->AddComponent(ComponentType::UIPart);

	//Get component
	std::shared_ptr<TextComponent> spUICom = GetTextComponent(*spUI);
	//Get font from the resource manager
	std::shared_ptr<sf::Font> fontPtr = ResourceManager::Instance().LoadFont("media/fonts/PixelOperator8.ttf").lock();
	
	//Set text properties
	spUICom->text = std::make_shared<sf::Text>(*fontPtr);
	spUICom->text->setFillColor(sf::Color::White);
	spUICom->text->setCharacterSize(fontSize);
	spUICom->text->setString(text);
	spUICom->drawAt = overviewType;

	//Set text position
	//gel::CentreText(*spUICom->text, sf::Vector2f{ 0,0 });
	spUI->SetPosition(position);

	return spUI;
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

	InitializeText("MouseCoordsText", " ", fontSize, sf::Vector2f{0.f, 0.f}, OverviewType::Always);
	InitializeText("WorldCoordsText", " ", fontSize, sf::Vector2f{ 0.f, 25.f }, OverviewType::Always);
	InitializeText("SystemsNearByText", " ", fontSize, sf::Vector2f{ 0.f, 50.f }, OverviewType::Space);
	InitializeText("FPSText", " ", fontSize, sf::Vector2f{ 0.f, 75.f }, OverviewType::Always);
}


//Creates UI of the game
void CreateUI() 
{
	sf::Vector2f iconSize{100.f, 100.f};

	//Create icon
	std::shared_ptr<Entity> spSSIcon = CreateNewEntityAtUIRoot("SelectedSystemIcon").lock();

	//Add component
	spSSIcon->AddComponent(ComponentType::UIPart);
	spSSIcon->AddComponent(ComponentType::UIFollower);
	spSSIcon->AddComponent(ComponentType::RectangleShape);

	//Get component
	std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spSSIcon);
	sf::IntRect intRect({0,0}, {32,32});
	SetupRectangleShape(spRectShape, iconSize, "media/textures/selectionIcon.png", OverviewType::Always, intRect);
	spRectShape->hidden = true;
}


void CreateSystemText(std::shared_ptr<SceneNode> systemNode, std::shared_ptr<Entity> spEntityToFollow, OverviewType overviewType, std::string& entityName)
{
	static int counter = 0;
	float fontSize = 22;

	std::string name{"UNDEFINED"};
	if (spEntityToFollow->HasComponent(ComponentType::ObjectSystem))
		name = GetObjectSystemComponent(*spEntityToFollow)->systemName;
	else if(spEntityToFollow->HasComponent(ComponentType::Star))
		name = GetStarComponent(*spEntityToFollow)->starName;
	else if (spEntityToFollow->HasComponent(ComponentType::Planet))
		name = GetPlanetComponent(*spEntityToFollow)->planetName;
	//Create text
	std::shared_ptr<Entity> spText = InitializeTextAt(systemNode, entityName+std::to_string(counter), name, fontSize, sf::Vector2f{0,0}, overviewType);
	
	//Add component
	spText->AddComponent(ComponentType::UIFollower);
	std::shared_ptr<UIFollowerComponent> spUIFollower = GetUIFollowerComponent(*spText);
	spUIFollower->entityToFollow = spEntityToFollow;
	spUIFollower->hideIfZoomLargeEnough = true;

	std::shared_ptr<TextComponent> spUIText = GetTextComponent(*spText);
	gel::CentreText(*spUIText->text, sf::Vector2f{ 0,fontSize*2.f });
	spUIText->text->setFillColor(sf::Color(229,229,229));
	spUIText->text->setOutlineColor(sf::Color(50,50,50));
	spUIText->text->setOutlineThickness(1.f);
	spUIText->drawAt = overviewType;

	counter++;
}


void InitializeMouseIcon() 
{
	sf::Vector2f mouseSize{ 60.f, 60.f };

	std::weak_ptr<Entity> wpMouseIcon = ECSGame::Instance().GetEntityManager().NewEntity("MouseIcon");
	ECSGame::Instance().GetUIRoot()->AddChild(std::make_shared<SceneNode>(wpMouseIcon));
	wpMouseIcon.lock()->AddComponent(ComponentType::UIPart);
	wpMouseIcon.lock()->AddComponent(ComponentType::RectangleShape);

	//Get component
	std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*wpMouseIcon.lock());
	SetupRectangleShape(spRectShape, mouseSize, "media/textures/mouseIcon.png", OverviewType::Always);
	spRectShape->shape.setPosition({32,32});
}