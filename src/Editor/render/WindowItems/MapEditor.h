//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_MAPEDITOR_H
#define MOTORSINNOMBREDECIDIDO_MAPEDITOR_H

#include "render/WindowItem.h"
#include <vector>
#include <imgui.h>

namespace editor {
    class Project;
}

namespace editor::render::subwindows {
    class MapEditor : public WindowItem {
    public:
        MapEditor(editor::Project* project);

        ~MapEditor() override;
    private:
        editor::Project* _project = nullptr;

        std::vector<ImTextureID> _textures;
        std::vector<std::string> _buttonTooltips;

        float _tileSize = 32.0f;
        float _offsetX = 0.0f;
        float _offsetY = 0.0f;
        float _zoom = 1.0f;
        int _selectedMode = 0;

        void beforeRender() override;

        void onRender() override;

        void drawGrid();

        void drawToolbar();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_MAPEDITOR_H
