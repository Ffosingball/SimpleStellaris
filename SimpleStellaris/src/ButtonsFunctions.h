#pragma once

#include <memory>
#include "Component.h"
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "TileMap.h"
//#include "ParticleSystem.h"
#include "SpaceObjectTypes.h"
#include <unordered_map>
#include "SpaceMapConfiguration.h"
#include "SceneNode.h"
#include "GameState.h"
#include <functional>
#include <sigslot/signal.hpp>


//List of all possible button signals
namespace ButtonSignals
{
	void ButtonHovered(std::shared_ptr<Entity> spEntity);
	void ButtonUnhovered(std::shared_ptr<Entity> spEntity);
	void ButtonReleased(std::shared_ptr<Entity> spEntity);
	void ButtonClicked(std::shared_ptr<Entity> spEntity);
	void ExitToMainMenuButtonPressed(std::shared_ptr<Entity> spEntity);
	void ExitButtonPressed(std::shared_ptr<Entity> spEntity);
	void InputBoxHovered(std::shared_ptr<Entity> spEntity);
	void InputBoxUnhovered(std::shared_ptr<Entity> spEntity);
	void InputBoxPressed(std::shared_ptr<Entity> spEntity);
	void InputBoxUnselected(std::weak_ptr<Entity> wpEntity);
	void CreateWorldButtonPressed(std::shared_ptr<Entity> wpEntity);

	inline sigslot::signal<std::shared_ptr<Entity>> OnDistrictHovered;
	inline sigslot::signal<std::shared_ptr<Entity>> OnDistrictUnhovered;
	inline sigslot::signal<std::shared_ptr<Entity>> OnButtonHovered;
	inline sigslot::signal<std::shared_ptr<Entity>> OnButtonUnhovered;
	inline sigslot::signal<std::shared_ptr<Entity>> OnButtonReleased;
	inline sigslot::signal<std::shared_ptr<Entity>> OnButtonClicked;
	inline sigslot::signal<std::shared_ptr<Entity>> OnResumeButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnExitButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnSlower3ButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnSlower2ButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnSlower1ButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnPlayingButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnStoppedButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnFaster3ButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnFaster2ButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnFaster1ButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnPreviousMusicButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnNextMusicButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnStopMusicButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnResumeMusicButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnMixMusicButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnExitToMainMenuButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnInputBoxHovered;
	inline sigslot::signal<std::shared_ptr<Entity>> OnInputBoxUnhovered;
	inline sigslot::signal<std::shared_ptr<Entity>> OnInputBoxPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnBackToMainMenuButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnStartGameButtonPressed;
	inline sigslot::signal<std::shared_ptr<Entity>> OnCreateWorldButtonPressed;
}