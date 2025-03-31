//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_MAINWINDOW_H
#define MOTORSINNOMBREDECIDIDO_MAINWINDOW_H

#include "render/Window.h"

namespace editor {
    class Project;
}

namespace editor::render::windows {
    class MainWindow : public Window {
    public:
        MainWindow(editor::Project* project);
    private:
        void beforeRender() override;

        void onRender() override;
    };
};


#endif //MOTORSINNOMBREDECIDIDO_MAINWINDOW_H
