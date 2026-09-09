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


namespace MainMenuScene
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


	//Creates background camera
	//Worst case: O(2N+M) where N is number of components available in game and M number of components
	//available in game
	void InitializeBackgroundCamera(std::shared_ptr<SceneNode> spCameraNode, const sf::Vector2u& windowSize)
	{
		int cameraHeight = 300;

		//Create camera
		std::shared_ptr<Entity> spCamera = CreateNewEntityAt(spCameraNode, "BackgroundCamera").lock();
		//Add component
		//Get component
		std::shared_ptr<CameraComponent> spCameraCom = spCamera->AddComponent<CameraComponent>().lock();
		//set camera properties
		float windowSizeRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		spCameraCom->view.setSize(sf::Vector2f{ (float)cameraHeight * windowSizeRatio,(float)cameraHeight });
		spCameraCom->view.setCenter(sf::Vector2f{ 0.f,0.f });
	}



	//Worst case: O(12N+4M) where N is number of components available in game and M number of components
	//available in game
	void InitializeAllCameras(const sf::Vector2u& windowSize, std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
	{
		std::shared_ptr<SceneNode> spAllCam = sceneNode->FindChild("Cameras").lock();

		InitializeUICamera(spAllCam, windowSize);
		InitializeBackgroundCamera(spAllCam, windowSize);
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
		wpMouseIcon.lock()->SetPosition(sf::Vector2f{(float)pos.x,(float)pos.y});
	}


	void CreateUI(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
	{
		float mainMenuMainFontSize = 90;
		float usualFontSize = 30;
		float inputBoxFontSize = 28;
		sf::Vector2f buttonSize{ 600.f, 60.f };
		sf::Vector2f menuSize{ 2560.f, 1600.f };
		sf::Vector2f inputBoxSize{ 500.f, 40.f };
		sf::Color mainMenuPanelColor = sf::Color{ 0,0,0,100 };
		sf::Color inputBoxColor = sf::Color{ 100,100,100,140 };
		std::string fontName = "PixelBold";
		std::shared_ptr<sf::Font> fontPtr = ResourceManager::Instance().GetFont(fontName).lock();
		sf::Color importantColor = sf::Color{ 235, 175, 38 };
		sf::Color usualColor = sf::Color{ 255, 255, 255 };
		sf::Color errorColor = sf::Color{ 240, 94, 78 };

		float uiSize = ECSGame::Instance().GetUISize();

		//CREATE main menu screen
		std::shared_ptr<Entity> spEscScreen = CreateNewEntityAt(uiNode, "MainMenuScreen").lock();
		spEscScreen->hidden = false;
		//Add component
		std::shared_ptr<RectangleShapeComponent> spRectShape = spEscScreen->AddComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setSize(menuSize * uiSize);
		spRectShape->shape.setOrigin(spRectShape->shape.getSize() / 2.f);
		spRectShape->shape.setFillColor(mainMenuPanelColor);
		spEscScreen->SetPosition(sf::Vector2f{ 1280.f, 800.f } * uiSize);

		std::shared_ptr<SceneNode> spEscapeNode = uiNode->FindChild(*spEscScreen).lock();
		//CREATE escape menu textes
		std::shared_ptr<Entity> spTextEn = InitializeText("MainText", "Simple Stellaris", (int)(mainMenuMainFontSize * uiSize), sf::Vector2f{ 0.f, -160.f } * uiSize, fontName, true, importantColor, spEscapeNode);

		//CREATE START GAME button
		std::shared_ptr<Entity> spButton = CreateNewEntityAt(spEscapeNode, "StartGameButton").lock();
		spButton->SetPosition(sf::Vector2f{ 0.f,-20.f } * uiSize);

		sf::Vector2f mousePos = spEscapeNode->FindChild("StartGameButton").lock()->GetCombinedPosition();
		ECSGame::Instance().SetMousePosition(sf::Vector2i{ (int)mousePos.x,(int)mousePos.y });

		std::shared_ptr<RectangleShapeComponent> spRectShapeCom = spButton->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, buttonSize * uiSize, "StartGameButton");

		std::shared_ptr<ButtonComponent> spButtonCom = spButton->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ buttonSize.x * 0.5f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("StartGameButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("StartGameHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("StartGameHoveredPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("StartGamePressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnStartGameButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE EXIT GAME button
		std::shared_ptr<Entity> spButton2 = CreateNewEntityAt(spEscapeNode, "ExitGameButton").lock();
		spButton2->SetPosition(sf::Vector2f{ 0.f,60.f } * uiSize);

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

		//CREATE generation config screen
		std::shared_ptr<Entity> spGenScreen = CreateNewEntityAt(uiNode, "GenerationConfigScreen").lock();
		spGenScreen->hidden = true;
		//Add component
		spRectShape = spGenScreen->AddComponent<RectangleShapeComponent>().lock();
		spRectShape->shape.setSize(menuSize * uiSize);
		spRectShape->shape.setOrigin(spRectShape->shape.getSize() / 2.f);
		spRectShape->shape.setFillColor(mainMenuPanelColor);
		spGenScreen->SetPosition(sf::Vector2f{ 1280.f, 800.f } * uiSize);

		std::shared_ptr<SceneNode> spGenNode = uiNode->FindChild(*spGenScreen).lock();
		//CREATE seed text
		spTextEn = InitializeText("SeedText", "Seed: ", (int)(usualFontSize * uiSize), sf::Vector2f{ -140.f, 0.f } * uiSize, fontName, true, usualColor, spGenNode);
		//CREATE error text
		spTextEn = InitializeText("ErrorText", " ", (int)(usualFontSize * uiSize), sf::Vector2f{ -140.f, 50.f } * uiSize, fontName, true, errorColor, spGenNode);

		//CREATE INPUT BOX
		std::shared_ptr<Entity> spInputBox = CreateNewEntityAt(spGenNode, "SeedInputBox").lock();
		spInputBox->SetPosition(sf::Vector2f{ 200.f,0.f } * uiSize);
		std::shared_ptr<SceneNode> spInBoxNode = spGenNode->FindChild(*spInputBox).lock();

		//rectShape
		spRectShapeCom = spInputBox->AddComponent<RectangleShapeComponent>().lock();
		spRectShapeCom->shape.setSize(inputBoxSize* uiSize);
		spRectShapeCom->shape.setOrigin(spRectShapeCom->shape.getSize() / 2.f);
		spRectShapeCom->shape.setFillColor(inputBoxColor);

		//button
		spButtonCom = spInputBox->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = inputBoxSize * uiSize;

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnInputBoxHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnInputBoxUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnInputBoxPressed(entity); };

		//inputBox
		std::shared_ptr<InputBoxComponent> spInputBoxCom = spInputBox->AddComponent<InputBoxComponent>().lock();
		spInputBoxCom->text = std::to_string(WorldGenerator::Instance().getSeed());
		spInputBoxCom->cursorPosition = (int)spInputBoxCom->text.size();
		spInputBoxCom->acceptOnlyDigits = true;
		spInputBoxCom->maxLength = 11;

		//text
		std::shared_ptr<Entity> spInputBoxText = CreateNewEntityAt(spInBoxNode, "InputBoxText").lock();
		spInputBoxText->SetPosition(sf::Vector2f{ -inputBoxSize.x / 2.f, 0.f });
		std::shared_ptr<TextComponent> spTextCom = spInputBoxText->AddComponent<TextComponent>().lock();
		spTextCom->text = std::make_shared<sf::Text>(*fontPtr);
		spTextCom->text->setCharacterSize(inputBoxFontSize);
		spTextCom->textAlignment = TextAlignment::Left;
		spTextCom->updateText = [spInputBoxCom](std::shared_ptr<sf::Text> spText) 
			{
				if (spInputBoxCom->focused)
				{
					std::string textToShow = spInputBoxCom->text;
					if (spInputBoxCom->showLine)
					{
						//std::cout << "show line\n";
						textToShow.insert(spInputBoxCom->cursorPosition, 1, '|');
						spText->setString(textToShow);
					}
					else
					{
						//std::cout << "DO NOT show\n";
						textToShow.insert(spInputBoxCom->cursorPosition, 1, '.');
						spText->setString(textToShow);
					}
				}
				else
					spText->setString(spInputBoxCom->text);
			};

		//CREATE BACK TO MENU button
		std::shared_ptr<Entity> spButtonBack = CreateNewEntityAt(spGenNode, "BackToMainMenuButton").lock();
		spButtonBack->SetPosition(sf::Vector2f{ -1000.f,750.f }* uiSize);

		spRectShapeCom = spButtonBack->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, buttonSize* uiSize, "ExitToMainMenuButton");

		spButtonCom = spButtonBack->AddComponent<ButtonComponent>().lock();
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
			{ ButtonSignals::OnBackToMainMenuButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };

		//CREATE CREATE WORLD button
		std::shared_ptr<Entity> spButtonCW = CreateNewEntityAt(spGenNode, "CreateWorldButton").lock();
		spButtonCW->SetPosition(sf::Vector2f{ 1000.f,750.f }* uiSize);

		spRectShapeCom = spButtonCW->AddComponent<RectangleShapeComponent>().lock();
		SetupRectangleShape(spRectShapeCom, buttonSize* uiSize, "CreateWorldButton");

		spButtonCom = spButtonCW->AddComponent<ButtonComponent>().lock();
		spButtonCom->buttonSize = sf::Vector2{ buttonSize.x * 0.8f,buttonSize.y } * uiSize;

		spButtonCom->unhoveredTexture = ResourceManager::Instance().GetTexture("CreateWorldButton", spButtonCom->unhoveredIntRect).lock();
		spButtonCom->hoveredTexture = ResourceManager::Instance().GetTexture("CreateWorldHoveredButton", spButtonCom->hoveredIntRect).lock();
		spButtonCom->hoveredPressedTexture = ResourceManager::Instance().GetTexture("CreateWorldHoveredPressedButton", spButtonCom->hoveredPressedIntRect).lock();
		spButtonCom->pressedTexture = ResourceManager::Instance().GetTexture("CreateWorldPressedButton", spButtonCom->pressedIntRect).lock();

		spButtonCom->onButtonHovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonHovered(entity); };
		spButtonCom->onButtonUnhovered = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonUnhovered(entity); };
		spButtonCom->onButtonPressed = [](std::shared_ptr<Entity> entity)
			{ ButtonSignals::OnCreateWorldButtonPressed(entity); };
		spButtonCom->onButtonReleased = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonReleased(entity); };
		spButtonCom->onButtonClicked = [](std::shared_ptr<Entity> entity)
			{ButtonSignals::OnButtonClicked(entity); };
	}


	void GenerateBackgroundTiles(std::shared_ptr<Entity> spTileMap)
	{
		sf::Vector2i tilesInTileset{ 4,5 };
		sf::Vector2i tilesSize{ 64,64 };
		sf::Vector2i backgroundSize{ 10, 10 };

		//Get component
		std::shared_ptr<TileMapComponent> spTileMapCom = spTileMap->AddComponent<TileMapComponent>().lock();
		//set tilemap properties
		spTileMapCom->tileMap.tileSize = tilesSize;
		spTileMapCom->tileMap.marginSize = sf::Vector2i{ 0,0 };
		spTileMapCom->tileMap.paddingSize = sf::Vector2i{ 0,0 };
		spTileMapCom->tileMap.numTilesInTileset = tilesInTileset;
		spTileMapCom->tileMap.tilesTexturePath = "media/textures/SpaceBackground.png";
		spTileMapCom->tileMap.mapSize = backgroundSize;
		spTileMapCom->tileMap.loadTilesFromFile = false;
		spTileMapCom->tileMap.rotateTiles = true;
		//Iitialize all tiles
		spTileMapCom->tileMap.Initialize(WorldGenerator::Instance().GenerateGridOfTiles(backgroundSize, sf::Vector2i{ 0, (tilesInTileset.x * tilesInTileset.y) - 1 }), WorldGenerator::Instance().GenerateGridOfRandomNumbers(backgroundSize, sf::Vector2i{ 0, 3 }));
		spTileMap->SetPosition(sf::Vector2f{ (float)(tilesSize.x * backgroundSize.x / (-2.f)),(float)(tilesSize.y * backgroundSize.y / (-2.f)) });
	}
}


