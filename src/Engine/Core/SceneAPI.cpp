#include "SceneAPI.h"
#include "SceneAPI.h"
#include "SceneAPI.h"
#include "SceneManager.h"

Game::Game(SceneManager* manager) : _manager(manager)
{
}

Entity* Game::instantiatePrefab(const std::string& handler) const {
	return _manager->instantiatePrefab(handler);
}

Scene* Game::addScene(std::string handler) const {
	return _manager->addScene(handler);
}

void Game::popScene() const {
	_manager->popScene();
}
