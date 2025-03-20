#include "SceneAPI.h"
#include "SceneAPI.h"
#include "SceneAPI.h"
#include "SceneManager.h"

SceneAPI::SceneAPI(SceneManager* manager) : _manager(manager)
{
}

Entity* SceneAPI::instantiatePrefab(const std::string& handler) const {
	return _manager->instantiatePrefab(handler);
}

Scene* SceneAPI::addScene(std::string handler) const {
	return _manager->addScene(handler);
}

void SceneAPI::popScene() const {
	_manager->popScene();
}
