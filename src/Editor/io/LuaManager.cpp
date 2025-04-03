//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "LuaManager.h"
#include "common/EditorError.h"
#include <filesystem>
#include <fstream>
#include <SDL3/SDL_filesystem.h>

#ifdef __APPLE__
#define GetCurrentDir strdup(SDL_GetBasePath())
#else
#define GetCurrentDir SDL_GetCurrentDirectory()
#endif
std::unique_ptr<editor::io::LuaManager> editor::io::LuaManager::_instance = nullptr;

bool editor::io::LuaManager::Init() {
    editorAssert(_instance == nullptr, "Lua manager singleton instance is already initialized")
    _instance = std::unique_ptr<LuaManager>(new LuaManager());
    if(!_instance->init()) {
        Destroy();
        return false;
    }
    return true;
}

bool editor::io::LuaManager::init() {
    _state = std::make_unique<sol::state>();
    if(!_state) {
        showError("Couldn't create a sol2 state")
        return false;
    }

    _currentDirectory = GetCurrentDir;
    if(_currentDirectory == nullptr) {
        showError(SDL_GetError())
        return false;
    }

    _state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::string, sol::lib::io);
    return true;
}

void editor::io::LuaManager::Destroy() {
    _instance.reset(nullptr);
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
    if(!res.valid()) {
        showWarning("Loaded file " + filename + " was not valid")
        return sol::lua_nil;
    }
    sol::protected_function_result res2 = res();
    if(!res2.valid()) {
        showWarning("Loaded file " + filename + " was not valid")
        return sol::lua_nil;
    }
    sol::table table = res2.get<sol::table>();
    if(!table.valid()) {
        showWarning("Couldn't create a sol table from " + filename)
        return sol::lua_nil;
    }
    return table;
}

sol::table editor::io::LuaManager::_getTableFromScript(const std::string &filename) {
    sol::protected_function_result res2 = _state->script_file(filename);
    if(!res2.valid()) {
        sol::error err = res2;
        showWarning(err.what())
        return sol::lua_nil;
    }
    sol::table table = res2.get<sol::table>();
    if(!table.valid()) {
        showWarning("Couldn't create a sol table from " + filename)
        return sol::lua_nil;
    }
    return table;
}

void editor::io::LuaManager::_writeToFile(const sol::table &table, const std::string &filename) {
    sol::table serpent = getTableFromScript(std::filesystem::path(std::string(_currentDirectory) + "/settings/serializer/serpent.lua").lexically_normal().string());

    if(!std::filesystem::exists(filename)) std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    std::ofstream file(filename);

    if(!serpent.valid()) {
        showError("Couldn't load file serializer")
        return;
    }

    auto block = serpent["block"];
    if(!block.valid()) {
        sol::error err = block;
        showError(err.what())
        return;
    }

    auto result = block(table);
    if(!result.valid()) {
        sol::error err = result;
        showError(err.what())
        return;
    }

    std::string serializedData = result.get<std::string>();
    if(!file.is_open()) {
        showError("Couldn't open file: " + filename)
        return;
    }
    file << "return " << serializedData;
    file.close();
}
