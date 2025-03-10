//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Project.h"
#include "io/LuaManager.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <lua.hpp>

const std::string &editor::Project::getName() const {
    return _name;
}

const std::filesystem::path &editor::Project::getPath() const {
    return _projectPath;
}

const std::unordered_set<std::filesystem::path> &editor::Project::getExtraRoutes() const {
    return _extraRoutes;
}

const std::tm &editor::Project::getLastModificationTime() const {
    return _lastModified;
}

void editor::Project::updateLastModified() {
    time_t now = time(nullptr);
    _lastModified = *localtime(&now);
}

void editor::Project::findProject() {
    if(std::filesystem::exists(_projectPath)) {
        // La carpeta del proyecto existe, vamos a buscar el archivo de configuración del proyecto
        if(std::filesystem::exists(_projectPath / ("ProjectSettings.lua"))) {
            io::LuaManager::GetInstance().loadFile((_projectPath / ("ProjectSettings.lua")).string());
            loadProject();
        }
    }
}

void editor::Project::loadProject() {
    lua_State* L = io::LuaManager::GetInstance().getLuaState();
    lua_getglobal(L, "project");
    if(lua_istable(L, -1)) {
        lua_getfield(L, -1, "Name");
        setName(lua_tostring(L, -1));
        lua_pop(L, 1);

        // Nos saltamos la ruta
        lua_pop(L, 1);

        lua_getfield(L, -1, "LastModified");
        std::istringstream ss(lua_tostring(L, -1));
        ss >> std::get_time(&_lastModified, "%d/%m/%Y %H:%M:%S");
        lua_pop(L,1);

        lua_getfield(L, -1, "AdditionalRoutes");
        if(lua_istable(L, -1)) {
            lua_pushnil(L);
            while(lua_next(L, -2)) {
                if(lua_isstring(L, -1)) {
                    _extraRoutes.insert(lua_tostring(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}
