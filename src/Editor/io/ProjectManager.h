//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_PROJECTMANAGER_H
#define MOTORSINNOMBREDECIDIDO_PROJECTMANAGER_H

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
        void addProject(T&& route) {
            _addProject(std::forward<std::string>(route));
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

        void _addProject(const std::string& route);

        void saveProjects() const;

        bool projectAlreadyIncluded(const std::string& route) const;

        void deleteOldProjects();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_PROJECTMANAGER_H
