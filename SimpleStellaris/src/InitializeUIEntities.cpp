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
	std::shared_ptr<UIPartComponent> sGTextCom = sGText->FindComponent<UIPartComponent>().lock();
	std::shared_ptr<MovementComponent> sGMovCom = sGText->FindComponent<MovementComponent>().lock();

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
std::shared_ptr<Entity> CreateGenericText(const std::string textName, const int fontSize, const std::string fontName, sf::Color color, std::weak_ptr<SceneNode> spCreateAt)
{
	//create entity
	std::shared_ptr<Entity> spUI;
	if (spCreateAt.lock() != nullptr)
		spUI = CreateNewEntityAt(spCreateAt.lock(), textName).lock();
	else
		spUI = CreateNewEntityAtUIRoot(textName).lock();
	//Add component
	spUI->AddComponent<UIPartComponent>();
	std::shared_ptr<TextComponent> spUICom = spUI->AddComponent<TextComponent>().lock();
	//Get font from the resource manager
	std::shared_ptr<sf::Font> fontPtr = ResourceManager::Instance().GetFont(fontName).lock();
	//Set text properties
	spUICom->text = std::make_shared<sf::Text>(*fontPtr);
	spUICom->text->setFillColor(color);
	//spUICom->text->setOutlineColor(sf::Color(100,100,100));
	//spUICom->text->setOutlineThickness(1.2f);
	spUICom->text->setCharacterSize(fontSize);

	return spUI;
}



//Creates text without moving animation
//Worst case: O(6N+3M) where N is number of components in entity and M number of components
//available in game
std::shared_ptr<Entity> InitializeText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const std::string fontName, bool centerText, sf::Color color = sf::Color{255,255,255}, std::weak_ptr<SceneNode> spCreateAt)
{
	//Check if text exist then use existing one, otherwise create new one
	std::shared_ptr<Entity> spUI = CreateGenericText(name, fontSize, fontName, color, spCreateAt);
	spUI->AddComponent<MovementComponent>();
	//Get component
	std::shared_ptr<UIPartComponent> spUICom = spUI->FindComponent<UIPartComponent>().lock();
	std::shared_ptr<TextComponent> spTextCom = spUI->FindComponent<TextComponent>().lock();
	//set text
	spTextCom->text->setString(text);
	//Set text position
	if (centerText)
		gel::CentreText(*spTextCom->text, sf::Vector2{0.f,0.f});
	
	spUI->SetPosition(position);

	return spUI;
}



//Creates text with moving animation
//Worst case: O(2N+9M+3K) where N is number entities in game and M number of components in text
// and K in number of components available
void InitializeMovingText(const std::string name, const std::string text, const int fontSize, const sf::Vector2f position, const bool isBlinking=false, const bool isMoving = false, const float* targetX=nullptr, const float* targetY=nullptr, const sf::Vector2f velocity = {0.f,0.f}, const bool skipOriginReset=false)
{
	//Create new text
	std::shared_ptr<Entity> spUI = CreateGenericText(name, fontSize, "wakaeueu", sf::Color{255,255,255});
	//Get component
	std::shared_ptr<UIPartComponent> spUICom = spUI->FindComponent<UIPartComponent>().lock();
	std::shared_ptr<TextComponent> spTextCom = spUI->FindComponent<TextComponent>().lock();
	//Add component
	std::shared_ptr<MovementComponent> spMovCom = spUI->AddComponent<MovementComponent>().lock();
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
	std::string fontName = "PixelBold";

	float uiSize = ECSGame::Instance().GetUISize();
	InitializeText("MouseCoordsText", " ", fontSize * uiSize, sf::Vector2f{0.f, 0.f} * uiSize, fontName, false);
	InitializeText("WorldCoordsText", " ", fontSize * uiSize, sf::Vector2f{ 0.f, 25.f } * uiSize, fontName, false);
	InitializeText("SystemsNearByText", " ", fontSize * uiSize, sf::Vector2f{ 0.f, 50.f } * uiSize, fontName, false);
	InitializeText("FPSText", " ", fontSize * uiSize, sf::Vector2f{ 0.f, 75.f } * uiSize, fontName, false);
	InitializeText("DaysPastText", " ", fontSize * uiSize, sf::Vector2f{ 0.f, 100.f } * uiSize, fontName, false);
	InitializeText("DateText", " ", fontSize * uiSize, sf::Vector2f{ 0.f, 125.f } * uiSize, fontName, false);
	InitializeText("RenderText", " ", fontSize * uiSize, sf::Vector2f{ 0.f, 150.f } * uiSize, fontName, false);
}



