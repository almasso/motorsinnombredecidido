#include "Game.h"
#include "SceneManager.h"
#include <sol/state.hpp>
#include "Scene.h"
#include "Entity.h"

Game::Game(SceneManager* manager) : _manager(manager)
{
}

Entity* Game::instantiatePrefab(const std::string& handler) const {
	return _manager->instantiatePrefab(handler);
}

Scene* Game::addScene(const std::string& handler) const {
	return _manager->addScene(handler);
}

void Game::popScene() const {
	_manager->popScene();
}

void Game::RegisterToLua(sol::state& lua) {
	sol::usertype<Game> type = lua.new_usertype<Game>("Game");
	type["instantiatePrefab"] = &Game::instantiatePrefab;
	type["addScene"] = &Game::addScene;
	type["popScene"] = &Game::popScene;
}
