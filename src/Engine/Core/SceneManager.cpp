#include "SceneManager.h"
#include "ComponentFactory.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "SceneBlueprint.h"
#include "PrefabBlueprint.h"
#include <Load/ResourceHandler.h>
#include <Utils/RPGError.h>

SceneManager* SceneManager::_instance = nullptr;

SceneManager::SceneManager() : _scenes()
{
	_game = new Game(this);
	_factory = new ComponentFactory();
}

SceneManager::~SceneManager()
{
	for (auto scene : _scenes) {
		delete scene;
	}
	_scenes.clear();
	delete _game;
	delete _factory;
}

Entity* SceneManager::createEntity(const EntityBlueprint* blueprint, Scene* scene)
{
	if (blueprint == nullptr) return nullptr;
	Entity* entity = new Entity();
	for (const EntityBlueprint& childBp : blueprint->getChildren()) {
		Entity* child = createEntity(&childBp, scene);
		if (!child) {
			delete entity;
			return nullptr;
		}
		if (!entity->addChild(child)) {
			delete child;
			delete entity;
			return nullptr;
		}
	}
	for (ComponentData* data : blueprint->getComponents()) {
		Component* component = _factory->createComponent(data);
		if (!component) {
			delete entity;
			return nullptr;
		}
		entity->addComponent(component);
		component->setContext(entity,scene,_game);
	}
	return entity;
}


Scene* SceneManager::createScene(const SceneBlueprint* blueprint)
{
	if (blueprint == nullptr) return nullptr;
	Scene* scene = new Scene();
	for (const EntityBlueprint& entityBp : blueprint->getEntities()) {
		Entity* entity = createEntity(&entityBp, scene);
		if (!entity) {
			delete scene;
			return nullptr;
		}
		scene->addEntity(entity);
	}
	if (!scene->init()) {
		delete scene;
		return nullptr;
	}
	return scene;
}

bool SceneManager::init(const std::string &startScene) {
	Scene* scene = addScene(startScene);
	return scene != nullptr;
}


SceneManager* SceneManager::Init(const std::string& startScene)
{
	if (_instance == nullptr) {
		_instance = new SceneManager();
		_instance->init(startScene);
		return _instance;
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

bool SceneManager::render(RenderManager* render) const {
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
	Entity* prefab = createEntity(ResourceHandler<PrefabBlueprint>::Instance()->get(handler),_scenes.back());
	if (prefab) {
		_scenes.back()->addEntity(prefab);
		if (!prefab->init()) {
			delete prefab;
			return nullptr;
		}
	}
	return prefab;
}

Scene* SceneManager::addScene(const std::string& handler)
{
	Scene* newScene = createScene(ResourceHandler<SceneBlueprint>::Instance()->get(handler));
	if (newScene) {
		_scenes.push_back(newScene);
	}
	return newScene;
}

void SceneManager::popScene()
{
	if (_scenes.size() > 1) {
		delete _scenes.back();
		_scenes.pop_back();
	}
}