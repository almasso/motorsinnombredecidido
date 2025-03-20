#include "SceneManager.h"
#include "SceneAPI.h"
#include "Scene.h"
#include <Utils/RPGError.h>

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager() : _scenes()
{
	_api = new SceneAPI(this);
}

SceneManager::~SceneManager()
{
	delete _api;
}

Entity* SceneManager::createPrefab(const std::string& handler)
{
	return nullptr;
}

Scene* SceneManager::createScene(const std::string& handler)
{
	return nullptr;
}


SceneManager* SceneManager::Init()
{
	if (_instance == nullptr) {
		return _instance = new SceneManager();
	}
	RPGError::ShowError("Error al inicializar SceneManager", "Ya existia una instancia de SceneManager");
	return nullptr;
}

bool SceneManager::update() const {
	return _scenes.back()->update();
}

bool SceneManager::fixedUpdate() const {
	return _scenes.back()->fixedUpdate();
}

bool SceneManager::render(RenderManager* render)
{
	for (Scene* scene : _scenes) {
		if (!scene->render(render)) {
			return false;
		}
	}
	return true;
}

void SceneManager::refresh() const {
	_scenes.back()->refresh();
}

void SceneManager::shutdown() const {
	delete this;
}

Entity* SceneManager::instantiatePrefab(const std::string& handler)
{
	Entity* prefab = createPrefab(handler);
	return prefab;
}

Scene* SceneManager::addScene(const std::string& handler)
{
	Scene* newScene = createScene(handler);
	_scenes.push_back(newScene);
	return newScene;
}

void SceneManager::popScene()
{
	if (_scenes.size() > 1) {
		delete _scenes.back();
		_scenes.pop_back();
	}
}