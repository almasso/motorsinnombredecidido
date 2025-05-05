//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_PROJECTMANAGER_H
#define RPGBAKER_PROJECTMANAGER_H

#include <memory>
#include <filesystem>
#include "utils/SearchableList.h"

namespace editor {
    class Project;
}

namespace editor::io {
    class ProjectManager {
    public:
        static bool Init();

        static void Destroy();

        static ProjectManager& GetInstance();

        static void Dump();

        uint32_t getProjectCount() const;

        const SearchableList<Project*>& getProjects() const;

        template <std::convertible_to<std::string> T>
        Project* addProject(T&& route) {
            return _addProject(std::forward<T>(route));
        }

        template <std::convertible_to<std::string> T>
        Project* getProject(T&& route) {
            return _getProject(std::forward<T>(route));
        }

        void addProject(editor::Project* project);

        void removeProject(editor::Project* project);

        ProjectManager(const ProjectManager &) = delete;

        ProjectManager &operator=(const ProjectManager &) = delete;

        ~ProjectManager();
    private:
        static std::unique_ptr<ProjectManager> _instance;

        std::filesystem::path _projectsPath;

        char* _currentDirectory = nullptr;

        Project* _setToDeleteProject = nullptr;

        SearchableList<Project*> _projects;

        ProjectManager() = default;

        bool init();

        void loadProjects();

        Project* _addProject(const std::string& route);

        Project* _getProject(const std::string& route);

        void saveProjects() const;

        bool projectAlreadyIncluded(const std::string& route) const;

        void deleteOldProjects();
    };
}


#endif //RPGBAKER_PROJECTMANAGER_H
