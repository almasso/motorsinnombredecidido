#include "OverworldManager.h"
#include "MapComponent.h"
#include <Core/ComponentData.h>
#include <Core/Entity.h>
#include <Core/Game.h>
#include <Utils/Error.h>


Entity* OverworldManager::addMap(const std::string &mapName) {
    if (Entity* map = _game->instantiatePrefab(mapName)) {
        _loadedMaps.insert({mapName, map});
        return map;
    }
    return nullptr;
}

OverworldManager::OverworldManager(const ComponentData *data) : ComponentTemplate(data) {
}


bool OverworldManager::init() {
    std::string startingMap = _data->getData<std::string>("startingMap");
    Entity* map = addMap(startingMap);
    if (!map) {
        Error::ShowError("Fallo al cargar el mapa " + startingMap, "No se encontro prefab para el mapa " + startingMap);
        return false;
    }
    MapComponent *mapComp = map->getComponent<MapComponent>();
    if (!mapComp) {
        Error::ShowError("Fallo al cargar el mapa " + startingMap, "Le falta el MapComponent al prefab " + startingMap);
        return false;
    }
    return changeMap(mapComp->getAdjacentMaps());
}

bool OverworldManager::changeMap(std::unordered_set<std::string> adjacent) {
    for (auto iter = _loadedMaps.begin(); iter != _loadedMaps.end();) {
        if (!adjacent.contains(iter->first)) {
            iter->second->setActive(false);
            _unloadedMaps.insert(std::move(*iter));
            iter = _loadedMaps.erase(iter);
        }
        else {
            adjacent.erase(iter->first);
            ++iter;
        }
    }
    for (const std::string& mapName : adjacent) {
        if (auto mapIter = _unloadedMaps.find(mapName); mapIter != _unloadedMaps.end()) {
            mapIter->second->setActive(true);
            _loadedMaps.insert(std::move(*mapIter));
            _unloadedMaps.erase(mapIter);
        }
        else {
            if (!addMap(mapName)) {
                Error::ShowError("Fallo al cargar el mapa " + mapName, "No se encontro prefab para el mapa " + mapName);
                return false;
            }

        }
    }
    return true;
}
