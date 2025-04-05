//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Tileset.h"

#include <io/LuaManager.h>
#include <SDL3/SDL.h>
#include "render/RenderManager.h"
#include <sol/table.hpp>

#define sourceKey "source"
#define offsetXKey "offsetX"
#define offsetYKey "offsetY"

std::filesystem::path editor::resources::Tileset::_tilesetsDirectory;

editor::resources::Tileset::Tileset() :
    EditorResource("tileset"),
    _offsetX(0),
    _offsetY(0) {
}

void editor::resources::Tileset::init(std::string const& name, std::filesystem::path const& source, int offsetX, int offsetY) {
    _name = name;
    _source = source;
    _offsetX = offsetX;
    _offsetY = offsetY;
    generateTileset();
}

bool editor::resources::Tileset::readFromLua(std::string const& name) {
    sol::table tilesetTable = io::LuaManager::GetInstance().getTable(GetFilePath(name));
    if (!tilesetTable.valid())
        return false;

    sol::optional<std::string> source = tilesetTable.get<sol::optional<std::string>>(sourceKey);
    if (!source.has_value())
        return false;

    sol::optional<int> offsetX = tilesetTable.get<sol::optional<int>>(offsetXKey);
    if (!offsetX.has_value())
        return false;

    sol::optional<int> offsetY = tilesetTable.get<sol::optional<int>>(offsetYKey);
    if (!offsetY.has_value())
        return false;

    init(name, source.value(), offsetX.value(), offsetY.value());

    return true;
}

void editor::resources::Tileset::writeToLua() {
    sol::table tilesetTable;

    tilesetTable[sourceKey] = _source.string();
    tilesetTable[offsetXKey] = _offsetX;
    tilesetTable[offsetYKey] = _offsetY;

    io::LuaManager::GetInstance().writeToFile(tilesetTable, GetFilePath(_name));
}

void editor::resources::Tileset::writeToEngineLua() {

}

const std::vector<editor::resources::Tile*> &editor::resources::Tileset::getTiles() const {
    return _tiles;
}

const std::filesystem::path &editor::resources::Tileset::getSource() const {
    return _source;
}

const std::string &editor::resources::Tileset::getName() const {
    return _name;
}

void editor::resources::Tileset::SetMapsDirectory(std::filesystem::path const& tilesetsDirectory) {
    _tilesetsDirectory = tilesetsDirectory;
}

void editor::resources::Tileset::generateTileset() {

}

std::string editor::resources::Tileset::GetFilePath(std::string const& mapName) {
    return (_tilesetsDirectory / (mapName) / (".lua")).string();
}


