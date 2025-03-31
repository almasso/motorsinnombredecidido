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
#include <sol/sol.hpp>

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
    auto locale = PreferencesManager::GetInstance().getPreference<std::string>("preferredLocale");
    if(locale.has_value()) {
        _preferredLocale = locale.value();
    }
    _languageTemplatesRoute = "/settings/lang";
    loadLocales();
    convertLocalesIntoTable();
    countLocales();
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
    _stringsTable.clear();
    sol::table localization = LuaManager::GetInstance().getTable(std::filesystem::path(std::filesystem::path(std::string(_currentDirectory) + _languageTemplatesRoute.string()).lexically_normal() / (_preferredLocale + ".lua")).string());
    if(localization.valid()) {
        for(const auto& [key, value] : localization) {
            if(key.is<std::string>() && value.is<std::string>()) {
                _stringsTable[key.as<std::string>()] = value.as<std::string>();
            }
        }
    }
}

void editor::io::LocalizationManager::countLocales() {
    for(const auto& file : std::filesystem::directory_iterator(std::filesystem::path(std::string(_currentDirectory) + _languageTemplatesRoute.string()).lexically_normal())) {
        if(file.is_regular_file()) {
            _languagesDetected.insert(file.path().stem().string());
        }
    }
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

int editor::io::LocalizationManager::totalLanguagesDetected() const {
    return _languagesDetected.size();
}

const std::unordered_set<std::string> &editor::io::LocalizationManager::getAllLanguages() const {
    return _languagesDetected;
}


