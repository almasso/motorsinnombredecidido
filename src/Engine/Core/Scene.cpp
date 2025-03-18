#include "Scene.h"

Scene::Scene()
{
}

bool Scene::update()
{
	for (const auto& [handler, entity] : entities) {
		if (!entity->update()) {
			return false;
		}
	}
	return true;
}

bool Scene::fixedUpdate()
{
	for (const auto& [handler, entity] : entities) {
		if (!entity->fixedUpdate()) {
			return false;
		}
	}
	return true;
}

bool Scene::render(RenderManager* manager)
{
	for (const auto& [layer, components] : renderComponents) {
		for (const auto& component : components) {
			if (!component->render(manager)) {
				return false;
			}
		}
	}
	return true;
}

void Scene::refresh()
{
}

void Scene::addEntity(Entity* entity, const std::string& handler)
{
	entities.insert({handler,entity});
}

Entity* Scene::getEntityByHandler(const std::string& handler)
{
	auto entityFinder = entities.find(handler);
	if (entityFinder == entities.end()) return nullptr;
	else return entityFinder->second;
}

void Scene::registerRenderComponent(RenderComponent* component, int layer)
{
	renderComponents[layer].insert(component);
}

void Scene::unregisterRenderComponent(RenderComponent* component, int layer)
{
	auto layerFinder = renderComponents.find(layer);
	if (layerFinder != renderComponents.end()) {
		layerFinder->second.erase(component);
		if (layerFinder->second.empty()) {
			renderComponents.erase(layerFinder);
		}
	}
}
