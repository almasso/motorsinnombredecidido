#include "SceneManager.h"
#include "SceneAPI.h"
#include "Scene.h"
#include <Utils/RPGError.h>

SceneManager::SceneManager() : scenes()
{
	api = new SceneAPI(this);
}

SceneManager::~SceneManager()
{
	delete api;
}

Entity* SceneManager::createPrefab(std::string handler)
{
	return nullptr;
}

Scene* SceneManager::createScene(std::string handler)
{
	return nullptr;
}


SceneManager* SceneManager::Init()
{
	if (instance == nullptr) {
		return instance = new SceneManager();
	}
	RPGError::ShowError("Error al inicializar SceneManager", "Ya existia una instancia de SceneManager");
	return nullptr;
}

bool SceneManager::update()
{
	return scenes.back()->update();
}

bool SceneManager::fixedUpdate()
{
	return scenes.back()->fixedUpdate();
}

bool SceneManager::render(RenderManager* render)
{
	for (Scene* scene : scenes) {
		if (!scene->render(render)) {
			return false;
		}
	}
	return true;
}

void SceneManager::refresh()
{
	scenes.back()->refresh();
}

void SceneManager::shutdown()
{
	delete instance;
}

Entity* SceneManager::instanciatePrefab(std::string handler)
{
	Entity* prefab = createPrefab(handler);
}

Scene* SceneManager::addScene(std::string handler)
{
	Scene* newScene = createScene(handler);
	scenes.push_back(newScene);
	return newScene;
}

void SceneManager::popScene()
{
	if (scenes.size() > 1) {
		delete scenes.back();
		scenes.pop_back();
	}
}