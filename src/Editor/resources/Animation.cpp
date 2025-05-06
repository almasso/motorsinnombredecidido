//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//


#include "Animation.h"
#include "resources/Sprite.h"
#include "io/LuaManager.h"
#include "common/Project.h"

#define framesKey "frames"
#define timeBFKey "timeBetweenFrames"
#define loopKey "loop"

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

void editor::resources::Animation::init(const std::string& name, const sol::table& framesTable, float timeBetweenFrames, bool loop) {
    std::vector<Sprite*> tmpFrames;
    for(int i = 1; i <= framesTable.size(); ++i) {
        std::string frameName = framesTable[i];
        _frames.push_back(_project->getSprite(frameName));
    }

    init(name, tmpFrames, timeBetweenFrames, loop);
}

bool editor::resources::Animation::readFromLua(const std::string &name) {
    sol::table animationTable = io::LuaManager::GetInstance().getTable(getFilePath(name));
    if(!animationTable.valid())
        return false;

    sol::optional<float> timeBetweenFrames = animationTable.get<sol::optional<float>>(timeBFKey);
    if(!timeBetweenFrames.has_value())
        return false;

    sol::optional<bool> loop = animationTable.get<sol::optional<bool>>(loopKey);
    if(!loop.has_value())
        return false;

    sol::optional<sol::table> frames = animationTable.get<sol::optional<sol::table>>(framesKey);
    if(frames.has_value())
        return false;

    init(name, frames.value(), timeBetweenFrames.value(), loop.value());

    return true;
}

void editor::resources::Animation::writeToLua() {
    sol::table animationTable = io::LuaManager::GetInstance().getState().create_table();
    sol::table frameTable = io::LuaManager::GetInstance().getState().create_table();

    for(int i = 0; i < _frames.size(); ++i) {
        frameTable[i + 1] = _frames[i]->getName();
    }

    animationTable[framesKey] = frameTable;
    animationTable[timeBFKey] = _timeBetweenFrames;
    animationTable[loopKey] = _loop;

    io::LuaManager::GetInstance().writeToFile(animationTable, getFilePath(_name));
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

editor::resources::Sprite* editor::resources::Animation::getFrameAtTime(float time) const {
    float accum = 0.0f;
    for(auto& frame : _frames) {
        accum += _timeBetweenFrames;
        if(time <= accum) return frame;
    }
    return _frames.back();
}

float editor::resources::Animation::getTotalDuration() const {
    return _timeBetweenFrames * _frames.size();
}

void editor::resources::Animation::setAnimationsDirectory(const std::filesystem::path &animationsDirectory) {
    _animationsDirectory = animationsDirectory;
}

std::string editor::resources::Animation::getFilePath(const std::string &animationName) {
    return (_animationsDirectory / (animationName + ".lua")).string();
}
