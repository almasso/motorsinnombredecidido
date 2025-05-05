//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_WELCOMEWINDOWRIGHTPANEL_H
#define RPGBAKER_WELCOMEWINDOWRIGHTPANEL_H

#include "render/Subwindow.h"
#include <unordered_map>

namespace editor {
    class Project;
}

namespace editor::render::modals {
    class DeleteProjectModal;
    class RenameProjectModal;
    class CreateProjectModal;
}

namespace editor::render::windows {
    class MainWindow;
}

namespace editor::render::subwindows {
    class WelcomeWindowRightPanel : public Subwindow {
    public:
        WelcomeWindowRightPanel(std::unordered_map<Project*, editor::render::modals::DeleteProjectModal*>* deleteProjects,
                                std::unordered_map<Project*, editor::render::modals::RenameProjectModal*>* renameProjects);

        ~WelcomeWindowRightPanel() override;
    private:
        std::unordered_map<Project*, editor::render::modals::DeleteProjectModal*>* _deleteProjects;
        std::unordered_map<Project*, editor::render::modals::RenameProjectModal*>* _renameProjects;

        bool _showDeleteConfirmation = false;

        bool _showRenameProject = false;

        bool _hasChangedWindow = false;

        editor::render::windows::MainWindow* _mainWindow = nullptr;

        void beforeRender() override;

        void onRender() override;

        void drawProjectButton(editor::Project* project);

        std::string searchProject();
    };
}


#endif //RPGBAKER_WELCOMEWINDOWRIGHTPANEL_H
