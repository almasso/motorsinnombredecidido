//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "PreferencesManager.h"
#include <cassert>
#include <SDL3/SDL_filesystem.h>
#include "LuaManager.h"

std::unique_ptr<editor::io::PreferencesManager> editor::io::PreferencesManager::_instance = nullptr;

bool editor::io::PreferencesManager::Init() {
    assert(_instance == nullptr && "Preferences manager singleton instance is already initialized || La instancia del singleton del gestor de preferencias ya está inicializada");
    _instance = std::unique_ptr<PreferencesManager>(new PreferencesManager());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

bool editor::io::PreferencesManager::init() {
    _preferencesPath = "/settings/preferences/userpreferences.lua";
    _currentDirectory = SDL_GetCurrentDirectory();
    if(_currentDirectory == nullptr) return false;

    loadPreferences();
    return true;
}

void editor::io::PreferencesManager::loadPreferences() {
    _preferences = LuaManager::GetInstance().getState().create_table();
    if(std::filesystem::exists(std::filesystem::path(std::string(_currentDirectory) + _preferencesPath.string()).lexically_normal().string())) {
        _preferences = LuaManager::GetInstance().getTableFromScript(std::filesystem::path(std::string(_currentDirectory) + _preferencesPath.string()).lexically_normal().string().c_str());
    }
}

editor::io::PreferencesManager &editor::io::PreferencesManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

void editor::io::PreferencesManager::Dump() {
    _instance->savePreferences();
}

editor::io::PreferencesManager::~PreferencesManager() {
    SDL_free(_currentDirectory);
    _currentDirectory = nullptr;
}

void editor::io::PreferencesManager::savePreferences() {
    if(!_preferences.empty()) {
        LuaManager::GetInstance().writeToFile(_preferences, std::filesystem::path(std::string(_currentDirectory) + _preferencesPath.string()).lexically_normal().string());
    }
}