//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Project.h"
#include "io/LuaManager.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <sol/sol.hpp>

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
            _found = true;
            sol::table T = io::LuaManager::GetInstance().getTable((_projectPath / ("ProjectSettings.lua")).string());
            loadProject(T);
        }
    }
}

void editor::Project::loadProject(const sol::table& project) {
    if(project.valid()) {
        if(project["Name"].valid()) {
            setName(project["Name"].get<std::string>());
        }

        if(project["LastModified"].valid()) {
            std::istringstream ss(project["LastModified"].get<std::string>());
            ss >> std::get_time(&_lastModified, "%d/%m/%Y %H:%M:%S");
        }

        if(project["AdditionalRoutes"].valid()) {
            sol::table additionalRoutes = project["AdditionalRoutes"];
            for(const auto& [key, value] : additionalRoutes) {
                if(value.is<std::string>()) {
                    _extraRoutes.insert(value.as<std::string>());
                }
            }
        }
    }
}

bool editor::Project::isSetToDelete() const {
    return _setToDelete;
}

void editor::Project::setToDelete() {
    _setToDelete = true;
}

bool editor::Project::isFound() const {
    return _found;
}

void editor::Project::saveProject() {
    if(_found) {
        sol::state& L = io::LuaManager::GetInstance().getState();

        sol::table pr = L.create_table();

        std::ostringstream oss;
        oss << std::put_time(&_lastModified, "%d/%m/%Y %H:%M:%S");

        pr["Name"] = _name;
        pr["Route"] = "";
        pr["LastModified"] = oss.str();
        int index = 1;
        for(auto elem : _extraRoutes) {
            pr["AdditionalRoutes"][index++] = elem.string();
        }

        io::LuaManager::GetInstance().writeToFile(pr, (_projectPath / ("ProjectSettings.lua")).string());
    }
}
