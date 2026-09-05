#pragma once
#include <memory>
#include <string>
#include <vector>
#include <sigslot/signal.hpp>
#include "Entity.h"
#include "SceneNode.h"
#include "System.h"
#include <functional>

class SceneManager
{
public:

	SceneManager();

	//Scene node first parameter, ui node second parameter
	void AddScene(std::string sceneName, std::function<void(std::shared_ptr<SceneNode>, std::shared_ptr<SceneNode>)> initializeSceneNodes);

private:

	void OnLoadScene(std::string sceneName);
	void LoadSceneAsynchronously(std::string sceneName);
	
	std::unordered_map<std::string, std::function<void(std::shared_ptr<SceneNode>, std::shared_ptr<SceneNode>)>> scenesInitializers;
};
