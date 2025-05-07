//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_MAINWINDOW_H
#define RPGBAKER_MAINWINDOW_H

#include <sol/sol.hpp>

#include "render/Window.h"

namespace editor {
    class Project;
}

namespace editor::render::tabs {
    class MapEditor;
    class EventEditor;
    class MapConnections;
    class PlayerSettings;
    class GeneralSettings;
    class SpriteAnimViewer;
}

namespace editor::render::windows {
    class MainWindow : public Window {
    public:
        MainWindow(editor::Project* project);
    private:
        editor::Project* _project;

        editor::render::tabs::MapEditor* _mapEditor = nullptr;
        editor::render::tabs::EventEditor* _eventEditor = nullptr;
        editor::render::tabs::MapConnections* _mapConnections = nullptr;
        editor::render::tabs::PlayerSettings* _playerSettings = nullptr;
        editor::render::tabs::GeneralSettings* _generalSettings = nullptr;
        editor::render::tabs::SpriteAnimViewer* _spriteEditor = nullptr;

        void beforeRender() override;

        void onRender() override;

        void build(const std::string &platform);
    };
};


#endif //RPGBAKER_MAINWINDOW_H
