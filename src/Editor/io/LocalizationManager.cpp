//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "LocalizationManager.h"
#include <cassert>
#include "LuaManager.h"
#include <iostream>
#include <SDL3/SDL_locale.h>
#include <SDL3/SDL_filesystem.h>

std::unique_ptr<editor::io::LocalizationManager> editor::io::LocalizationManager::_instance = nullptr;

bool editor::io::LocalizationManager::Init() {
    assert(_instance == nullptr && "Resource manager singleton instance is already initialized || La instancia del singleton del gestor de recursos ya está inicializada");
    _instance = std::unique_ptr<LocalizationManager>(new LocalizationManager());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

bool editor::io::LocalizationManager::init() {
    _locales = SDL_GetPreferredLocales(&_totalUserLocales);
    if(_locales == nullptr) return false;
    _currentDirectory = SDL_GetCurrentDirectory();
    if(_currentDirectory == nullptr) return false;
    _preferredLocale = std::string(_locales[0]->language) + "_" + std::string(_locales[0]->country);
    _languageTemplatesRoute = "/settings/lang";
    loadLocales();
    convertLocalesIntoTable();
    return true;
}

void editor::io::LocalizationManager::loadLocales() {
    // Buscamos si existe el .lua con la tabla de idiomas del locale preferido del usuario
    // We search if the .lua file with the languages table from the preferred user locale exists
    if(!std::filesystem::exists(std::filesystem::path(std::string(_currentDirectory) + _languageTemplatesRoute.string()).lexically_normal() / (_preferredLocale + ".lua"))) {
        // Si no existe, buscamos primero por un archivo que sea del mismo idioma
        // If it doesn't exist, we search for a file with the same language
        if(!searchForSecondaryLocales())
            // Si tampoco existe, ponemos el en_US por defecto
            // If it doesn't exist, we default to en_US
            _preferredLocale = "en_US";
    }
    LuaManager::GetInstance().loadFile(std::filesystem::path(std::filesystem::path(std::string(_currentDirectory) + _languageTemplatesRoute.string()).lexically_normal() / (_preferredLocale + ".lua")).string());
}

bool editor::io::LocalizationManager::searchForSecondaryLocales() {
    for(int i = 0; i < _totalUserLocales; ++i) {
        for(const auto& file : std::filesystem::directory_iterator(std::filesystem::path(std::string(_currentDirectory) + _languageTemplatesRoute.string()).lexically_normal())) {
            if(file.is_regular_file()) {
                std::string filename = file.path().filename().stem().string();
                if(filename.rfind(_locales[i]->language, 0) == 0) {
                    _preferredLocale = filename;
                    return true;
                }
            }
        }
    }
    return false;
}

void editor::io::LocalizationManager::convertLocalesIntoTable() {
    lua_State* L = LuaManager::GetInstance().getLuaState();
    lua_getglobal(L, "localization");
    if(lua_istable(L, -1)) {
        lua_pushnil(L);
        while(lua_next(L, -2) != 0) {
            if(lua_isstring(L, -2) && lua_isstring(L, -1)) {
                _stringsTable[lua_tostring(L, -2)] = lua_tostring(L, -1);
            }
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
}

editor::io::LocalizationManager &editor::io::LocalizationManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::io::LocalizationManager::~LocalizationManager() {
    SDL_free(_locales);
    _locales = nullptr;
    SDL_free(_currentDirectory);
    _currentDirectory = nullptr;
}

