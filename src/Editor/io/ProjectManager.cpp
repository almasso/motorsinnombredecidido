//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ProjectManager.h"
#include <cassert>
#include "LuaManager.h"
#include <SDL3/SDL_filesystem.h>
#include <iostream>
#include "common/Project.h"
#include <sol/sol.hpp>
#include <cctype>
#include "utils/tinyfiledialogs/tinyfiledialogs.h"

std::unique_ptr<editor::io::ProjectManager> editor::io::ProjectManager::_instance = nullptr;

bool editor::io::ProjectManager::Init() {
    assert(_instance == nullptr && "Project manager singleton instance is already initialized || La instancia del singleton del gestor de proyectos ya está inicializada");
    _instance = std::unique_ptr<ProjectManager>(new ProjectManager());
    if(!_instance->init()) {
        _instance.reset(nullptr);
        return false;
    }
    return true;
}

bool editor::io::ProjectManager::init() {
    _projectsPath = "/settings/projects/projects.lua";
    _currentDirectory = SDL_GetCurrentDirectory();
    if(_currentDirectory == nullptr) return false;

    loadProjects();
    return true;
}

void editor::io::ProjectManager::loadProjects() {
    sol::table projects = LuaManager::GetInstance().getTable(std::filesystem::path(std::string(_currentDirectory) + _projectsPath.string()).lexically_normal().string());
    if(projects.valid()) {
        for(const auto& [index, value] : projects) {
            if(value.is<std::string>()) {
                std::string route = value.as<std::string>();
                if(!projectAlreadyIncluded(route)) _projects.push_back(new Project(route));
            }
        }
    }
}

editor::io::ProjectManager &editor::io::ProjectManager::GetInstance() {
    return *_instance;
}

editor::io::ProjectManager::~ProjectManager() {
    SDL_free(_currentDirectory);
    _currentDirectory = nullptr;
}

uint32_t editor::io::ProjectManager::getProjectCount() const {
    return _projects.size();
}

const SearchableList<editor::Project*>& editor::io::ProjectManager::getProjects() const {
    return _projects;
}

void editor::io::ProjectManager::_addProject(const std::string &route) {
    std::filesystem::path p(route);
    if(!projectAlreadyIncluded(p.parent_path().string())) _projects.push_back(new Project(p.parent_path().string()));
    saveProjects();
}

void editor::io::ProjectManager::saveProjects() const {
    sol::state& L = LuaManager::GetInstance().getState();

    sol::table projectsTable = L.create_table();
    int index = 1;
    for(Project* p : _projects) {
        std::string s = p->getPath().string();
        projectsTable[index++] = s;
    }

    LuaManager::GetInstance().writeToFile(projectsTable, std::filesystem::path(std::string(_currentDirectory) + _projectsPath.string()).lexically_normal().string());
}

bool editor::io::ProjectManager::projectAlreadyIncluded(const std::string &route) const {
    return std::ranges::any_of(_projects, [&](Project* p) {
        return p->getPath().string() == route;
    });
}


