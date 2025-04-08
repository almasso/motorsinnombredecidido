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
#include "resources/Tileset.h"
#include "resources/Map.h"

void editor::Project::initResources() {
    std::filesystem::path mapsPath = (_projectPath / "projectfiles" / "maps");
    resources::Map::SetMapsDirectory(mapsPath);
    for (auto const& file : std::filesystem::directory_iterator(mapsPath)) {
        auto map = new resources::Map(this);
        auto name = file.path().stem().string();
        if (map->readFromLua(name))
            _maps.insert({name, map});
        else
            delete map;
    }

    std::filesystem::path tilesetsPath = (_projectPath / "projectfiles" / "tilesets");
    resources::Tileset::SetTilesetsDirectory(tilesetsPath);
    for (auto const& file : std::filesystem::directory_iterator(tilesetsPath)) {
        auto tileset = new resources::Tileset(this);
        auto name = file.path().stem().string();
        if (tileset->readFromLua(name))
            _tilesets.insert({name, tileset});
        else
            delete tileset;
    }
}

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

        if(project["Dimensions"].valid()) {
            _dimensions[0] = project["Dimensions"][1];
            _dimensions[1] = project["Dimensions"][2];
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
        pr["Dimensions"] = L.create_table();
        pr["Dimensions"][1] = _dimensions[0];
        pr["Dimensions"][2] = _dimensions[1];
        int index = 1;
        pr["AdditionalRoutes"] = L.create_table();
        for(auto elem : _extraRoutes) {
            pr["AdditionalRoutes"][index++] = elem.string();
        }

        if(!std::filesystem::exists(_projectPath / "assets/")) std::filesystem::create_directories(_projectPath / "assets");
        if(!std::filesystem::exists(_projectPath / "projectfiles/")) std::filesystem::create_directories(_projectPath / "projectfiles");
        if(!std::filesystem::exists(_projectPath / "bin/")) std::filesystem::create_directories(_projectPath / "bin");
        if(!std::filesystem::exists(_projectPath / "projectfiles/maps/")) std::filesystem::create_directories(_projectPath / "projectfiles/maps");
        if(!std::filesystem::exists(_projectPath / "projectfiles/tilesets/")) std::filesystem::create_directories(_projectPath / "projectfiles/tilesets");

        io::LuaManager::GetInstance().writeToFile(pr, (_projectPath / ("ProjectSettings.lua")).string());
        for (auto& [key, tileset] : _tilesets)
            tileset->writeToLua();
        for (auto& [key, map] : _maps)
            map->writeToLua();
    }
}

const int *editor::Project::getDimensions() const {
    return _dimensions;
}

void editor::Project::addMap(editor::resources::Map *map) {
    _maps[map->getName()] = map;
    map->writeToLua();
}

void editor::Project::addTileset(editor::resources::Tileset *tileset) {
    _tilesets[tileset->getName()] = tileset;
    tileset->writeToLua();
}

int editor::Project::totalMaps() const {
    return _maps.size();
}

int editor::Project::totalTilesets() const {
    return _tilesets.size();
}

const std::unordered_map<std::string, editor::resources::Map*>& editor::Project::getMaps() const {
    return _maps;
}

const std::unordered_map<std::string, editor::resources::Tileset*>& editor::Project::getTilesets() const {
    return _tilesets;
}

std::filesystem::path editor::Project::getAssetsPath() const {
    return _projectPath / "assets";
}
