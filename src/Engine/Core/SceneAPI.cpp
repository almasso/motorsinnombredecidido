#include "SceneAPI.h"
#include "SceneAPI.h"
#include "SceneAPI.h"
#include "SceneManager.h"

SceneAPI::SceneAPI(SceneManager* manager) : manager(manager)
{
}

Entity* SceneAPI::instanciatePrefab(std::string handler)
{
	return manager->instanciatePrefab(handler);
}

Scene* SceneAPI::addScene(std::string handler)
{
	return manager->addScene(handler);
}

void SceneAPI::popScene()
{
	manager->popScene();
}
