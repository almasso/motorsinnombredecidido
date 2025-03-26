//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H
#define MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H

#include <ctime>
#include "render/Window.h"

struct ImVec2;

namespace editor {
    class Project;
}

namespace editor::render::windows {
    class WelcomeWindow : public Window {
    public:
        WelcomeWindow();
    private:
        bool _showDeleteConfirmation = false;
        bool _showRenameProject = false;
        bool _isRenaming = false;
        char _nameBuffer[256];

        void onRender() override;

        void beforeRender() override;

        void setFullscreen();

        void drawWindow();

        void newProjectModal();

        std::string searchProject();

        void drawProjectButton(Project* project);
    };
}

#endif //MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H
