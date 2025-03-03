//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ResourceManager.h"
#include <cassert>

std::unique_ptr<editor::io::ResourceManager> editor::io::ResourceManager::_instance = nullptr;

bool editor::io::ResourceManager::Init() {
    assert(_instance == nullptr && "Resource manager singleton instance is already initialized || La instancia del singleton del gestor de recursos ya está inicializada");
    _instance = std::unique_ptr<ResourceManager>(new ResourceManager());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

editor::io::ResourceManager &editor::io::ResourceManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::io::ResourceManager::~ResourceManager() = default;

bool editor::io::ResourceManager::init() {
    return true;
}