//Initialize MainMenuScene
void InitializeMainMenuScene(std::shared_ptr<SceneNode> sceneNode, std::shared_ptr<SceneNode> uiNode)
{
	//Create nodes, in which i will sort new entities which will be created during the game
	std::weak_ptr<Entity> wpCameras = ECSGame::Instance().GetEntityManager().NewEntity("Cameras");
	sceneNode->AddChild(std::make_shared<SceneNode>(wpCameras));

	std::weak_ptr<Entity> wpBackgroundE = ECSGame::Instance().GetEntityManager().NewEntity("Background");
	sceneNode->AddChild(std::make_shared<SceneNode>(wpBackgroundE));

	MainMenuScene::GenerateBackgroundTiles(wpBackgroundE.lock());
	//Initialize all cameras
	MainMenuScene::InitializeAllCameras(ECSGame::Instance().GetWindowSize(), sceneNode, uiNode);
	//Setup mouseIcon
	MainMenuScene::InitializeMouseIcon(sceneNode, uiNode);
	//Initialize game ui
	MainMenuScene::CreateUI(sceneNode, uiNode);

	uiNode->ChangeChildOrder(uiNode->FindChild("MouseIcon").lock(), (int)uiNode->GetAllChildren().size() - 1);

	signals::onChangeInputType(InputType::Menu);
}