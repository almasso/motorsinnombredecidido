//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_WELCOMEWINDOW_H
#define RPGBAKER_WELCOMEWINDOW_H

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
    class SettingsModal;
}

namespace editor::render::subwindows {
    class WelcomeWindowLeftPanel;
    class WelcomeWindowRightPanel;
    class WelcomeWindowProjectManagementButtons;
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
        editor::render::modals::SettingsModal* _settings = nullptr;
        editor::render::subwindows::WelcomeWindowLeftPanel* _leftPanel = nullptr;
        editor::render::subwindows::WelcomeWindowRightPanel* _rightPanel = nullptr;
        editor::render::subwindows::WelcomeWindowProjectManagementButtons* _projectManagementButtons = nullptr;

        void onRender() override;

        void beforeRender() override;

        void setFullscreen();

        void drawWindow();

        void createModals();

        void createSubwindows();
    };
}

#endif //RPGBAKER_WELCOMEWINDOW_H
