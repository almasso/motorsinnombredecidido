//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_MAPEDITOR_H
#define MOTORSINNOMBREDECIDIDO_MAPEDITOR_H

#include "render/WindowItem.h"
#include <vector>
#include <deque>
#include <imgui.h>

namespace editor {
    class Project;
}

namespace editor::render::modals {
    class TilesetWizard;
}

namespace editor::resources {
    class Tileset;
    class Map;
}

namespace editor::render::tabs {
    class MapEditor : public WindowItem {
    public:
        MapEditor(editor::Project* project);

        ~MapEditor() override;
    private:
        enum GridDrawingMode {
            DRAW_SELECTED_LAYER_ONLY = 0,
            DRAW_SELECTED_LAYER_BELOW_TRANSPARENT = 1,
            DRAW_SELECTED_LAYER_BELOW_OPAQUE = 2,
            DRAW_ALL = 3
        };

        editor::Project* _project = nullptr;

        std::vector<ImTextureID> _uiTextures;
        std::vector<std::string> _buttonTooltips;
        std::deque<resources::Map*> _maps;
        std::deque<resources::Tileset*> _tilesets;

        editor::render::modals::TilesetWizard* _tilesetWizard;

        float _zoom = 1.0f;

        int _selectedGridMode = 0;
        int _selectedLayer = 0;

        int _selectedTileset = 0;
        int _selectedTile = 0;
        int _selectedMap = 0;

        bool _isGridShown = true;

        void beforeRender() override;

        void onRender() override;

        void drawGrid();

        void drawTileInGrid(resources::Map* currentMap, int mapWidth, int i, int j, ImVec2 tilePos, ImVec2 tileEnd, ImDrawList* drawList);

        void drawToolbar();

        void drawTileSelector();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_MAPEDITOR_H
