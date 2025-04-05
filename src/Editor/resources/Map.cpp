//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Map.h"

#include <io/LuaManager.h>

#include "Object.h"
#include "Tile.h"

void editor::resources::Map::writeToLua() {
    sol::table map;
    map["width"] = _mapWidth;
    map["height"] = _mapHeight;
    map["layers"] = _layers;

    sol::table tiles;
    writeTiles(tiles);
    map["tiles"] = tiles;

    sol::table collisions;
    writeCollisions(collisions);
    map["collisions"] = collisions;

    sol::table objects;
    writeObjects(objects);
    map["objects"] = objects;

    io::LuaManager::GetInstance().writeToFile(map, filename);
}

void editor::resources::Map::readFromLua() {

}

void editor::resources::Map::writeToEngineLua() {

}

std::vector<std::vector<editor::resources::Tile*>> &editor::resources::Map::getTiles() {
    return _tiles;
}

std::vector<bool> &editor::resources::Map::getCollisions() {
    return _collisions;
}

int editor::resources::Map::getMapWidth() const {
    return _mapWidth;
}

int editor::resources::Map::getMapHeight() const {
    return _mapHeight;
}

int editor::resources::Map::getLayers() const {
    return _layers;
}

void editor::resources::Map::setLayers(int layers) {
    _layers = layers;
}

void editor::resources::Map::writeTiles(sol::table& tiles) {
    for (auto const& row : _tiles) {
        sol::table rowTable;
        for (auto const& tile : row) {
            sol::table tileTable;
            tileTable["tileset"] = tile->tileset;
            tileTable["pos"] = tile->pos;
            rowTable.add(tileTable);
        }
        tiles.add(rowTable);
    }
}

void editor::resources::Map::writeCollisions(sol::table& collisions) {
    for (bool const& collision : _collisions) {
        collisions.add(collision);
    }
}

bool editor::resources::Map::writeObjects(sol::table& objects) {
    for (auto const& [key, object] : _objects) {
        sol::table objectTable;
        if (!object->write(objectTable))
            return false;
        objectTable.add(objectTable);
    }
    return true;
}

const std::string &editor::resources::Map::getName() const {
    return _name;
}
