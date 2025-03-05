//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "LuaManager.h"
#include <cassert>

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
    _luaState = luaL_newstate();
    if(_luaState == nullptr) return false;
    luaL_openlibs(_luaState);
    return true;
}

editor::io::LuaManager &editor::io::LuaManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::io::LuaManager::~LuaManager() {
    lua_close(_luaState);
    _luaState = nullptr;
}

lua_State* editor::io::LuaManager::getLuaState() const {
    return _luaState;
}
