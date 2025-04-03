//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "PreferencesManager.h"
#include "common/EditorError.h"
#include <SDL3/SDL_filesystem.h>
#include "LuaManager.h"

#ifdef __APPLE__
#define GetCurrentDir strdup(SDL_GetBasePath())
#elif
#define GetCurrentDir SDL_GetCurrentDirectory()
#endif

std::unique_ptr<editor::io::PreferencesManager> editor::io::PreferencesManager::_instance = nullptr;

bool editor::io::PreferencesManager::Init() {
    editorAssert(_instance == nullptr, "Preferences manager singleton instance is already initialized")
    _instance = std::unique_ptr<PreferencesManager>(new PreferencesManager());
    if(!_instance->init()) {
        Destroy();
        return false;
    }
    return true;
}

bool editor::io::PreferencesManager::init() {
    _preferencesPath = "/settings/preferences/userpreferences.lua";
    _currentDirectory = GetCurrentDir;
    if(_currentDirectory == nullptr) {
        showError(SDL_GetError())
        return false;
    }
    loadPreferences();
    return true;
}

void editor::io::PreferencesManager::loadPreferences() {
    _preferences = LuaManager::GetInstance().getState().create_table();
    if(std::filesystem::exists(std::filesystem::path(std::string(_currentDirectory) + _preferencesPath.string()).lexically_normal().string())) {
        _preferences = LuaManager::GetInstance().getTableFromScript(std::filesystem::path(std::string(_currentDirectory) + _preferencesPath.string()).lexically_normal().string().c_str());
    }
}

void editor::io::PreferencesManager::Destroy() {
    _instance.reset(nullptr);
}

editor::io::PreferencesManager &editor::io::PreferencesManager::GetInstance() {
    editorAssert(_instance != nullptr, "Preferences manager singleton instance is not initialized")
    return *_instance;
}

void editor::io::PreferencesManager::Dump() {
    _instance->savePreferences();
}

editor::io::PreferencesManager::~PreferencesManager() {
    SDL_free(_currentDirectory);
    _currentDirectory = nullptr;
    _preferences.abandon();
}

void editor::io::PreferencesManager::savePreferences() {
    if(!_preferences.empty()) {
        LuaManager::GetInstance().writeToFile(_preferences, std::filesystem::path(std::string(_currentDirectory) + _preferencesPath.string()).lexically_normal().string());
    }
}