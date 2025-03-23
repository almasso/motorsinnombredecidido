//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H
#define MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H

#include <ctime>
#include "render/Window.h"

struct ImVec2;

namespace editor::render::windows {
    class WelcomeWindow : public Window {
    public:
        WelcomeWindow();
    private:
        void onRender() override;

        void beforeRender() override;

        void setFullscreen();

        void drawWindow();

        void drawProjectButton(const std::string& projectName, const std::string& projectRoute, const std::tm& lastModified);
    };
}

#endif //MOTORSINNOMBREDECIDIDO_WELCOMEWINDOW_H
