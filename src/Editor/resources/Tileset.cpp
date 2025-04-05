//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Tileset.h"

void editor::resources::Tileset::readFromLua() {

}

void editor::resources::Tileset::writeToLua() {

}

void editor::resources::Tileset::writeToEngineLua() {

}

const std::vector<editor::resources::Tile*> &editor::resources::Tileset::getTiles() const {
    return _tiles;
}

const std::filesystem::path &editor::resources::Tileset::getSource() const {
    return _source;
}


