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

    if(!openProjectsFile()) return false;
    loadProjects();
    return true;
}

bool editor::io::ProjectManager::openProjectsFile() {
    if(!LuaManager::GetInstance().loadFile(std::filesystem::path(std::string(_currentDirectory) + _projectsPath.string()).lexically_normal().string())) return false;
    return true;
}

void editor::io::ProjectManager::loadProjects() {
    sol::state& L = LuaManager::GetInstance().getLuaState();
    sol::table projects = L["projects"];
    if(projects.valid()) {
        for(const auto& [index, value] : projects) {
            if(value.is<std::string>()) {
                std::string route = value.as<std::string>();
                _projects.push_back(new Project(route));
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
