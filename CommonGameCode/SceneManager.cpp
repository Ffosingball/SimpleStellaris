#include <memory>
#include <string>
#include <vector>
#include <sigslot/signal.hpp>
#include "Entity.h"
#include "SceneNode.h"
#include "System.h"
#include <functional>
#include "SceneManager.h"
#include "ECSGame.h"

SceneManager::SceneManager() 
{
	signals::onLoadSceneAsync.connect(&SceneManager::OnLoadScene, this);
	signals::onLoadScene.connect(&SceneManager::LoadSceneAsynchronously, this);
}

void SceneManager::AddScene(std::string sceneName, std::function<void(std::shared_ptr<SceneNode>, std::shared_ptr<SceneNode>)> initializeSceneNodes)
{
	scenesInitializers[sceneName] = initializeSceneNodes;
}


void SceneManager::OnLoadScene(std::string sceneName) 
{
	std::thread loadSceneAsync(&SceneManager::LoadSceneAsynchronously, this, sceneName);
	loadSceneAsync.detach();
}


void SceneManager::LoadSceneAsynchronously(std::string sceneName)
{
	ECSGame::Instance().SetGameState(GameState::Loading);
	ECSGame::Instance().SetDeltaTimeMultiplier(0.f);

	std::weak_ptr<Entity> wpRoot = ECSGame::Instance().GetEntityManager().NewEntity(sceneName);
	std::shared_ptr<SceneNode> root = std::make_shared<SceneNode>(wpRoot);
	std::weak_ptr<Entity> wpScene = ECSGame::Instance().GetEntityManager().NewEntity("Scene");
	std::shared_ptr<SceneNode> sceneNode = std::make_shared<SceneNode>(wpScene);
	root->AddChild(sceneNode);
	std::weak_ptr<Entity> wpUI = ECSGame::Instance().GetEntityManager().NewEntity("UI");
	std::shared_ptr<SceneNode> uiNode = std::make_shared<SceneNode>(wpUI);
	root->AddChild(uiNode);

	scenesInitializers[sceneName](sceneNode, uiNode);

	signals::onSceneLoaded(root);
}