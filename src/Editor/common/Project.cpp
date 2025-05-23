//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Project.h"
#include "io/LuaManager.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <sol/sol.hpp>
#include "EditorError.h"
#include "resources/Tileset.h"
#include "resources/Map.h"
#include "resources/Sprite.h"
#include "resources/Animation.h"
#include <SDL3/SDL_filesystem.h>
#include "resources/events/Event.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <SDL3/SDL_process.h>
#endif

#ifdef __APPLE__
#define GetCurrentDir strdup(SDL_GetBasePath())
#else
#define GetCurrentDir SDL_GetCurrentDirectory()
#endif

void editor::Project::initResources() {
    if(!std::filesystem::exists(_projectPath / "projectfiles"/"maps/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"maps");
    if(!std::filesystem::exists(_projectPath / "projectfiles"/"tilesets/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"tilesets");
    if(!std::filesystem::exists(_projectPath / "projectfiles"/"events/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"events");
    if(!std::filesystem::exists(_projectPath / "projectfiles"/"sprites/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"sprites");
    if(!std::filesystem::exists(_projectPath / "projectfiles"/"animations/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"animations");

    std::filesystem::path tilesetsPath = (_projectPath / "projectfiles" / "tilesets");
    resources::Tileset::SetTilesetsDirectory(tilesetsPath);
    for (auto const& file : std::filesystem::directory_iterator(tilesetsPath)) {
        auto tileset = new resources::Tileset(this);
        auto name = file.path().stem().string();
        if (tileset->readFromLua(name))
            _tilesets.insert({name, tileset});
        else
            delete tileset;
    }

    std::filesystem::path eventsPath = (_projectPath / "projectfiles" / "events");
    resources::events::Event::SetEventsDirectory(eventsPath);
    for (auto const& file : std::filesystem::directory_iterator(eventsPath)) {
        auto event = new resources::events::Event(this);
        auto name = file.path().stem().string();
        if (event->readFromLua(name))
            _events.insert({name, event});
        else
            delete event;
    }

    std::filesystem::path mapsPath = (_projectPath / "projectfiles" / "maps");
    resources::Map::SetMapsDirectory(mapsPath);
    for (auto const& file : std::filesystem::directory_iterator(mapsPath)) {
        auto map = new resources::Map(this);
        auto name = file.path().stem().string();
        if (map->readFromLua(name))
            _maps.insert({name, map});
        else
            delete map;
    }

    std::filesystem::path spritesPath = (_projectPath / "projectfiles" / "sprites");
    resources::Sprite::setSpritesDirectory(spritesPath);
    for(auto const& file : std::filesystem::directory_iterator(spritesPath)) {
        auto sprite = new resources::Sprite(this);
        auto name = file.path().stem().string();
        if(sprite->readFromLua(name))
            _sprites.insert({name, sprite});
        else
            delete sprite;
    }

    std::filesystem::path animationsPath = (_projectPath / "projectfiles" / "animations");
    resources::Animation::setAnimationsDirectory(animationsPath);
    for(auto const& file : std::filesystem::directory_iterator(animationsPath)) {
        auto anim = new resources::Animation(this);
        auto name = file.path().stem().string();
        if(anim->readFromLua(name))
            _animations.insert({name, anim});
        else
            delete anim;
    }
}

bool editor::Project::build(const std::string &platform, const sol::table& overWorldScene, const std::array<float,3>& audio, bool genericFont) {
    try {
        if (!exists(getBuildPath("") )) {
            create_directory(getBuildPath(""));
        }
        if (exists(getBuildPath(platform))) {
            remove_all(getBuildPath(platform));
        }
        char* filepath = GetCurrentDir;
        auto path = std::filesystem::path(filepath)/"GameBinaries"/platform;
        copy(path,getBuildPath(platform),
             std::filesystem::copy_options::recursive | std::filesystem::copy_options::copy_symlinks |
            std::filesystem::copy_options::overwrite_existing);
        if (!exists(getBuildPath(platform) )) {
            create_directory(getBuildPath(platform));
        }
        if (!exists(getBuildPath(platform)/"data/"))
            create_directory(getBuildPath(platform)/"data/");
        copy(getAssetsPath(),getBuildPath(platform)/"data"/"assets",
             std::filesystem::copy_options::recursive | std::filesystem::copy_options::copy_symlinks |
             std::filesystem::copy_options::overwrite_existing);
        if (genericFont) {
            auto fontPath = std::filesystem::path(filepath)/"settings"/"fonts"/"Raleway-Regular.ttf";
            copy_file(fontPath,(getBuildPath(platform)/"data"/"assets"/"Raleway-Regular.ttf"),
                std::filesystem::copy_options::overwrite_existing);
        }
        SDL_free(filepath);
        for (const auto& [key,map] : _maps) {
            map->writeToEngineLua(platform);
        }
        for (const auto& [key,tileset] : _tilesets) {
            tileset->writeToEngineLua(platform);
        }
        io::LuaManager::GetInstance().writeToFile(overWorldScene, (getBuildPath(platform)/"data"/"scenes"/"overworld.scene.lua").string());
        buildSettings(platform);
        buildAudioSettings(platform, audio);
        buildSprites(platform);
        buildAnimations(platform);
    } catch (std::filesystem::filesystem_error& e) {
        EditorError::showError_impl(e.what(), "Project",128);
        return false;
    }
    launchBuild();
    return true;
}

void editor::Project::buildSettings(const std::string& platform) {
    auto& lua = io::LuaManager::GetInstance().getState();
    sol::table config = lua.create_table();
    sol::table memory = lua.create_table();
    memory["maxSize"] = 1024*1024*1024;
    config["memory"] = memory;
    config["initScene"] = "data/scenes/overworld.scene.lua";
    io::LuaManager::GetInstance().writeToFile(config, (getBuildPath(platform)/"data"/"config.lua").string());
}

void editor::Project::buildAudioSettings(const std::string &platform, const std::array<float,3>& audio) {
    auto& lua = io::LuaManager::GetInstance().getState();
    sol::table master = lua.create_table();
    master["inputs"] = {"data/mixers/music.mixer.lua","data/mixers/sfx.mixer.lua"};
    master["name"] = "Master";
    master["volume"] = audio[0];
    io::LuaManager::GetInstance().writeToFile(master, (getBuildPath(platform)/"data"/"mixers"/"master.mixer.lua").string());
    sol::table music = lua.create_table();
    music["output"] = "data/mixers/master.mixer.lua";
    music["name"] = "Music";
    music["volume"] = audio[1];
    io::LuaManager::GetInstance().writeToFile(music, (getBuildPath(platform)/"data"/"mixers"/"music.mixer.lua").string());
    sol::table sfx = lua.create_table();
    sfx["output"] = "data/mixers/master.mixer.lua";
    sfx["name"] = "SFX";
    sfx["volume"] = audio[2];
    io::LuaManager::GetInstance().writeToFile(sfx, (getBuildPath(platform)/"data"/"mixers"/"sfx.mixer.lua").string());
}

const std::string &editor::Project::getName() const {
    return _name;
}

int editor::Project::getMaxLayers() const {
    int max = 0;
    for (const auto& [key,map] : _maps) {
        if (map->getLayers() > max) max = map->getLayers();
    }
    return max;
}

const std::filesystem::path &editor::Project::getPath() const {
    return _projectPath;
}

const std::unordered_set<std::filesystem::path> &editor::Project::getExtraRoutes() const {
    return _extraRoutes;
}

const std::tm &editor::Project::getLastModificationTime() const {
    return _lastModified;
}

void editor::Project::updateLastModified() {
    time_t now = time(nullptr);
    _lastModified = *localtime(&now);
}

void editor::Project::findProject() {
    if(std::filesystem::exists(_projectPath)) {
        // La carpeta del proyecto existe, vamos a buscar el archivo de configuración del proyecto
        if(std::filesystem::exists(_projectPath / ("ProjectSettings.lua"))) {
            _found = true;
            sol::table T = io::LuaManager::GetInstance().getTable((_projectPath / ("ProjectSettings.lua")).string());
            loadProject(T);
        }
    }
}

void editor::Project::loadProject(const sol::table& project) {
    if(project.valid()) {
        if(project["Name"].valid()) {
            setName(project["Name"].get<std::string>());
        }

        if(project["LastModified"].valid()) {
            std::istringstream ss(project["LastModified"].get<std::string>());
            ss >> std::get_time(&_lastModified, "%d/%m/%Y %H:%M:%S");
        }

        if(project["Dimensions"].valid()) {
            _dimensions[0] = project["Dimensions"][1];
            _dimensions[1] = project["Dimensions"][2];
        }

        if(project["AdditionalRoutes"].valid()) {
            sol::table additionalRoutes = project["AdditionalRoutes"];
            for(const auto& [key, value] : additionalRoutes) {
                if(value.is<std::string>()) {
                    _extraRoutes.insert(value.as<std::string>());
                }
            }
        }
    }
}

bool editor::Project::isSetToDelete() const {
    return _setToDelete;
}

void editor::Project::setToDelete() {
    _setToDelete = true;
}

bool editor::Project::isFound() const {
    return _found;
}

void editor::Project::saveProject() {
    if(_found) {
        sol::state& L = io::LuaManager::GetInstance().getState();

        sol::table pr = L.create_table();

        std::ostringstream oss;
        oss << std::put_time(&_lastModified, "%d/%m/%Y %H:%M:%S");

        pr["Name"] = _name;
        pr["Route"] = "";
        pr["LastModified"] = oss.str();
        pr["Dimensions"] = L.create_table();
        pr["Dimensions"][1] = _dimensions[0];
        pr["Dimensions"][2] = _dimensions[1];
        int index = 1;
        pr["AdditionalRoutes"] = L.create_table();
        for(auto elem : _extraRoutes) {
            pr["AdditionalRoutes"][index++] = elem.string();
        }

        if(!std::filesystem::exists(_projectPath / "assets/")) std::filesystem::create_directories(_projectPath / "assets");
        if(!std::filesystem::exists(_projectPath / "projectfiles/")) std::filesystem::create_directories(_projectPath / "projectfiles");
        if(!std::filesystem::exists(_projectPath / "projectfiles"/"maps/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"maps");
        if(!std::filesystem::exists(_projectPath / "projectfiles"/"tilesets/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"tilesets");
        if(!std::filesystem::exists(_projectPath / "projectfiles"/"events/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"events");
        if(!std::filesystem::exists(_projectPath / "projectfiles"/"sprites/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"sprites");
        if(!std::filesystem::exists(_projectPath / "projectfiles"/"animations/")) std::filesystem::create_directories(_projectPath / "projectfiles"/"animations");

        io::LuaManager::GetInstance().writeToFile(pr, (_projectPath / ("ProjectSettings.lua")).string());
        for (auto& [key, tileset] : _tilesets)
            tileset->writeToLua();
        for (auto& [key, event] : _events)
            event->writeToLua();
        for (auto& [key, map] : _maps)
            map->writeToLua();
        for(auto& [key, sprite] : _sprites)
            sprite->writeToLua();
        for(auto& [key, animation] : _animations)
            animation->writeToLua();
    }
}

const int *editor::Project::getDimensions() const {
    return _dimensions;
}

void editor::Project::addMap(editor::resources::Map *map) {
    _maps[map->getName()] = map;
    map->writeToLua();
}

void editor::Project::refreshMaps() {
    for(auto it = _maps.begin(); it != _maps.end();) {
        if(it->second->getName() != it->first) {
            resources::Map* mptmp = it->second;
            it = _maps.erase(it);
            _maps[mptmp->getName()] = mptmp;
        }
        else ++it;
    }
}

void editor::Project::refreshTilesets() {
    for (auto it = _tilesets.begin(); it != _tilesets.end();) {
        if(it->second->getName() != it->first) {
            resources::Tileset* tstmp = it->second;
            it = _tilesets.erase(it);
            _tilesets[tstmp->getName()] = tstmp;
        }
        else ++it;
    }
}


void editor::Project::addTileset(editor::resources::Tileset *tileset) {
    _tilesets[tileset->getName()] = tileset;
    tileset->writeToLua();
}

int editor::Project::totalMaps() const {
    return _maps.size();
}

int editor::Project::totalTilesets() const {
    return _tilesets.size();
}

void editor::Project::addEvent(editor::resources::events::Event* event) {
    _events[event->getName()] = event;
    event->writeToLua();
}

int editor::Project::totalEvents() const {
    return _events.size();
}

const std::unordered_map<std::string, editor::resources::events::Event*>& editor::Project::getEvents() const {
    return _events;
}

void editor::Project::refreshEvents() {
    for (auto it = _events.begin(); it != _events.end();) {
        if(it->second->getName() != it->first) {
            resources::events::Event* evtmp = it->second;
            resources::events::Event::EraseFromLua(it->first);
            it = _events.erase(it);
            _events[evtmp->getName()] = evtmp;
            evtmp->writeToLua();
        }
        else ++it;
    }
}

const std::unordered_map<std::string, editor::resources::Map*>& editor::Project::getMaps() const {
    return _maps;
}

const std::unordered_map<std::string, editor::resources::Tileset*>& editor::Project::getTilesets() const {
    return _tilesets;
}

void editor::Project::addSprite(editor::resources::Sprite *sprite) {
    _sprites[sprite->getName()] = sprite;
    sprite->writeToLua();
}

int editor::Project::totalSprites() const {
    return _sprites.size();
}

const std::unordered_map<std::string, editor::resources::Sprite*>& editor::Project::getSprites() const {
    return _sprites;
}

void editor::Project::refreshSprites() {
    for (auto it = _sprites.begin(); it != _sprites.end();) {
        if(it->second->getName() != it->first) {
            resources::Sprite* spritetmp = it->second;
            it = _sprites.erase(it);
            _sprites[spritetmp->getName()] = spritetmp;
        }
        else ++it;
    }
}

void editor::Project::addAnimation(editor::resources::Animation *animation) {
    _animations[animation->getName()] = animation;
    animation->writeToLua();
}

int editor::Project::totalAnimations() const {
    return _animations.size();
}

const std::unordered_map<std::string, editor::resources::Animation*>& editor::Project::getAnimations() const {
    return _animations;
}

void editor::Project::refreshAnimations() {
    for (auto it = _animations.begin(); it != _animations.end();) {
        if(it->second->getName() != it->first) {
            resources::Animation* animtmp = it->second;
            it = _animations.erase(it);
            _animations[animtmp->getName()] = animtmp;
        }
        else ++it;
    }
}

std::filesystem::path editor::Project::getAssetsPath() const {
    return _projectPath / "assets";
}

void editor::Project::saveEverything() {
    updateLastModified();
    saveProject();
}

std::filesystem::path editor::Project::getBuildPath(const std::string &platform) const {
    return _projectPath / "build" / platform;
}

void editor::Project::setPlayerLocalVariables(std::unordered_map<std::string, sol::object>* localVariables) {
    _localVariables = localVariables;
}

std::unordered_map<std::string, sol::object>* editor::Project::getPlayerLocalVariables() const {
    return _localVariables;
}

void editor::Project::buildSprites(std::string const& platform) {
    if (!exists(getBuildPath(platform)/"data"/"sprites/"))
        create_directory(getBuildPath(platform)/"data"/"sprites/");
    for (auto const& [spriteName, sprite] : _sprites) {
        sprite->writeToEngineLua(platform);
    }
}

void editor::Project::buildAnimations(std::string const& platform) {
    if (!exists(getBuildPath(platform)/"data"/"animations/"))
        create_directory(getBuildPath(platform)/"data"/"animations/");
    for (auto const& [animName, animation] : _animations) {
        animation->writeToEngineLua(platform);
    }
}

#ifdef _WIN32
void editor::Project::launchBuild() {
    STARTUPINFO info={sizeof(info)};
    PROCESS_INFORMATION processInfo;
    auto exeDirectory = getBuildPath("Desktop").string();
    auto exePath = (getBuildPath("Desktop") / "Executable.exe").string();
    if (CreateProcess(NULL, const_cast<LPSTR>(exePath.c_str()), NULL, NULL, TRUE, 0, NULL, exeDirectory.c_str(), &info, &processInfo)) {
        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
}
#else
void editor::Project::launchBuild() {
    SDL_PropertiesID prop = SDL_CreateProperties();
    auto exePath = (getBuildPath("Desktop") / "Executable.exe").string();
    const char *args[] = { exePath.c_str(), NULL };
    SDL_SetPointerProperty(prop, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, args);
    SDL_CreateProcessWithProperties(prop);
}
#endif


bool editor::Project::isSpriteBeingUsedInAnim(editor::resources::Sprite *sprite, std::vector<std::string>& names) {
    names.clear();
    bool used = false;

    for(auto& [key, value] : _animations) {
        if(value->isSpriteBeingUsed(sprite)) {
            used = true;
            names.push_back(value->getName());
        }
    }
    return used;
}
