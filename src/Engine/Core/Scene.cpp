#include "Scene.h"
#include "Entity.h"
#include "Render/RenderComponent.h"
#include <sol/state.hpp>

Scene::Scene()
= default;

bool Scene::init() {
	refresh();
	for (Entity* entity : _entities) {
		if (!entity->init()) {
			return false;
		}
	}
	return true;
}

bool Scene::update() const {
	for (Entity* entity : _entities) {
		if (entity->isActive() && !entity->update()) {
			return false;
		}
	}
	return true;
}

bool Scene::fixedUpdate() const {
	for (Entity* entity : _entities) {
		if (entity->isActive() && !entity->fixedUpdate()) {
			return false;
		}
	}
	return true;
}

bool Scene::render(RenderManager* manager) {
	for (const auto& [layer, components] : _renderComponents) {
		for (const auto& component : components) {
			if (component->isEnabled() && component->isEntityActive() && !component->render(manager)) {
				return false;
			}
		}
	}
	return true;
}

void Scene::refresh()
{
	for (auto entity = _entities.begin(); entity != _entities.end();) {
		if (!(*entity)->isAlive()) {
			delete *entity;
			entity = _entities.erase(entity);
		}
		else ++entity;
	}
	for (auto entity : _entitiesToAdd) {
		_entities.insert(entity);
	}
	_entitiesToAdd.clear();
}

void Scene::addEntity(Entity* entity)
{
	_entitiesToAdd.insert(entity);
}

void Scene::addHandler(Entity* entity, const std::string &handler) {
	if (!_handlers.contains(handler)) {
		_handlers.insert({handler, entity});
	}
}

Entity* Scene::getEntityByHandler(const std::string& handler)
{
	auto entityFinder = _handlers.find(handler);
	if (entityFinder == _handlers.end()) return nullptr;
	return entityFinder->second;
}

void Scene::registerRenderComponent(RenderComponent* component, int layer)
{
	_renderComponents[layer].insert(component);
}

void Scene::unregisterRenderComponent(RenderComponent* component, const int layer)
{
	if (auto layerFinder = _renderComponents.find(layer); layerFinder != _renderComponents.end()) {
		layerFinder->second.erase(component);
		if (layerFinder->second.empty()) {
			_renderComponents.erase(layerFinder);
		}
	}
}

void Scene::RegisterToLua(sol::state& lua) {
	sol::usertype<Scene> type = lua.new_usertype<Scene>("Scene");
	type["getEntity"] = &Scene::getEntityByHandler;
}
