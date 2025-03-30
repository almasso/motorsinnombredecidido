//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H
#define MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H

#include <ctime>
#include <unordered_map>
#include "render/Window.h"

struct ImVec2;

namespace editor {
    class Project;
}

namespace editor::render::modals {
    class DeleteProjectModal;
    class RenameProjectModal;
    class CreateProjectModal;
}

namespace editor::render::windows {
    class WelcomeWindow : public Window {
    public:
        WelcomeWindow();

        ~WelcomeWindow() override;
    private:
        std::unordered_map<Project*, editor::render::modals::DeleteProjectModal*> _deleteProjects;
        std::unordered_map<Project*, editor::render::modals::RenameProjectModal*> _renameProjects;
        editor::render::modals::CreateProjectModal* _createProject = nullptr;

        bool _showDeleteConfirmation = false;

        bool _showRenameProject = false;

        bool _showCreateProject = false;

        void onRender() override;

        void beforeRender() override;

        void setFullscreen();

        void drawWindow();

        void createModals();

        std::string searchProject();

        void drawProjectButton(Project* project);
    };
}

#endif //MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H
