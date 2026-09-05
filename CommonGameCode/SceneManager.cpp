#include <memory>
#include <string>
#include <vector>
#include <sigslot/signal.hpp>
#include "Entity.h"
#include "SceneNode.h"
#include "System.h"
#include <functional>
#include "SceneManager.h"

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
	std::thread loadSceneAsync(LoadSceneAsynchronously, sceneName);
	loadSceneAsync.detach();
}


void SceneManager::LoadSceneAsynchronously(std::string sceneName)
{
	std::shared_ptr<SceneNode> root = std::make_shared<SceneNode>();
	std::shared_ptr<SceneNode> sceneNode = std::make_shared<SceneNode>();
	root->AddChild(sceneNode);
	std::shared_ptr<SceneNode> uiNode = std::make_shared<SceneNode>();
	root->AddChild(uiNode);

	scenesInitializers[sceneName](sceneNode, uiNode);

	signals::onSceneLoaded(root);
}