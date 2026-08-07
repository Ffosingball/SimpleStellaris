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
//Worst case: O(N+2M) where N is number of entities in game and M number of components in the text
void SetupMoveTextProperties(const std::string textName, std::shared_ptr<SceneNode> nodeWithName, const float* xPos, const float* yPos, const sf::Vector2f velocity, const bool destroyAtTarget) 
{
	//Get text by name
	std::weak_ptr<SceneNode> wGNode = nodeWithName->FindChild(textName);
	std::shared_ptr<Entity> sGText = wGNode.lock()->GetEntity().lock();
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
//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
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
	std::shared_ptr<sf::Font> fontPtr = ResourceManager::Instance().GetFont("PixelBold").lock();
	//Set text properties
	spUICom->text = std::make_shared<sf::Text>(*fontPtr);
	spUICom->text->setFillColor(sf::Color::White);
	//spUICom->text->setOutlineColor(sf::Color(100,100,100));
	//spUICom->text->setOutlineThickness(1.2f);
	spUICom->text->setCharacterSize(fontSize);

	return spUI;
}



//Creates text without moving animation
//Worst case: O(6N+3M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position)
{
	//Check if text exist then use existing one, otherwise create new one
	std::shared_ptr<Entity> spUI = CreateGenericText(name, fontSize);
	spUI->AddComponent(ComponentType::Movement);
	//Get component
	std::shared_ptr<UIPartComponent> spUICom = GetUIPartComponent(*spUI);
	std::shared_ptr<TextComponent> spTextCom = GetTextComponent(*spUI);
	//set text
	spTextCom->text->setString(text);
	//gel::CentreText(*spTextCom->text, sf::Vector2f{ 0,0 });
	//Set text position
	spUI->SetPosition(position);

	return spUI;
}



//Creates text without moving animation at provided node
//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> InitializeTextAt(std::shared_ptr<SceneNode> spNode, const std::string name, const std::string text, const int fontSize, const sf::Vector2f position)
{
	//create entity
	std::shared_ptr<Entity> spUI = CreateNewEntityAt(spNode, name).lock();

	//Add component
	spUI->AddComponent(ComponentType::Text);
	spUI->AddComponent(ComponentType::UIPart);

	//Get component
	std::shared_ptr<TextComponent> spUICom = GetTextComponent(*spUI);
	//Get font from the resource manager
	std::shared_ptr<sf::Font> fontPtr = ResourceManager::Instance().GetFont("Pixel").lock();
	
	//Set text properties
	spUICom->text = std::make_shared<sf::Text>(*fontPtr);
	spUICom->text->setFillColor(sf::Color::White);
	spUICom->text->setCharacterSize(fontSize);
	spUICom->text->setString(text);

	//Set text position
	//gel::CentreText(*spUICom->text, sf::Vector2f{ 0,0 });
	spUI->SetPosition(position);

	return spUI;
}



//Creates text with moving animation
//Worst case: O(2N+9M+3K) where N is number entities in game and M number of components in text
// and K in number of components available
void InitializeMovingText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const bool isBlinking=false, const bool isMoving = false, const float* targetX=nullptr, const float* targetY=nullptr, const sf::Vector2f velocity = {0.f,0.f}, const bool skipOriginReset=false)
{
	//Create new text
	std::shared_ptr<Entity> spUI = CreateGenericText(name, fontSize);
	spUI->AddComponent(ComponentType::Movement);
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
		SetupMoveTextProperties(name, ECSGame::Instance().GetUIRoot(), targetX, targetY, velocity, false);
	}
	//Reset text origin to center of the text if needed
	if(!skipOriginReset)
		gel::SetTextOrigin(*(spTextCom->text), position);
}



//Create debugging text at the top right corner
//Worst case: O((6N+3M)*K) where N is number of components in entity and M number of components
//available in game and K number of texts to create
void CreateDebugText() 
{
	float fontSize = 20;

	InitializeText("MouseCoordsText", " ", fontSize, sf::Vector2f{0.f, 0.f});
	InitializeText("WorldCoordsText", " ", fontSize, sf::Vector2f{ 0.f, 25.f });
	InitializeText("SystemsNearByText", " ", fontSize, sf::Vector2f{ 0.f, 50.f });
	InitializeText("FPSText", " ", fontSize, sf::Vector2f{ 0.f, 75.f });
	InitializeText("DaysPastText", " ", fontSize, sf::Vector2f{ 0.f, 100.f });
	InitializeText("DateText", " ", fontSize, sf::Vector2f{ 0.f, 125.f });
	InitializeText("RenderText", " ", fontSize, sf::Vector2f{ 0.f, 150.f });
}



