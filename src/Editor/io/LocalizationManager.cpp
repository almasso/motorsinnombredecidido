//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "LocalizationManager.h"
#include <cassert>

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

editor::io::LocalizationManager &editor::io::LocalizationManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::io::LocalizationManager::~LocalizationManager() = default;

bool editor::io::LocalizationManager::init() {
    return true;
}