//Creates UI of the game
//Worst case: O(4N+M) where N is number of components in entity and M number of components
//available in game
void CreateUI() 
{
	sf::Vector2f iconSize{100.f, 100.f};
	sf::Vector2f uiTopPartSize{ 1000.f, 120.f };
	sf::Vector2f uiBottomPartSize{ 1200.f, 200.f };
	sf::Vector2f uiInfoPartSize{ 800.f, 700.f };
	float dateFontSize = 32;
	float simulationFontSize = 25;
	float metricsFontSize = 22;
	float mainFontSize = 40;
	float infoFontSize = 25;
	std::string fontName = "PixelBold";
	sf::Color importantColor = sf::Color{ 235, 175, 38 };
	sf::Color usualColor = sf::Color{ 255,255,255 };

	float uiSize = ECSGame::Instance().GetUISize();
	std::shared_ptr<SceneNode> spUIRootNode = ECSGame::Instance().GetUIRoot();
	//CREATE SELECTION ICON
	std::shared_ptr<Entity> spSSIcon = CreateNewEntityAtUIRoot("SelectedSystemIcon").lock();
	//Add component
	spSSIcon->AddComponent<UIPartComponent>();
	spSSIcon->AddComponent<UIFollowerComponent>();
	std::shared_ptr<RectangleShapeComponent> spRectShape = spSSIcon->AddComponent<RectangleShapeComponent>().lock();
	SetupRectangleShape(spRectShape, iconSize * uiSize, "SelectionIcon");
	spSSIcon->hidden = true;

	//CREATE Upper and lower parts of ui
	std::shared_ptr<Entity> spToPart = CreateNewEntityAtUIRoot("UpperPart").lock();
	//Add component
	spToPart->AddComponent<UIPartComponent>();
	std::shared_ptr<RectangleShapeComponent> spRectShape2 = spToPart->AddComponent<RectangleShapeComponent>().lock();
	SetupRectangleShape(spRectShape2, uiTopPartSize * uiSize, "TopUIPart");
	spToPart->SetPosition(sf::Vector2f{1280.f,uiTopPartSize.y/2.f}*uiSize);

	std::shared_ptr<Entity> spLoPart = CreateNewEntityAtUIRoot("LowerPart").lock();
	//Add component
	spLoPart->AddComponent<UIPartComponent>();
	std::shared_ptr<RectangleShapeComponent> spRectShape3 = spLoPart->AddComponent<RectangleShapeComponent>().lock();
	SetupRectangleShape(spRectShape3, uiBottomPartSize * uiSize, "BottomUIPart");
	spLoPart->SetPosition(sf::Vector2f{ 1280.f,1600.f-(uiBottomPartSize.y / 2.f) } * uiSize);

	std::shared_ptr<SceneNode> spLowerPartNode = spUIRootNode->FindChild(*spLoPart).lock();
	std::shared_ptr<SceneNode> spUpperPartNode = spUIRootNode->FindChild(*spToPart).lock();
	//CREATE UI textes
	InitializeText("MonthText", " ", dateFontSize * uiSize, sf::Vector2f{ 140.f, 0.f } * uiSize, fontName, true, importantColor, spLowerPartNode);
	InitializeText("DayText", " ", dateFontSize * uiSize, sf::Vector2f{ -110.f, 0.f } * uiSize, fontName, true, importantColor, spLowerPartNode);
	InitializeText("YearText", " ", dateFontSize * uiSize, sf::Vector2f{ 380.f, 0.f } * uiSize, fontName, true, importantColor, spLowerPartNode);
	InitializeText("SimulationStateText", " ", dateFontSize * uiSize, sf::Vector2f{ -300.f, 0.f } * uiSize, fontName, true, usualColor, spLowerPartNode);
	InitializeText("SimulationSpeedText", " ", simulationFontSize * uiSize, sf::Vector2f{ 0.f, -60.f } * uiSize, fontName, true, usualColor, spLowerPartNode);
	InitializeText("ViewSizeText", " ", metricsFontSize * uiSize, sf::Vector2f{ 0.f, 60.f } * uiSize, fontName, true, usualColor, spLowerPartNode);
	InitializeText("OverviewText", " ", mainFontSize * uiSize, sf::Vector2f{ 0.f, 0.f } * uiSize, fontName, true, importantColor, spUpperPartNode);

	//CREATE SIDE part of ui
	std::shared_ptr<Entity> spInfoPart = CreateNewEntityAtUIRoot("InfoPart").lock();
	//Add component
	spInfoPart->AddComponent<UIPartComponent>();
	std::shared_ptr<RectangleShapeComponent> spRectShape4 = spInfoPart->AddComponent<RectangleShapeComponent>().lock();
	SetupRectangleShape(spRectShape4, uiInfoPartSize * uiSize, "UIPartSide");
	spInfoPart->SetPosition(sf::Vector2f{ 300.f, 800.f } * uiSize);

	std::shared_ptr<SceneNode> spInfoPartNode = spUIRootNode->FindChild(*spInfoPart).lock();
	//CREATE INFO textes
	InitializeText("InfoText0", " ", infoFontSize * uiSize, sf::Vector2f{ -260.f, -240.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
	InitializeText("InfoText1", " ", infoFontSize * uiSize, sf::Vector2f{ -260.f, -160.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
	InitializeText("InfoText2", " ", infoFontSize * uiSize, sf::Vector2f{ -260.f,-80.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
	InitializeText("InfoText3", " ", infoFontSize * uiSize, sf::Vector2f{ -260.f, 0.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
	InitializeText("InfoText4", " ", infoFontSize * uiSize, sf::Vector2f{ -260.f, 80.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
	InitializeText("InfoText5", " ", infoFontSize * uiSize, sf::Vector2f{ -260.f, 160.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
	InitializeText("InfoText6", " ", infoFontSize * uiSize, sf::Vector2f{ -260.f, 240.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
}



//Worst case: O(4N+3M+6K) where N is number of components in provided entity and M is
//number of components available in game and K number of components to add to the text
std::shared_ptr<Entity> CreateSystemText(std::shared_ptr<SceneNode> systemNode, std::shared_ptr<SceneNode> spNodeToFollow, std::string& entityName, bool hideIfZoomLarge)
{
	//static int counter = 0;
	float fontSize = 22;
	float nebulaFontSize = 40;

	float uiSize = ECSGame::Instance().GetUISize();
	std::string name{"UNDEFINED"};
	std::shared_ptr<Entity> spEntityToFollow = spNodeToFollow->GetEntity().lock();
	if (spEntityToFollow->HasComponent<ObjectSystemComponent>())
		name = spEntityToFollow->FindComponent<ObjectSystemComponent>().lock()->systemName;
	else if (spEntityToFollow->HasComponent<StarComponent>())
		name = spEntityToFollow->FindComponent<StarComponent>().lock()->starName;
	else if (spEntityToFollow->HasComponent<PlanetComponent>())
		name = spEntityToFollow->FindComponent<PlanetComponent>().lock()->planetName;
	else if (spEntityToFollow->HasComponent<NebulaComponent>())
	{
		name = spEntityToFollow->FindComponent<NebulaComponent>().lock()->nebulaName;
		fontSize = nebulaFontSize;
	}
	else
		name = entityName;
	//Create text
	std::shared_ptr<Entity> spText = InitializeText(entityName, name, fontSize * uiSize, sf::Vector2f{0,0}, "Pixel", false, sf::Color::White, systemNode);
	
	//Add component
	std::shared_ptr<UIFollowerComponent> spUIFollower = spText->AddComponent<UIFollowerComponent>().lock();
	spUIFollower->nodeToFollow = spNodeToFollow;
	spUIFollower->hideIfZoomLargeEnough = hideIfZoomLarge;

	std::shared_ptr<TextComponent> spUIText = spText->FindComponent<TextComponent>().lock();
	gel::CentreText(*spUIText->text, sf::Vector2f{ 0,fontSize*2.f * uiSize });
	spUIText->text->setFillColor(sf::Color(229,229,229));
	spUIText->text->setOutlineColor(sf::Color(50,50,50));
	spUIText->text->setOutlineThickness(1.f * uiSize);

	//counter++;

	return spText;
}



//Worst case: O(3N+2M) where N is number of components in entity and M number of components
//available in game
void InitializeMouseIcon() 
{
	sf::Vector2f mouseSize{ 60.f, 60.f };
	float uiSize = ECSGame::Instance().GetUISize();

	std::weak_ptr<Entity> wpMouseIcon = ECSGame::Instance().GetEntityManager().NewEntity("MouseIcon");
	ECSGame::Instance().GetUIRoot()->AddChild(std::make_shared<SceneNode>(wpMouseIcon));
	//Add components
	wpMouseIcon.lock()->AddComponent<UIPartComponent>();
	std::shared_ptr<RectangleShapeComponent> spRectShape = wpMouseIcon.lock()->AddComponent<RectangleShapeComponent>().lock();
	SetupRectangleShape(spRectShape, mouseSize * uiSize, "MouseIcon");
	spRectShape->shape.setPosition({32,32});
}


//Creates icons for system overview
//Worst case: O(4N+M) where N is number of components in entity and M number of components
//available in game
void CreateIconForSystemOverview(std::shared_ptr<SceneNode> nodeToFollow, std::shared_ptr<SceneNode> createIconIn, std::string iconTexture, std::string name, bool hideIfZoomLarge, sf::Vector2f iconSize, bool hideIfZoomSmall, sf::Vector2f zoomLevelAtWhichHide)
{
	//if (iconTexture == "CenterOfMassIcon")
	//	std::cout << systemNode->GetCombinedParentsNames()<<'\n';
	float uiSize = ECSGame::Instance().GetUISize();

	//Create selection icon
	std::shared_ptr<Entity> spSSIcon = CreateNewEntityAt(createIconIn, name).lock();
	//Add component
	spSSIcon->AddComponent<UIPartComponent>();
	std::shared_ptr<RectangleShapeComponent> spRectShape = spSSIcon->AddComponent<RectangleShapeComponent>().lock();
	SetupRectangleShape(spRectShape, iconSize * uiSize, iconTexture);
	std::shared_ptr<UIFollowerComponent> spUIFollower = spSSIcon->AddComponent<UIFollowerComponent>().lock();
	spUIFollower->nodeToFollow = nodeToFollow;
	spUIFollower->hideIfZoomLargeEnough = hideIfZoomLarge;
	spUIFollower->hideIfZoomSmallEnough = hideIfZoomSmall;
	spUIFollower->zoomLevelsAtWhichHideEntity = zoomLevelAtWhichHide;
}



void CreateOrbitFor(std::shared_ptr<SceneNode> spParentNode, std::string name, bool inheritParentPosition, double orbitRadius, std::weak_ptr<SceneNode> wpNodeToFollow, float outlineThikness, sf::Color outlineColor, bool hideIfZoomLarge)
{
	//Create orbit
	std::shared_ptr<Entity> spOrbitE = CreateNewEntityAt(spParentNode, name).lock();
	//Add component
	spOrbitE->AddComponent<UIPartComponent>();
	spOrbitE->inheritParentPosition = inheritParentPosition;

	std::shared_ptr<OrbitVisualizerComponent> spOrbitVis = spOrbitE->AddComponent<OrbitVisualizerComponent>().lock();
	spOrbitVis->orbitShape.setPointCount(300);
	spOrbitVis->orbitShape.setOutlineColor(outlineColor);
	spOrbitVis->orbitShape.setOutlineThickness(outlineThikness);
	spOrbitVis->orbitShape.setFillColor(sf::Color(0, 0, 0, 0));
	spOrbitVis->orbitSize = orbitRadius*0.9996;

	std::shared_ptr<UIFollowerComponent> spUIFollower = spOrbitE->AddComponent<UIFollowerComponent>().lock();
	spUIFollower->nodeToFollow = wpNodeToFollow;
	spUIFollower->hideIfZoomLargeEnough = hideIfZoomLarge;
	spUIFollower->hideIfOutsideOfCamera = false;
}