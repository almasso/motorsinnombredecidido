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

        static ProjectManager& GetInstance();

        uint32_t getProjectCount() const;

        const SearchableList<Project*>& getProjects() const;

        ProjectManager(const ProjectManager &) = delete;

        ProjectManager &operator=(const ProjectManager &) = delete;

        ~ProjectManager();
    private:
        static std::unique_ptr<ProjectManager> _instance;

        std::filesystem::path _projectsPath;

        char* _currentDirectory = nullptr;

        SearchableList<Project*> _projects;

        ProjectManager() = default;

        bool init();

        bool openProjectsFile();

        void loadProjects();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_PROJECTMANAGER_H
