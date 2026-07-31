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


//INPUT SYSTEM
void InputSystem::Initialize() 
{
	//Subscribe functions to these signals
	signals::onKeyPressed.connect(&InputSystem::OnKeyPressed, this);
	signals::onKeyReleased.connect(&InputSystem::OnKeyReleased, this);
	signals::onMouseWheelScrolled.connect(&InputSystem::OnMouseWheelScrolled, this);
}

//Process keys they are pressed
void InputSystem::OnKeyPressed(sf::Event::KeyPressed key) 
{
	if (key.code == sf::Keyboard::Key::Q)
	{
		;
	}
	else if (key.code == sf::Keyboard::Key::Escape)
	{
		//Close game
		ECSGame::Instance().CloseGame();
	}
}


void InputSystem::OnKeyReleased(sf::Event::KeyReleased key)
{
	//If key released then reset bool values
}


void InputSystem::OnMouseWheelScrolled(sf::Event::MouseWheelScrolled mw) 
{
	std::shared_ptr<CameraComponent> spCameraCom = GetCameraFromCameraEntity();
	//Zoom camera
	spCameraCom->currentZoom = gel::clamp((spCameraCom->zoomingSpeed * ECSGame::Instance().GetDeltaTime() * mw.delta) + spCameraCom->currentZoom, spCameraCom->zoomingBorders.x, spCameraCom->zoomingBorders.y);
	spCameraCom->view.setSize(spCameraCom->cameraSize * spCameraCom->currentZoom);
}


//I process movement and fire keys in every frame, because game reacts to the key press
//on the same frame as it was pressed, and it will react every fram until the key
//is released. If I would use events, they are not called every frame, which is bad
void InputSystem::Update(SceneNode& scene, float deltaTime)
{
	//Set direction to 0,0
	sf::Vector2f direction{ 0,0 };

	if (ECSGame::Instance().GetGameState() == GameState::Game)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			//Change direction to positive
			direction.y -= 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			//Change direction to negative
			direction.y += 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			direction.x -= 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			direction.x += 1.f;
		}
	}

	//Signal the direction to the movement system
	signals::onMoveCamera(direction);
}


//MOVEMENT SYSTEM
void MovementSystem::Initialize()
{
	//Subscribe to this signal, to get player direction every frame
	signals::onMoveCamera.connect(&MovementSystem::OnMoveCamera, this);
	//signals::onZoomCamera.connect(&MovementSystem::OnZoomCamera, this);
}


void MovementSystem::Update(SceneNode& scene, float deltaTime) 
{
	//If game paused, then do nothing
	if (ECSGame::Instance().GetGameState() != GameState::Pause)
	{
		SceneNodeVisitorMovement visitor(*this);
		scene.AcceptVisitor(visitor);
	}
}


//UI SYSTEM
void UISystem::Initialize() 
{
	//Subscribe to some signals
}

void UISystem::Update(SceneNode& scene, float deltaTime) 
{
	SceneNodeVisitorUI visitor(*this);
	scene.AcceptVisitor(visitor);
}




//MUSIC SYSTEM
void MusicSystem::Initialize()
{
	/*
	//Subscribe to some signals
	signals::onGameRestart.connect(&MusicSystem::OnGameRestart, this);
	signals::onPlayExplosionSound.connect(&MusicSystem::OnPlayExplosionSound, this);
	signals::onPlayerHurt.connect(&MusicSystem::OnPlayHurtSound, this);
	signals::onPlayShotSound.connect(&MusicSystem::OnPlayShotSound, this);
	signals::onPlayWarningSound.connect(&MusicSystem::OnPlayWarningSound, this);
	signals::onPlayPowerUpSound.connect(&MusicSystem::OnPlayPowerUpSound, this);
	signals::onPlayManyExplosionsSounds.connect(&MusicSystem::OnPlayManyExplosionsSounds, this);
	signals::onPlayWonSound.connect(&MusicSystem::OnPlayWonSound, this);
	//Initialize music
	mainMusic = ResourceManager::Instance().LoadMusic("media/music/time_for_adventure.mp3").lock();
	mainMusic->setLooping(true);
	mainMusic->play();
	//Load sound
	spExplosionSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/explosion.wav").lock());
	spHurtSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/hurt.wav").lock());
	spShotSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/tap.wav").lock());
	spWarningSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/jump.wav").lock());
	spPowerUpSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/power_up.wav").lock());
	spWonSound = std::make_shared<sf::Sound>(*ResourceManager::Instance().LoadSoundBuffer("media/sfx/coin.wav").lock());
	*/
}

void MusicSystem::Update(SceneNode& scene, float deltaTime)
{
	/*
	//Check if we played all explosion sound
	if (playExplosionSound > 0)
	{
		timeLeftToPlaySound -= deltaTime;
		//Check when we need to play it again
		if (timeLeftToPlaySound <= 0)
		{
			spExplosionSound->play();
			timeLeftToPlaySound += gel::Randf(0.1f, 0.5f);
			playExplosionSound--;
		}
	}

	//Check if we played all won sound
	if (playWonSound > 0)
	{
		timeLeftToPlayWonSound -= deltaTime;
		//Check when we need to play it again
		if (timeLeftToPlayWonSound <= 0)
		{
			spWonSound->play();
			timeLeftToPlayWonSound += 1.f;
			playWonSound--;
		}
	}*/
}



//GAME SYSTEM
void GameSystem::Initialize()
{
	//Subscribe to some signals
}

//Update player`s invulnerability and shield
void GameSystem::Update(SceneNode& scene, float deltaTime)
{

}
