//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Sprite.h"
#include "Render/Sprite.h"

std::filesystem::path editor::resources::Sprite::_spritesDirectory;

editor::resources::Sprite::Sprite(Project *project) :
    EditorResource("sprite"),
    _name(),
    _height(1),
    _width(1),
    _x(0),
    _y(0),
    _source(""),
    _project(project) {

}

editor::resources::Sprite::~Sprite() = default;

void editor::resources::Sprite::init(std::string const &name, std::filesystem::path const &path, int x, int y, int w,
    int h) {
    _name = name;
    _source = path;
    _height = h;
    _width = w;
    _x = x;
    _y = y;
    _init = true;
}

bool editor::resources::Sprite::readFromLua(std::string const &name) {
}

void editor::resources::Sprite::writeToLua() {
}

void editor::resources::Sprite::writeToEngineLua(const std::string &platform) {
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

