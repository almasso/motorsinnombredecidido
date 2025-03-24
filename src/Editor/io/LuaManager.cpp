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
    _state = std::make_unique<sol::state>();
    if(!_state) return false;

    _state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
    return true;
}

editor::io::LuaManager &editor::io::LuaManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::io::LuaManager::~LuaManager() {
    _state.reset(nullptr);
}

sol::state& editor::io::LuaManager::getState() const {
    return *_state;
}

sol::table editor::io::LuaManager::_getTable(const std::string &filename) {
    sol::load_result res = _state->load_file(filename);
    if(!res.valid()) return nullptr;
    sol::protected_function_result res2 = res();
    if(!res2.valid()) return nullptr;
    sol::table table = res2.get<sol::table>();
    if(!table.valid()) return nullptr;
    return table;
}

sol::table editor::io::LuaManager::_getTableFromScript(const std::string &filename) {
    sol::protected_function_result res2 = _state->script_file(filename);
    if(!res2.valid()) return nullptr;
    sol::table table = res2.get<sol::table>();
    if(!table.valid()) return nullptr;
    return table;
}

std::string editor::io::LuaManager::serializeTable(const sol::table& table) {
    std::string result = "{\n";
    for (const auto& pair : table) {
        sol::object key = pair.first;
        sol::object value = pair.second;

        if (key.is<std::string>()) {
            result += "\t" + key.as<std::string>() + " = ";
        }

        if (value.is<std::string>()) {
            result += "\"" + value.as<std::string>() + "\",\n";
        } else if (value.is<int>()) {
            result += std::to_string(value.as<int>()) + ",\n";
        } else if (value.is<double>()) {
            result += std::to_string(value.as<double>()) + ",\n";
        } else if (value.is<bool>()) {
            result += std::string(value.as<bool>() ? "true" : "false") + ",\n";
        } else if (value.is<sol::table>()) {
            result += serializeTable(value.as<sol::table>()) + ",\n";
        }
    }
    result += "}";
    return result;
}
