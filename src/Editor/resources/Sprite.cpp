//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Sprite.h"
#include "Render/Sprite.h"

#include <SDL3/SDL_render.h>

#include "render/RenderManager.h"
#include <sol/sol.hpp>
#include "common/EditorError.h"
#include "common/Project.h"
#include "io/LuaManager.h"

#define sourceKey "source"
#define heightKey "height"
#define widthKey "width"
#define offsetXKey "offsetX"
#define offsetYKey "offsetY"

std::filesystem::path editor::resources::Sprite::_spritesDirectory;

editor::resources::Sprite::Sprite(Project *project) :
    EditorResource("sprite"),
    _height(1),
    _width(1),
    _x(0),
    _y(0),
    _source(""),
    _project(project),
    _textureID(0) {

}

editor::resources::Sprite::~Sprite() {
    if(_textureID != 0) editor::render::RenderManager::GetInstance().destroyTexture(_textureID);
}

void editor::resources::Sprite::init(std::string const &name, std::filesystem::path const &path, ImTextureID textureID, int x, int y, int w,
    int h) {
    _name = name;
    _source = path;
    _height = h;
    _width = w;
    _x = x;
    _y = y;
    _init = true;
    if(_textureID != 0 && textureID != _textureID) editor::render::RenderManager::GetInstance().destroyTexture(_textureID);
    _textureID = textureID;
}

void editor::resources::Sprite::init(const std::string& name, const std::filesystem::path& path, int x, int y, int w, int h) {
    init(name, path, render::RenderManager::GetInstance().loadTexture(path.string()), x, y, w, h);
}

bool editor::resources::Sprite::readFromLua(std::string const &name) {
    sol::table spriteTable = io::LuaManager::GetInstance().getTable(getFilePath(name));
    if(!spriteTable.valid())
        return false;

    sol::optional<std::string> source = spriteTable.get<sol::optional<std::string>>(sourceKey);
    if(!source.has_value())
        return false;

    sol::optional<int> offsetX = spriteTable.get<sol::optional<int>>(offsetXKey);
    if(!offsetX.has_value())
        return false;

    sol::optional<int> offsetY = spriteTable.get<sol::optional<int>>(offsetYKey);
    if(!offsetY.has_value())
        return false;

    sol::optional<int> width = spriteTable.get<sol::optional<int>>(widthKey);
    if(!width.has_value())
        return false;

    sol::optional<int> height = spriteTable.get<sol::optional<int>>(heightKey);
    if(!height.has_value())
        return false;

    init(name, _project->getAssetsPath() / source.value(), offsetX.value(), offsetY.value(), width.value(), height.value());

    return true;
}

void editor::resources::Sprite::writeToLua() {
    sol::table spriteTable = io::LuaManager::GetInstance().getState().create_table();

    spriteTable[sourceKey] = _source.lexically_relative(_project->getAssetsPath()).string();
    spriteTable[offsetXKey] = _x;
    spriteTable[offsetYKey] = _y;
    spriteTable[widthKey] = _width;
    spriteTable[heightKey] = _height;

    io::LuaManager::GetInstance().writeToFile(spriteTable, getFilePath(_name));
}

void editor::resources::Sprite::writeToEngineLua(const std::string &platform) {
    sol::table spriteTable = io::LuaManager::GetInstance().getState().create_table();

    spriteTable["texture"] = "data/assets/" + _source.lexically_relative(_project->getAssetsPath()).string();
    sol::table rectTable = io::LuaManager::GetInstance().getState().create_table();
    rectTable["x"] = _x;
    rectTable["y"] = _y;
    rectTable["w"] = _width;
    rectTable["h"] = _height;
    spriteTable["rect"] = rectTable;

    io::LuaManager::GetInstance().writeToFile(spriteTable, (_project->getBuildPath(platform) / "data" / "sprites"/ (_name + ".lua")).string());
}

const std::string & editor::resources::Sprite::getName() const {
    return _name;
}

const std::filesystem::path & editor::resources::Sprite::getSource() const {
    return _source;
}

bool editor::resources::Sprite::isInitialized() const {
    return _init;
}

int editor::resources::Sprite::getX() const {
    return _x;
}

int editor::resources::Sprite::getY() const {
    return _y;
}

int editor::resources::Sprite::getWidth() const {
    return _width;
}

int editor::resources::Sprite::getHeight() const {
    return _height;
}

ImVec2 editor::resources::Sprite::getSpriteCoordsMin() const {
    return ImVec2(static_cast<float>(_x) / reinterpret_cast<SDL_Texture *>(_textureID)->w,
        static_cast<float>(_y) / reinterpret_cast<SDL_Texture *>(_textureID)->h);
}

ImVec2 editor::resources::Sprite::getSpriteCoordsMax() const {
    return ImVec2(static_cast<float>(_x + _width) / reinterpret_cast<SDL_Texture *>(_textureID)->w,
        static_cast<float>(_y + _height) / reinterpret_cast<SDL_Texture *>(_textureID)->h);
}

const ImTextureID editor::resources::Sprite::getTextureID() const {
    return _textureID;
}

void editor::resources::Sprite::setSpritesDirectory(const std::filesystem::path &spritesDirectory) {
    _spritesDirectory = spritesDirectory;
}

std::string editor::resources::Sprite::getFilePath(const std::string &spriteName) {
    return (_spritesDirectory / (spriteName + ".lua")).string();
}

