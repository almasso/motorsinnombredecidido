//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//


#include "Animation.h"
#include "resources/Sprite.h"

std::filesystem::path editor::resources::Animation::_animationsDirectory;

editor::resources::Animation::Animation(Project *project) :
    EditorResource("animation"),
    _name(),
    _timeBetweenFrames(1.0f),
    _loop(false),
    _project(project) {

}

editor::resources::Animation::~Animation() = default;

void editor::resources::Animation::init(const std::string &name, const std::vector<editor::resources::Sprite*> &frames,
    float timeBetweenFrames, bool loop) {
    _name = name;
    _frames = frames;
    _timeBetweenFrames = timeBetweenFrames;
    _loop = loop;
    _init = true;
}

bool editor::resources::Animation::readFromLua(const std::string &name) {
}

void editor::resources::Animation::writeToLua() {
}

void editor::resources::Animation::writeToEngineLua(const std::string &platform) {
}

const std::string & editor::resources::Animation::getName() const {
    return _name;
}

bool editor::resources::Animation::isInitialized() const {
    return _init;
}

const std::vector<editor::resources::Sprite*> & editor::resources::Animation::getFrames() const {
    return _frames;
}

float editor::resources::Animation::getTimeBetweenFrames() const {
    return _timeBetweenFrames;
}

bool editor::resources::Animation::isLoop() const {
    return _loop;
}