//Creates UI of the game
//Worst case: O(4N+M) where N is number of components in entity and M number of components
//available in game
void CreateUI() 
{
	sf::Vector2f iconSize{100.f, 100.f};

	//Create selection icon
	std::shared_ptr<Entity> spSSIcon = CreateNewEntityAtUIRoot("SelectedSystemIcon").lock();
	//Add component
	spSSIcon->AddComponent(ComponentType::UIPart);
	spSSIcon->AddComponent(ComponentType::UIFollower);
	spSSIcon->AddComponent(ComponentType::RectangleShape);
	//Get component
	std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spSSIcon);
	sf::IntRect intRect({0,0}, {32,32});
	SetupRectangleShape(spRectShape, iconSize, "SelectionIcon");
	spSSIcon->hidden = true;
}



//Worst case: O(4N+3M+6K) where N is number of components in provided entity and M is
//number of components available in game and K number of components to add to the text
std::shared_ptr<Entity> CreateSystemText(std::shared_ptr<SceneNode> systemNode, std::shared_ptr<SceneNode> spNodeToFollow, std::string& entityName, bool hideIfZoomLarge)
{
	static int counter = 0;
	float fontSize = 22;

	std::string name{"UNDEFINED"};
	std::shared_ptr<Entity> spEntityToFollow = spNodeToFollow->GetEntity().lock();
	if (spEntityToFollow->HasComponent(ComponentType::ObjectSystem))
		name = GetObjectSystemComponent(*spEntityToFollow)->systemName;
	else if(spEntityToFollow->HasComponent(ComponentType::Star))
		name = GetStarComponent(*spEntityToFollow)->starName;
	else if (spEntityToFollow->HasComponent(ComponentType::Planet))
		name = GetPlanetComponent(*spEntityToFollow)->planetName;
	//Create text
	std::shared_ptr<Entity> spText = InitializeTextAt(systemNode, entityName+std::to_string(counter), name, fontSize, sf::Vector2f{0,0});
	
	//Add component
	spText->AddComponent(ComponentType::UIFollower);
	std::shared_ptr<UIFollowerComponent> spUIFollower = GetUIFollowerComponent(*spText);
	spUIFollower->nodeToFollow = spNodeToFollow;
	spUIFollower->hideIfZoomLargeEnough = hideIfZoomLarge;

	std::shared_ptr<TextComponent> spUIText = GetTextComponent(*spText);
	gel::CentreText(*spUIText->text, sf::Vector2f{ 0,fontSize*2.f });
	spUIText->text->setFillColor(sf::Color(229,229,229));
	spUIText->text->setOutlineColor(sf::Color(50,50,50));
	spUIText->text->setOutlineThickness(1.f);

	counter++;

	return spText;
}



//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
void InitializeMouseIcon() 
{
	sf::Vector2f mouseSize{ 60.f, 60.f };

	std::weak_ptr<Entity> wpMouseIcon = ECSGame::Instance().GetEntityManager().NewEntity("MouseIcon");
	ECSGame::Instance().GetUIRoot()->AddChild(std::make_shared<SceneNode>(wpMouseIcon));
	wpMouseIcon.lock()->AddComponent(ComponentType::UIPart);
	wpMouseIcon.lock()->AddComponent(ComponentType::RectangleShape);

	//Get component
	std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*wpMouseIcon.lock());
	SetupRectangleShape(spRectShape, mouseSize, "MouseIcon");
	spRectShape->shape.setPosition({32,32});
}


//Creates icons for system overview
//Worst case: O(4N+M) where N is number of components in entity and M number of components
//available in game
void CreateIconForSystemOverview(std::shared_ptr<SceneNode> systemNode, std::string iconTexture)
{
	sf::Vector2f iconSize{ 100.f, 100.f };

	//if (iconTexture == "CenterOfMassIcon")
	//	std::cout << systemNode->GetCombinedParentsNames()<<'\n';

	//Create selection icon
	std::shared_ptr<Entity> spSSIcon = CreateNewEntityAtUIRoot("ObjectIcon").lock();
	//Add component
	spSSIcon->AddComponent(ComponentType::UIPart);
	spSSIcon->AddComponent(ComponentType::UIFollower);
	spSSIcon->AddComponent(ComponentType::RectangleShape);
	//Get component
	std::shared_ptr<RectangleShapeComponent> spRectShape = GetRectangleShapeComponent(*spSSIcon);
	SetupRectangleShape(spRectShape, iconSize, iconTexture);
	spSSIcon->hidden = true;
	std::shared_ptr<UIFollowerComponent> spUIFollower = GetUIFollowerComponent(*spSSIcon);
	spUIFollower->nodeToFollow = systemNode;
}