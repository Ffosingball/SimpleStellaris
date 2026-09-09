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
#include "WorldGenerator.h"
#include "SceneNodeVisitors.h"
#include "CompilerInstructions.h"
#include "ButtonsFunctions.h"
#include "TextFunctions.h"


namespace SpaceWorldScene
{
	//Creates UI camera
	//Worst case: O(2N+M) where N is number of components available in game and M number of components
	//available in game
	void InitializeUICamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
	{
		//Create camera
		std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "UICamera").lock();
		//Add component
		//Get component
		std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
		//set camera properties
		spCameraCom->view.setSize(static_cast<sf::Vector2f>(windowSize));
		spCameraCom->view.setCenter(static_cast<sf::Vector2f>(windowSize) / 2.f);
	}



	//Creates a camera
	//Worst case: O(4N+M) where N is number of components available in game and M number of components
	//available in game
	void InitializeSpaceCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
	{
		int cameraHeight = 300;
		float cameraVelocity = 20.f;//14.f
		float cameraZoomSpeed = 5.f;
		float velocityChange = 8.f;
		sf::Vector2f zoomBorders = { 0.3f, 1.6f };
		float outsideBordersMaxRenderDistance = 15.f;

		//Create camera
		std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "SpaceCamera").lock();
		//Add component
		//Get component
		std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
		std::shared_ptr<MovementComponent> spMovementCom = spCamera->AddComponent<MovementComponent>().lock();
		//set camera properties
		float windowSizeRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		//Set camera sizes
		spCameraCom->view.setSize(sf::Vector2f{ (float)cameraHeight * windowSizeRatio,(float)cameraHeight });
		spCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
		//Set zoom properties
		spCameraCom->cameraSize = spCameraCom->view.getSize();
		spCameraCom->zoomingBorders = zoomBorders;
		spCameraCom->zoomingSpeed = cameraZoomSpeed;
		spCameraCom->speedChange = velocityChange;
		spCameraCom->renderOutsideBoundsFor = outsideBordersMaxRenderDistance;
		spCameraCom->moveCamera = true;
		//Get movement com
		spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity };
	}



	//Creates a system camera
	//Worst case: O(4N+M) where N is number of components available in game and M number of components
	//available in game
	void InitializeSystemCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
	{
		int cameraHeight = 300;
		float cameraVelocity = 20.f;//14.f
		float cameraZoomSpeed = 15.f;
		float velocityChange = 8.f;
		sf::Vector2f zoomBorders = { 0.0001f, 8.f };
		float outsideBordersMaxRenderDistance = 15.f;

		//Create camera
		std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "SystemCamera").lock();
		//Add component
		//Get component
		std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
		std::shared_ptr<MovementComponent> spMovementCom = spCamera->AddComponent<MovementComponent>().lock();
		//set camera properties
		float windowSizeRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		//Set camera sizes
		spCameraCom->view.setSize(sf::Vector2f{ (float)cameraHeight * windowSizeRatio,(float)cameraHeight });
		spCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
		//Set zoom properties
		spCameraCom->cameraSize = spCameraCom->view.getSize();
		spCameraCom->zoomingBorders = zoomBorders;
		spCameraCom->zoomingSpeed = cameraZoomSpeed;
		spCameraCom->speedChange = velocityChange;
		spCameraCom->renderOutsideBoundsFor = outsideBordersMaxRenderDistance;
		spCameraCom->moveCamera = false;
		//Get movement com
		spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity };
	}



	//Creates planet overview camera
	void InitializePlanetCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
	{
		int cameraHeight = 1500;
		float cameraVelocity = 120.f;//14.f
		float cameraZoomSpeed = 20.f;
		float velocityChange = 8.f;
		sf::Vector2f zoomBorders = { 0.008f, 7.f };
		float outsideBordersMaxRenderDistance = 15.f;

		//Create camera
		std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "PlanetCamera").lock();
		//Add component
		//Get component
		std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
		std::shared_ptr<MovementComponent> spMovementCom = spCamera->AddComponent<MovementComponent>().lock();
		//set camera properties
		float windowSizeRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		//Set camera sizes
		spCameraCom->view.setSize(sf::Vector2f{ (float)cameraHeight * windowSizeRatio,(float)cameraHeight });
		spCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
		//Set zoom properties
		spCameraCom->cameraSize = spCameraCom->view.getSize();
		spCameraCom->zoomingBorders = zoomBorders;
		spCameraCom->zoomingSpeed = cameraZoomSpeed;
		spCameraCom->speedChange = velocityChange;
		spCameraCom->renderOutsideBoundsFor = outsideBordersMaxRenderDistance;
		spCameraCom->moveCamera = false;
		//Get movement com
		spMovementCom->velocity = sf::Vector2f{ cameraVelocity , cameraVelocity };
	}



	//Creates background camera
	//Worst case: O(2N+M) where N is number of components available in game and M number of components
	//available in game
	void InitializeBackgroundCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
	{
		//Create camera
		std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "BackgroundCamera").lock();
		//Add component
		//Get component
		std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
		//set camera properties
		spCameraCom->view.setSize(static_cast<sf::Vector2f>(windowSize));
		spCameraCom->view.setCenter(static_cast<sf::Vector2f>(windowSize) / 2.f);
	}

	//Worst case: O(12N+4M) where N is number of components available in game and M number of components
	//available in game
	void InitializeAllCameras(const sf::Vector2u& windowSize, std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
	{
		std::shared_ptr<SceneNode> spAllCam = sceneNode->FindChild("Cameras").lock();

		InitializeSpaceCamera(spAllCam, windowSize);
		InitializeUICamera(spAllCam, windowSize);
		InitializeBackgroundCamera(spAllCam, windowSize);
		InitializeSystemCamera(spAllCam, windowSize);
		InitializePlanetCamera(spAllCam, windowSize);
	}


	//Worst case: O(4N) where N is number of tiles to generate
	std::shared_ptr<TileMapComponent> GenerateBackgroundTiles(std::shared_ptr<Entity> spTileMap)
	{
		sf::Vector2i tilesInTileset{ 4,5 };
		sf::Vector2i tilesSize{ 64,64 };
		SpaceMapConfigurations& mapConfig = WorldGenerator::Instance().getSpaceMapConfig();

		//Get component
		std::shared_ptr<TileMapComponent> spTileMapCom = spTileMap->AddComponent<TileMapComponent>().lock();
		//set tilemap properties
		spTileMapCom->tileMap.tileSize = tilesSize;
		spTileMapCom->tileMap.marginSize = sf::Vector2i{ 0,0 };
		spTileMapCom->tileMap.paddingSize = sf::Vector2i{ 0,0 };
		spTileMapCom->tileMap.numTilesInTileset = tilesInTileset;
		spTileMapCom->tileMap.tilesTexturePath = "media/textures/SpaceBackground.png";
		spTileMapCom->tileMap.mapSize = mapConfig.backgroundSize;
		spTileMapCom->tileMap.loadTilesFromFile = false;
		spTileMapCom->tileMap.rotateTiles = true;
		//Iitialize all tiles
		spTileMapCom->tileMap.Initialize(WorldGenerator::Instance().GenerateGridOfTiles(mapConfig.backgroundSize, sf::Vector2i{ 0, (tilesInTileset.x * tilesInTileset.y) - 1 }), WorldGenerator::Instance().GenerateGridOfRandomNumbers(mapConfig.backgroundSize, sf::Vector2i{ 0, 3 }));
		spTileMap->SetPosition(sf::Vector2f{ (float)(tilesSize.x * mapConfig.backgroundSize.x / (-2.f)),(float)(tilesSize.y * mapConfig.backgroundSize.y / (-2.f)) });

		return spTileMapCom;
	}


	//Creates space objects
	//Worst case: O(2N+4M) where N is number of systems + stars to create and M number of tiles to create
	void CreateSpaceObjects(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
	{
		float additionalSpaceForCameraBoundaries = 30.f;
		std::weak_ptr<Entity> wpPlay = ECSGame::Instance().GetEntityManager().NewEntity("SpaceMap");
		std::shared_ptr<SceneNode> spNode = std::make_shared<SceneNode>(wpPlay);
		wpPlay.lock()->AddComponent<SystemPropertiesComponent>();

		std::weak_ptr<Entity> wpSysN = ECSGame::Instance().GetEntityManager().NewEntity("SystemNames");
		std::shared_ptr<SceneNode> spSysNamesNode = std::make_shared<SceneNode>(wpSysN);
		std::weak_ptr<Entity> wpNebN = ECSGame::Instance().GetEntityManager().NewEntity("NebulasNames");
		std::shared_ptr<SceneNode> spNebNamesNode = std::make_shared<SceneNode>(wpNebN);

		std::weak_ptr<Entity> wpBackgroundE = ECSGame::Instance().GetEntityManager().NewEntity("Background");
		std::shared_ptr<SceneNode> spBackgroundNode = std::make_shared<SceneNode>(wpBackgroundE);
		std::weak_ptr<Entity> wpNebul = ECSGame::Instance().GetEntityManager().NewEntity("Nebulas");
		spBackgroundNode->AddChild(std::make_shared<SceneNode>(wpNebul));

		//Reset world generator
		SpaceMapConfigurations& mapConfig = WorldGenerator::Instance().getSpaceMapConfig();

		//Firstly generate background
		std::shared_ptr<TileMapComponent> spTileMapCom = GenerateBackgroundTiles(wpBackgroundE.lock());
		//Secondly generate nebulas
		WorldGenerator::Instance().GenerateNebulas(spBackgroundNode->FindChild("Nebulas").lock(), spNebNamesNode);
		//Thirdly generate systems and stars in it
		WorldGenerator::Instance().GenerateSpaceMap(spNode);
		//After put rectangleShape components for all objects
		TextureAndNameSetter txSetter(WorldGenerator::Instance().getSeed(), spSysNamesNode);
		txSetter.wpSpaceMapNode = spNode;
		spNode->AcceptVisitor(txSetter);

		std::weak_ptr<SceneNode> wCamNode = sceneNode->FindChild("Cameras");

		//Lastly set camera boundaries
		std::shared_ptr<CameraComponent> spCameraCom = wCamNode.lock()->FindChild("SpaceCamera").lock()->GetEntity().lock()->FindComponent<CameraComponent>().lock();
		sf::Vector2f mapSize = static_cast<sf::Vector2f>(spTileMapCom->tileMap.getMapSize());
		spCameraCom->horizontalBorders = { mapConfig.horizontalPosBoundaries.x - additionalSpaceForCameraBoundaries, mapConfig.horizontalPosBoundaries.y + additionalSpaceForCameraBoundaries };
		spCameraCom->verticalBorders = { mapConfig.verticalPosBoundaries.x - additionalSpaceForCameraBoundaries, mapConfig.verticalPosBoundaries.y + additionalSpaceForCameraBoundaries };
		spCameraCom->view.setCenter(sf::Vector2f{ 0.f, 0.f });

		std::shared_ptr<CameraComponent> spCameraCom2 = wCamNode.lock()->FindChild("SystemCamera").lock()->GetEntity().lock()->FindComponent<CameraComponent>().lock();
		spCameraCom2->horizontalBorders = { -mapConfig.afarStarsBoundaries.y * 2.f, mapConfig.afarStarsBoundaries.y * 2.f };
		spCameraCom2->verticalBorders = { -mapConfig.afarStarsBoundaries.y * 2.f, mapConfig.afarStarsBoundaries.y * 2.f };
		spCameraCom2->view.setCenter(sf::Vector2f{ 0.f, 0.f });

		std::shared_ptr<CameraComponent> spCameraCom3 = wCamNode.lock()->FindChild("PlanetCamera").lock()->GetEntity().lock()->FindComponent<CameraComponent>().lock();
		spCameraCom3->horizontalBorders = { -mapConfig.planetCameraMaxBoundary, mapConfig.planetCameraMaxBoundary };
		spCameraCom3->verticalBorders = { -mapConfig.planetCameraMaxBoundary, mapConfig.planetCameraMaxBoundary };
		spCameraCom3->view.setCenter(sf::Vector2f{ 0.f, 0.f });

#ifdef OUTPUT_WORLD_STATISTICS
		SceneNodeSpaceObjectsCounter visitor(mapConfig);
		ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);
		visitor.OutputAllData();
#endif

#ifdef OUTPUT_WORLD_MEMORY_USAGE
		SceneNodeSpaceObjectsMemorySize visitor;
		ECSGame::Instance().GetSceneRoot()->AcceptVisitor(visitor);
		visitor.OutputAllData();
#endif

		sceneNode->AddChild(spNode);
		sceneNode->AddChild(spBackgroundNode);
		uiNode->AddChild(spSysNamesNode);
		uiNode->AddChild(spNebNamesNode);

		sceneNode->ChangeChildOrder(wpBackgroundE.lock(), 0);
		uiNode->ChangeChildOrder(wpSysN.lock(), 0);
		uiNode->ChangeChildOrder(wpNebN.lock(), 1);

		uiNode->FindChild("LoadingScreen").lock()->GetEntity().lock()->hidden = true;
		std::shared_ptr<sf::Text> spText = uiNode->FindChild("EscapeMenuScreen").lock()->FindChild("SeedText").lock()->GetEntity().lock()->FindComponent<TextComponent>().lock()->text;
		spText->setString("Seed: " + std::to_string(WorldGenerator::Instance().getSeed()));
		gel::CentreText(*spText, sf::Vector2f{ 0.f, 0.f });

		WorldGenerator::Instance().SetWorldIsGenerated();
		ECSGame::Instance().SetGameState(GameState::Resumed);
		ECSGame::Instance().SetDeltaTimeMultiplier(1.f);
		signals::onChangeInputType(InputType::World);
	}


	//Worst case: O(3N+2M) where N is number of components in entity and M number of components
	//available in game
	void InitializeMouseIcon(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
	{
		sf::Vector2f mouseSize{ 60.f, 60.f };
		float uiSize = ECSGame::Instance().GetUISize();

		std::weak_ptr<Entity> wpMouseIcon = ECSGame::Instance().GetEntityManager().NewEntity("MouseIcon");
		uiNode->AddChild(std::make_shared<SceneNode>(wpMouseIcon));
		//Add components
		//wpMouseIcon.lock()->AddComponent<UIPartComponent>();
		std::shared_ptr<RectangleShapeComponent> spRectShape = wpMouseIcon.lock()->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShape, mouseSize * uiSize, "MouseIcon");
		spRectShape->shape.setPosition({ 32,32 });

		sf::Vector2i pos = ECSGame::Instance().GetMousePosition();
		wpMouseIcon.lock()->SetPosition(sf::Vector2f{ (float)pos.x,(float)pos.y });
	}

	//Creates UI of the game
	//Worst case: O(4N+M) where N is number of components in entity and M number of components
	//available in game
	void CreateUI(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
	{
		sf::Vector2f iconSize{ 100.f, 100.f };
		sf::Vector2f uiTopPartSize{ 1000.f, 140.f };
		sf::Vector2f uiBottomPartSize{ 1200.f, 200.f };
		sf::Vector2f uiInfoPartSize{ 800.f, 500.f };
		sf::Vector2f planetDisPartSize{ 1650.f, 900.f };
		sf::Vector2f escapeMenuSize{ 2560.f, 1600.f };
		sf::Vector2f buttonSize{ 600.f, 60.f };
		sf::Vector2f playerButtonSize{ 80.f, 30.f };
		float dateFontSize = 32;
		float simulationFontSize = 25;
		float metricsFontSize = 22;
		float mainFontSize = 40;
		float mainMenuMainFontSize = 90;
		float infoFontSize = 21;
		std::string fontName = "PixelBold";
		sf::Color importantColor = sf::Color{ 235, 175, 38 };
		sf::Color usualColor = sf::Color{ 255,255,255 };
		sf::Color escapeMenuPanelColor = sf::Color{ 0,0,0,230 };

		float uiSize = ECSGame::Instance().GetUISize();
		//CREATE SELECTION ICON
		std::shared_ptr<Entity> spSSIcon = CreateNewEntityAt(uiNode, "SelectedSystemIcon").lock();
		//Add component
		//spSSIcon->AddComponent<UIPartComponent>();
		spSSIcon->AddComponent<UIFollowerComponent>();
		std::shared_ptr<RectangleShapeComponent> spRectShape = spSSIcon->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShape, iconSize * uiSize, "SelectionIcon");
		spSSIcon->hidden = true;

		//CREATE Upper and lower parts of ui
		std::shared_ptr<Entity> spToPart = CreateNewEntityAt(uiNode, "UpperPart").lock();
		//Add component
		//spToPart->AddComponent<UIPartComponent>();
		std::shared_ptr<RectangleShapeComponent> spRectShape2 = spToPart->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShape2, uiTopPartSize * uiSize, "TopUIPart");
		spToPart->SetPosition(sf::Vector2f{ 1280.f,uiTopPartSize.y / 2.f } * uiSize);

		std::shared_ptr<Entity> spLoPart = CreateNewEntityAt(uiNode, "LowerPart").lock();
		//Add component
		//spLoPart->AddComponent<UIPartComponent>();
		std::shared_ptr<RectangleShapeComponent> spRectShape3 = spLoPart->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShape3, uiBottomPartSize * uiSize, "BottomUIPart");
		spLoPart->SetPosition(sf::Vector2f{ 1280.f,1600.f - (uiBottomPartSize.y / 2.f) } * uiSize);

		std::shared_ptr<SceneNode> spLowerPartNode = uiNode->FindChild(*spLoPart).lock();
		std::shared_ptr<SceneNode> spUpperPartNode = uiNode->FindChild(*spToPart).lock();
		//CREATE UI textes
		std::shared_ptr<Entity> spTextEn = InitializeText("MonthText", " ", (int)(dateFontSize * uiSize), sf::Vector2f{ 140.f, 0.f } * uiSize, fontName, true, importantColor, spLowerPartNode);
		std::shared_ptr<TextComponent> spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateMonthText;
		spTextCom->textAlignment = TextAlignment::Center;
		
		spTextEn = InitializeText("DayText", " ", (int)(dateFontSize * uiSize), sf::Vector2f{ -110.f, 0.f } * uiSize, fontName, true, importantColor, spLowerPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateDayText;
		spTextCom->textAlignment = TextAlignment::Center;
		
		spTextEn = InitializeText("YearText", " ", (int)(dateFontSize * uiSize), sf::Vector2f{ 380.f, 0.f } * uiSize, fontName, true, importantColor, spLowerPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateYearText;
		spTextCom->textAlignment = TextAlignment::Center;
		
		spTextEn = InitializeText("SimulationStateText", " ", (int)(dateFontSize * uiSize), sf::Vector2f{ -300.f, 0.f } * uiSize, fontName, true, usualColor, spLowerPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateSimulationStateText;
		spTextCom->textAlignment = TextAlignment::Center;
		
		spTextEn = InitializeText("SimulationSpeedText", " ", (int)(simulationFontSize * uiSize), sf::Vector2f{ 0.f, -60.f } * uiSize, fontName, true, usualColor, spLowerPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateSimulationSpeedText;
		spTextCom->textAlignment = TextAlignment::Center;
		
		spTextEn = InitializeText("ViewSizeText", " ", (int)(metricsFontSize * uiSize), sf::Vector2f{ 0.f, 30.f } * uiSize, fontName, true, usualColor, spUpperPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateViewSizeText;
		spTextCom->textAlignment = TextAlignment::Center;
		
		spTextEn = InitializeText("OverviewText", " ", (int)(mainFontSize * uiSize), sf::Vector2f{ 0.f, -20.f } * uiSize, fontName, true, importantColor, spUpperPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateOverviewText;
		spTextCom->textAlignment = TextAlignment::Center;

		//CREATE SIDE part of ui
		std::shared_ptr<Entity> spInfoPart = CreateNewEntityAt(uiNode, "InfoPart").lock();
		spInfoPart->hidden = true;
		//Add component
		//spInfoPart->AddComponent<UIPartComponent>();
		std::shared_ptr<RectangleShapeComponent> spRectShape4 = spInfoPart->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShape4, uiInfoPartSize * uiSize, "UIPartSide");
		spInfoPart->SetPosition(sf::Vector2f{ 390.f, 800.f } * uiSize);

		std::shared_ptr<SceneNode> spInfoPartNode = uiNode->FindChild(*spInfoPart).lock();
		//CREATE INFO textes
		spTextEn = InitializeText("InfoText0", " ", (int)(infoFontSize * uiSize), sf::Vector2f{ -370.f, -180.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateInfoText0;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("InfoText1", " ", (int)(infoFontSize * uiSize), sf::Vector2f{ -370.f, -120.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateInfoText1;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("InfoText2", " ", (int)(infoFontSize * uiSize), sf::Vector2f{ -370.f,-60.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateInfoText2;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("InfoText3", " ", (int)(infoFontSize * uiSize), sf::Vector2f{ -370.f, 0.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateInfoText3;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("InfoText4", " ", (int)(infoFontSize * uiSize), sf::Vector2f{ -370.f, 60.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateInfoText4;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("InfoText5", " ", (int)(infoFontSize * uiSize), sf::Vector2f{ -370.f, 120.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateInfoText5;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("InfoText6", " ", (int)(infoFontSize * uiSize), sf::Vector2f{ -370.f, 180.f } * uiSize, fontName, true, usualColor, spInfoPartNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateInfoText6;
		spTextCom->textAlignment = TextAlignment::Left;

		//CREATE PLANET DISTRICTS panel
		std::shared_ptr<Entity> spPlDisPart = CreateNewEntityAt(uiNode, "PlanetDistrictsPart").lock();
		spPlDisPart->hidden = true;
		//Add component
		//spPlDisPart->AddComponent<UIPartComponent>();
		std::shared_ptr<RectangleShapeComponent> spRectShape5 = spPlDisPart->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShape5, planetDisPartSize * uiSize, "UIPartPlanetDistricts");
		spPlDisPart->SetPosition(sf::Vector2f{ 1280.f, 800.f } * uiSize);

		std::shared_ptr<SceneNode> spPlDisNode = uiNode->FindChild(*spPlDisPart).lock();
		//CREATE Planet Districts textes
		spTextEn = InitializeText("PlanetNameText", " ", (int)(dateFontSize * uiSize), sf::Vector2f{ 0.f, -380.f } * uiSize, fontName, true, importantColor, spPlDisNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdatePlanetNameDistrictText;
		spTextCom->textAlignment = TextAlignment::Center;
		
		spTextEn = InitializeText("DistrictsText", "Planet Districts:", (int)(infoFontSize * uiSize), sf::Vector2f{ -700.f, -300.f } * uiSize, fontName, false, usualColor, spPlDisNode);
		gel::AlignTextToLeftSide(*spTextEn->FindComponent<TextComponent>().lock()->text, sf::Vector2f { 0.f, 0.f });
		
		spTextEn = InitializeText("DistrictTypeText", "Type: ", (int)(infoFontSize * uiSize), sf::Vector2f{ -700.f, 310.f } * uiSize, fontName, false, usualColor, spPlDisNode);
		gel::AlignTextToLeftSide(*spTextEn->FindComponent<TextComponent>().lock()->text, sf::Vector2f { 0.f, 0.f });
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateDistrictTypeText;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("DistrictPopulationText", "Population ", (int)(infoFontSize * uiSize), sf::Vector2f{ -700.f, 340.f } * uiSize, fontName, false, usualColor, spPlDisNode);
		gel::AlignTextToLeftSide(*spTextEn->FindComponent<TextComponent>().lock()->text, sf::Vector2f { 0.f, 0.f });
		spTextEn = InitializeText("DistrictIndustryText", "Industry ", (int)(infoFontSize * uiSize), sf::Vector2f{ -700.f, 370.f } * uiSize, fontName, false, usualColor, spPlDisNode);
		gel::AlignTextToLeftSide(*spTextEn->FindComponent<TextComponent>().lock()->text, sf::Vector2f { 0.f, 0.f });
		spTextEn = InitializeText("CountriesText", "Countries list:", (int)(infoFontSize * uiSize), sf::Vector2f{ 200.f, -300.f } * uiSize, fontName, true, usualColor, spPlDisNode);
		spTextEn = InitializeText("BuildingsText", "Buildings list:", (int)(infoFontSize * uiSize), sf::Vector2f{ 600.f, -300.f } * uiSize, fontName, true, usualColor, spPlDisNode);

		//CREATE MUSIC PLAYER panel
		std::shared_ptr<Entity> spMusPart = CreateNewEntityAt(uiNode, "MusicPlayerPart").lock();
		//Add component
		//spPlDisPart->AddComponent<UIPartComponent>();
		spRectShape = spMusPart->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShape, uiInfoPartSize * uiSize, "UIPartSide");
		spRectShape->shape.rotate(sf::Angle(sf::degrees(180)));
		spMusPart->SetPosition(sf::Vector2f{ 2450.f, -130.f } * uiSize);

		std::shared_ptr<SceneNode> spMusNode = uiNode->FindChild(*spMusPart).lock();
		//CREATE MUSIC PLAYER text
		spTextEn = InitializeText("MusicPlayerText", "Music Player", (int)(metricsFontSize * uiSize), sf::Vector2f{ -40.f, 150.f } * uiSize, fontName, true, usualColor, spMusNode);

		//CREATE SELECT PREVIOUS MUSIC button
		std::shared_ptr<Entity> spButton = CreateNewEntityAt(spMusNode, "PreviousButton").lock();
		spButton->SetPosition(sf::Vector2f{ -100.f, 200.f } * uiSize);
		spButton->hidden = false;

		std::shared_ptr<RectangleShapeComponent> spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Slower1Button");

		std::shared_ptr<ButtonComponent> spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Slower1Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Slower1HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Slower1PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Slower1PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnPreviousMusicButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE STOP MUSIC button
		spButton = CreateNewEntityAt(spMusNode, "StopButton").lock();
		std::shared_ptr<Entity> spButRes = CreateNewEntityAt(spMusNode, "ResumeButton").lock();
		spButton->SetPosition(sf::Vector2f{ -50.f, 200.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "PlayingButton");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("PlayingButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("PlayingHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("PlayingPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("PlayingPressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [spButton, spButRes](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnStopMusicButtonPressed(entity); 
			spButton->hidden = true;
			spButRes->hidden = false; };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE RESUME MUSIC button
		spButRes->SetPosition(sf::Vector2f{ -50.f, 200.f } * uiSize);
		spButRes->hidden = true;

		spRectShapeCom = spButRes->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "StoppedButton");

		spButtonCom = spButRes->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("StoppedButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("StoppedHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("StoppedPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("StoppedPressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [spButton, spButRes](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnResumeMusicButtonPressed(entity); 
			spButton->hidden = false;
			spButRes->hidden = true; };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SELECT NEXT MUSIC button
		spButton = CreateNewEntityAt(spMusNode, "NextButton").lock();
		spButton->SetPosition(sf::Vector2f{ 0.f, 200.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Faster1Button");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Faster1Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Faster1HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Faster1PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Faster1PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnNextMusicButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE MIX MUSIC LIST button
		spButton = CreateNewEntityAt(spMusNode, "MixButton").lock();
		spButton->SetPosition(sf::Vector2f{ 50.f, 200.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "MixButton");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("MixButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("MixHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("MixPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("MixPressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnMixMusicButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE Loading screen
		std::shared_ptr<Entity> spLoadScreen = CreateNewEntityAt(uiNode, "LoadingScreen").lock();
		spLoadScreen->hidden = false;
		//Add component
		spRectShape = spLoadScreen->AddComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setSize(escapeMenuSize * uiSize);
		spRectShape->shape.setOrigin(spRectShape->shape.getSize() / 2.f);
		spRectShape->shape.setFillColor(escapeMenuPanelColor);
		spLoadScreen->SetPosition(sf::Vector2f{ 1280.f, 800.f } * uiSize);

		std::shared_ptr<SceneNode> spLoadSNode = uiNode->FindChild(*spLoadScreen).lock();
		//CREATE Loading screen textes
		spTextEn = InitializeText("LoadingText", "Generating...", (int)(mainFontSize * uiSize), sf::Vector2f{ 0.f, 0.f } * uiSize, fontName, true, importantColor, spLoadSNode);

		//CREATE escape menu screen
		std::shared_ptr<Entity> spEscScreen = CreateNewEntityAt(uiNode, "EscapeMenuScreen").lock();
		spEscScreen->hidden = true;
		//Add component
		spRectShape = spEscScreen->AddComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setSize(escapeMenuSize * uiSize);
		spRectShape->shape.setOrigin(spRectShape->shape.getSize() / 2.f);
		spRectShape->shape.setFillColor(escapeMenuPanelColor);
		spEscScreen->SetPosition(sf::Vector2f{ 1280.f, 800.f } * uiSize);

		std::shared_ptr<SceneNode> spEscapeNode = uiNode->FindChild(*spEscScreen).lock();
		//CREATE escape menu textes
		spTextEn = InitializeText("EscapeText", "Game paused", (int)(mainMenuMainFontSize * uiSize), sf::Vector2f{ 0.f, -160.f } * uiSize, fontName, true, importantColor, spEscapeNode);
		spTextEn = InitializeText("SeedText", "Seed: ", (int)(infoFontSize * uiSize), sf::Vector2f{ 0.f, 750.f } * uiSize, fontName, true, usualColor, spEscapeNode);

		//CREATE RESUME button
		spButton = CreateNewEntityAt(spEscapeNode, "ResumeButton").lock();
		spButton->SetPosition(sf::Vector2f{ 0.f,-20.f } * uiSize);

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, buttonSize * uiSize, "ResumeButton");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ buttonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("ResumeButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("ResumeHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("ResumeHoveredPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("ResumePressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnResumeButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE EXIT GAME button
		std::shared_ptr<Entity> spButton2 = CreateNewEntityAt(spEscapeNode, "ExitGameButton").lock();
		spButton2->SetPosition(sf::Vector2f{ 0.f,140.f } * uiSize);

		spRectShapeCom = spButton2->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, buttonSize * uiSize, "ExitGameButton");

		spButtonCom = spButton2->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ buttonSize.x * 0.65f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("ExitGameButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("ExitGameHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("ExitGameHoveredPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("ExitGamePressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnExitButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };


		//CREATE EXIT TO MAIN MENU button
		spButton2 = CreateNewEntityAt(spEscapeNode, "ExitToMainMenuButton").lock();
		spButton2->SetPosition(sf::Vector2f{ 0.f,60.f } * uiSize);

		spRectShapeCom = spButton2->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, buttonSize * uiSize, "ExitToMainMenuButton");

		spButtonCom = spButton2->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ buttonSize.x * 0.65f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("ExitToMainMenuButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("ExitToMainMenuHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("ExitToMainMenuHoveredPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("ExitToMainMenuPressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnExitToMainMenuButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };


		//CREATE SIMULATION PLAYING button
		spButton = CreateNewEntityAt(spLowerPartNode, "PlayingButton").lock();
		spButton->SetPosition(sf::Vector2f{ 0.f,50.f } * uiSize);
		spButton->hidden = true;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "PlayingButton");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("PlayingButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("PlayingHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("PlayingPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("PlayingPressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnPlayingButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SIMULATION STOPPED button
		spButton = CreateNewEntityAt(spLowerPartNode, "StoppedButton").lock();
		spButton->SetPosition(sf::Vector2f{ 0.f,50.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "StoppedButton");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("StoppedButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("StoppedHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("StoppedPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("StoppedPressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnStoppedButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SIMULATION SLOWER 3 button
		spButton = CreateNewEntityAt(spLowerPartNode, "Slower3Button").lock();
		spButton->SetPosition(sf::Vector2f{ -240.f,50.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Slower3Button");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Slower3Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Slower3HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Slower3PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Slower3PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnSlower3ButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SIMULATION SLOWER 2 button
		spButton = CreateNewEntityAt(spLowerPartNode, "Slower2Button").lock();
		spButton->SetPosition(sf::Vector2f{ -130.f,50.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Slower2Button");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.8f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Slower2Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Slower2HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Slower2PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Slower2PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnSlower2ButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SIMULATION SLOWER 1 button
		spButton = CreateNewEntityAt(spLowerPartNode, "Slower1Button").lock();
		spButton->SetPosition(sf::Vector2f{ -60.f,50.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Slower1Button");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Slower1Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Slower1HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Slower1PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Slower1PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnSlower1ButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SIMULATION FASTER 3 button
		spButton = CreateNewEntityAt(spLowerPartNode, "Faster3Button").lock();
		spButton->SetPosition(sf::Vector2f{ 240.f,50.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Faster3Button");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Faster3Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Faster3HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Faster3PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Faster3PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnFaster3ButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SIMULATION FASTER 2 button
		spButton = CreateNewEntityAt(spLowerPartNode, "Faster2Button").lock();
		spButton->SetPosition(sf::Vector2f{ 130.f,50.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Faster2Button");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.8f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Faster2Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Faster2HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Faster2PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Faster2PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnFaster2ButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE SIMULATION FASTER 1 button
		spButton = CreateNewEntityAt(spLowerPartNode, "Faster1Button").lock();
		spButton->SetPosition(sf::Vector2f{ 60.f,50.f } * uiSize);
		spButton->hidden = false;

		spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, playerButtonSize * uiSize, "Faster1Button");

		spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ playerButtonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("Faster1Button", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("Faster1HoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("Faster1PressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("Faster1PressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnFaster1ButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };
	}


	//Create debugging text at the top right corner
	//Worst case: O((6N+3M)*K) where N is number of components in entity and M number of components
	//available in game and K number of texts to create
	void CreateDebugText(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
	{
		float fontSize = 20.f;
		std::string fontName = "PixelBold";

		std::weak_ptr<Entity> wpObjOrb = ECSGame::Instance().GetEntityManager().NewEntity("DebugPanel");
		wpObjOrb.lock()->hidden = true;
		std::shared_ptr<SceneNode> spDebugNode = std::make_shared<SceneNode>(wpObjOrb);
		uiNode->AddChild(spDebugNode);

		float uiSize = ECSGame::Instance().GetUISize();
		std::shared_ptr<Entity> spTextEn = InitializeText("MouseCoordsText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 13.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		std::shared_ptr<TextComponent> spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateMousePositionDebugText;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("WorldCoordsText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 38.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateWorldPositionDebugText;
		spTextCom->textAlignment = TextAlignment::Left;

		//spTextEn = InitializeText("SystemsNearByText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 50.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		//spTextCom = spTextEn->FindComponent<TextComponent>().lock();

		spTextEn = InitializeText("FPSText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 63.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateFPSDebugText;
		spTextCom->textAlignment = TextAlignment::Left;

		spTextEn = InitializeText("DaysPastText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 88.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateDaysPastDebugText;
		spTextCom->textAlignment = TextAlignment::Left;

		spTextEn = InitializeText("DateText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 113.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateDateDebugText;
		spTextCom->textAlignment = TextAlignment::Left;

		spTextEn = InitializeText("RenderText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 138.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateNodesDebugText;
		spTextCom->textAlignment = TextAlignment::Left;
		
		spTextEn = InitializeText("MouseOverUIText", " ", (int)(fontSize * uiSize), sf::Vector2f{ 0.f, 163.f } * uiSize, fontName, false, sf::Color::White, spDebugNode);
		spTextCom = spTextEn->FindComponent<TextComponent>().lock();
		spTextCom->updateText = TextUpdateFunctions::UpdateMouseOverUIDebugText;
		spTextCom->textAlignment = TextAlignment::Left;
	}
}

//Initialize SpaceWorldScene
void InitializeSpaceWorldScene(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode) 
{
	sceneNode->GetParent().lock()->GetEntity().lock()->AddComponent<SpaceSceneStatesComponent>();

	//Create nodes, in which i will sort new entities which will be created during the game
	std::weak_ptr<Entity> wpCameras = ECSGame::Instance().GetEntityManager().NewEntity("Cameras");
	sceneNode->AddChild(std::make_shared<SceneNode>(wpCameras));

	std::weak_ptr<Entity> wpObjOrb = ECSGame::Instance().GetEntityManager().NewEntity("ObjectOrbits");
	uiNode->AddChild(std::make_shared<SceneNode>(wpObjOrb));

	std::weak_ptr<Entity> wpSysIc = ECSGame::Instance().GetEntityManager().NewEntity("SystemIcons");
	uiNode->AddChild(std::make_shared<SceneNode>(wpSysIc));

	//Initialize all cameras
	SpaceWorldScene::InitializeAllCameras(ECSGame::Instance().GetWindowSize(), sceneNode, uiNode);
	//Setup mouseIcon
	SpaceWorldScene::InitializeMouseIcon(sceneNode, uiNode);
	//Initialize game ui
	SpaceWorldScene::CreateUI(sceneNode, uiNode);
	//Create debug text
	SpaceWorldScene::CreateDebugText(sceneNode, uiNode);

	uiNode->ChangeChildOrder(uiNode->FindChild("MouseIcon").lock(), (int)uiNode->GetAllChildren().size() - 1);

	//Generate world in a separate thread
	std::thread generateWorldAsync(SpaceWorldScene::CreateSpaceObjects, sceneNode, uiNode);
	generateWorldAsync.detach();
}