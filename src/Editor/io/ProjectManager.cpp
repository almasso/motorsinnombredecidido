//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ProjectManager.h"
#include "common/EditorError.h"
#include "LuaManager.h"
#include <SDL3/SDL_filesystem.h>
#include "common/Project.h"
#include <sol/sol.hpp>

std::unique_ptr<editor::io::ProjectManager> editor::io::ProjectManager::_instance = nullptr;

bool editor::io::ProjectManager::Init() {
    editorAssert(_instance == nullptr, "Project manager singleton instance is already initialized")
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
            sol::table pr = value.as<sol::table>();
            if(pr.valid()) {
                std::string route = pr["Route"].get<std::string>();
                std::string provName = pr["Name"].get<std::string>();
                if(!projectAlreadyIncluded(route)) {
                    Project* nP = new Project(route);
                    if(!nP->isFound()) nP->setName(provName);
                    _projects.push_back(nP);
                }
            }
        }
    }
}

editor::io::ProjectManager &editor::io::ProjectManager::GetInstance() {
    editorAssert(_instance != nullptr, "Project manager singleton instance is not initialized")
    return *_instance;
}

void editor::io::ProjectManager::Dump() {
    _instance->saveProjects();
}

editor::io::ProjectManager::~ProjectManager() {
    SDL_free(_currentDirectory);
    _currentDirectory = nullptr;
    for(Project* p : _projects) {
        delete p;
        p = nullptr;
    }
    _projects.clear();
    _setToDeleteProject = nullptr;
}

uint32_t editor::io::ProjectManager::getProjectCount() const {
    return _projects.size();
}

const SearchableList<editor::Project*>& editor::io::ProjectManager::getProjects() const {
    return _projects;
}

void editor::io::ProjectManager::_addProject(const std::string &route) {
    deleteOldProjects();
    std::filesystem::path p(route);
    if(!projectAlreadyIncluded(p.parent_path().string())) _projects.push_back(new Project(p.parent_path().string()));
    saveProjects();
}

void editor::io::ProjectManager::addProject(editor::Project *project) {
    if(!projectAlreadyIncluded(project->getPath().string())) _projects.push_back(project);
    saveProjects();
}

void editor::io::ProjectManager::saveProjects() const {
    sol::state& L = LuaManager::GetInstance().getState();

    sol::table projectsTable = L.create_table();
    int index = 1;
    for(Project* p : _projects) {
        if(!p->isSetToDelete()) {
            std::string s = p->getPath().string();
            projectsTable[index] = L.create_table();
            projectsTable[index]["Route"] = s;
            projectsTable[index]["Name"] = p->getName();
            index++;
        }
    }

    LuaManager::GetInstance().writeToFile(projectsTable, std::filesystem::path(std::string(_currentDirectory) + _projectsPath.string()).lexically_normal().string());
}

bool editor::io::ProjectManager::projectAlreadyIncluded(const std::string &route) const {
    return std::ranges::any_of(_projects, [&](Project* p) {
        return p->getPath().string() == route;
    });
}

void editor::io::ProjectManager::removeProject(editor::Project *project) {
    deleteOldProjects();
    _setToDeleteProject = *(_projects.find(project));
    _setToDeleteProject->setToDelete();
    saveProjects();
}

void editor::io::ProjectManager::deleteOldProjects() {
    if(_setToDeleteProject != nullptr) _projects.erase(_setToDeleteProject);
    delete _setToDeleteProject;
    _setToDeleteProject = nullptr;
}


