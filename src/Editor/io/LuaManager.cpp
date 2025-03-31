//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "LuaManager.h"
#include "common/EditorError.h"
#include <filesystem>
#include <fstream>
#include <SDL3/SDL_filesystem.h>

std::unique_ptr<editor::io::LuaManager> editor::io::LuaManager::_instance = nullptr;

bool editor::io::LuaManager::Init() {
    editorAssert(_instance == nullptr, "Lua manager singleton instance is already initialized")
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
    _currentDirectory = SDL_GetCurrentDirectory();

    _state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::string, sol::lib::io);
    return true;
}

editor::io::LuaManager &editor::io::LuaManager::GetInstance() {
    editorAssert(_instance != nullptr, "Lua manager singleton instance is not initialized")
    return *_instance;
}

editor::io::LuaManager::~LuaManager() {
    SDL_free(_currentDirectory);
    _currentDirectory = nullptr;
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

void editor::io::LuaManager::_writeToFile(const sol::table &table, const std::string &filename) {
    sol::table serpent = getTableFromScript(std::filesystem::path(std::string(_currentDirectory) + "/settings/serializer/serpent.lua").lexically_normal().string());

    if(!std::filesystem::exists(filename)) std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    std::ofstream file(filename);

    if(serpent.valid()) {
        auto block = serpent["block"];
        if(block.valid()) {
            auto result = block(table);
            if(result.valid()) {
                std::string serializedData = result.get<std::string>();
                if(file.is_open()) {
                    file << "return " << serializedData;
                    file.close();
                }
            }
        }
    }
}
