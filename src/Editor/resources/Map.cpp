//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Map.h"

editor::resources::Map::Map(int mapWidth, int mapHeight, int layers) : EditorResource("map"),
_mapWidth(mapWidth), _mapHeight(mapHeight), _layers(layers) {
    _tiles = std::vector<std::vector<ImTextureID>>(_layers, std::vector<ImTextureID>(_mapWidth * _mapHeight, 0));
    _collisions = std::vector<bool>(_mapWidth * _mapHeight, false);
}

void editor::resources::Map::writeToLua() {

}

void editor::resources::Map::readFromLua() {

}

void editor::resources::Map::writeToEngineLua() {

}

std::vector<std::vector<ImTextureID>> &editor::resources::Map::getTiles() {
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
