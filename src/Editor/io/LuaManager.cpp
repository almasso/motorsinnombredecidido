//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "LuaManager.h"
#include <cassert>
#include <lua.hpp>
#include <sol/sol.hpp>

std::unique_ptr<editor::io::LuaManager> editor::io::LuaManager::_instance = nullptr;

bool editor::io::LuaManager::Init() {
    assert(_instance == nullptr && "Lua manager singleton instance is already initialized || La instancia del singleton del gestor de Lua ya está inicializada");
    _instance = std::unique_ptr<LuaManager>(new LuaManager());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

bool editor::io::LuaManager::init() {
    _luaState = std::make_unique<sol::state>();
    if(!_luaState) return false;

    _luaState->open_libraries(sol::lib::base, sol::lib::package);
    return true;
}

editor::io::LuaManager &editor::io::LuaManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::io::LuaManager::~LuaManager() {
    _luaState.reset(nullptr);
}

sol::state& editor::io::LuaManager::getLuaState() const {
    return *_luaState;
}

bool editor::io::LuaManager::_loadFile(const std::string &filename) {
    try {
        _luaState->do_file(filename);
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}
