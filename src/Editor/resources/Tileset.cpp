//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Tileset.h"

#include <common/EditorError.h>
#include <common/Project.h>
#include <io/LuaManager.h>
#include <SDL3/SDL.h>
#include "render/RenderManager.h"

#include "Tile.h"

#define sourceKey "source"
#define offsetXKey "offsetX"
#define offsetYKey "offsetY"
#define collisionsKey "collisions"

std::filesystem::path editor::resources::Tileset::_tilesetsDirectory;

editor::resources::Tileset::Tileset(Project* project) :
    EditorResource("tileset"),
    _name(),
    _source(""),
    _offsetX(0),
    _offsetY(0),
    _project(project),
    _collisions(std::vector<bool>(project->getDimensions()[0] * project->getDimensions()[1], false)){
}

editor::resources::Tileset::~Tileset() {
    for (auto& tile : _tiles) {
        delete tile;
        tile = nullptr;
    }
    _tiles.clear();
}

void editor::resources::Tileset::init(std::string const& name, std::filesystem::path const& source, int offsetX, int offsetY) {
    _name = name;
    _source = source;
    _offsetX = offsetX;
    _offsetY = offsetY;
    generateTileset();
    _collisions = std::vector<bool>(_xTiles * _yTiles, false);
    _init = true;
}

void editor::resources::Tileset::init(const std::string &name, const std::filesystem::path &source,
                                      const sol::table &collisions, int offsetX, int offsetY) {
    init(name, source, offsetX, offsetY);

    for(int i = 1; i <= collisions.size(); ++i) {
        _collisions[i - 1] = collisions[i].get<bool>();
    }
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

    sol::optional<sol::table> collisions = tilesetTable.get<sol::optional<sol::table>>(collisionsKey);
    if(!collisions.has_value())
        return false;

    init(name, _project->getAssetsPath() / source.value(), collisions.value(), offsetX.value(), offsetY.value());

    return true;
}

void editor::resources::Tileset::writeToLua() {
    sol::table tilesetTable = io::LuaManager::GetInstance().getState().create_table();
    sol::table collisionsTable = io::LuaManager::GetInstance().getState().create_table();

    for(int i = 0; i < _collisions.size(); ++i) {
        collisionsTable[i + 1] = static_cast<bool>(_collisions[i]);
    }

    tilesetTable[sourceKey] = _source.lexically_relative(_project->getAssetsPath()).string();
    tilesetTable[offsetXKey] = _offsetX;
    tilesetTable[offsetYKey] = _offsetY;
    tilesetTable[collisionsKey] = collisionsTable;


    io::LuaManager::GetInstance().writeToFile(tilesetTable, GetFilePath(_name));
}

void editor::resources::Tileset::writeToEngineLua(const std::string &platform) {
    auto& lua = io::LuaManager::GetInstance().getState();
    for (Tile* tile : _tiles) {
        sol::table tileSprite = lua.create_table();
        tileSprite["texture"] = (std::filesystem::path("data") / "assets" / _source.lexically_relative(_project->getAssetsPath())).string();
        sol::table rect = lua.create_table();
        SDL_Texture* sdlTexture = (SDL_Texture*)tile->texture;

        rect["x"] = sdlTexture->w * tile->rect.Min.x;
        rect["y"] = sdlTexture->h * tile->rect.Min.y;
        rect["w"] = _project->getDimensions()[0];
        rect["h"] = _project->getDimensions()[1];
        tileSprite["rect"] = rect;
        std::string path = (_project->getBuildPath(platform)/ "data" / "sprites"/(_name+std::to_string(tile->pos)+".lua")).string();
        io::LuaManager::GetInstance().writeToFile(tileSprite, path);
    }
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

void editor::resources::Tileset::SetTilesetsDirectory(std::filesystem::path const& tilesetsDirectory) {
    _tilesetsDirectory = tilesetsDirectory;
}

void editor::resources::Tileset::generateTileset() {
    int const* dimensions = _project->getDimensions();
    ImTextureID texture = render::RenderManager::GetInstance().loadTexture(_source.string());
    if (texture == 0) {
        showError("Tileset image not found.");
        return;
    }
    SDL_Texture* sdlTexture = (SDL_Texture*)texture;
    _xTiles = sdlTexture->w / dimensions[0];
    _yTiles = sdlTexture->h / dimensions[1];
    _xTiles -= (_offsetX % dimensions[0] == 0) ? 0 : 1;
    _yTiles -= (_offsetY % dimensions[1] == 0) ? 0 : 1;
    int xOffset = std::abs(_offsetX);
    int yOffset = std::abs(_offsetY);
    for (int j = 0; j < _yTiles; j++) {
        for (int i = 0; i < _xTiles; i++) {
            Tile* tile = new Tile();
            tile->pos = i + j * _yTiles;
            tile->texture = texture;
            tile->tileset = _name;
            tile->rect = ImRect(
                static_cast<float>(i * dimensions[0] + xOffset) / sdlTexture->w,
                static_cast<float>(j * dimensions[1] + yOffset) / sdlTexture->h,
                static_cast<float>((i + 1) * dimensions[0] - 1 + xOffset) / sdlTexture->w,
                static_cast<float>((j + 1) * dimensions[1] - 1 + yOffset) / sdlTexture->h);
            _tiles.push_back(tile);
        }
    }
}

std::string editor::resources::Tileset::GetFilePath(std::string const& mapName) {
    return (_tilesetsDirectory / (mapName + ".lua")).string();
}

int editor::resources::Tileset::getOffsetX() const {
    return _offsetX;
}

int editor::resources::Tileset::getOffsetY() const {
    return _offsetY;
}

bool editor::resources::Tileset::isInitialized() const {
    return _init;
}

int editor::resources::Tileset::getXTiles() const {
    return _xTiles;
}

int editor::resources::Tileset::getYTiles() const {
    return _yTiles;
}

std::vector<bool>& editor::resources::Tileset::getCollisions() {
    return _collisions;
}




