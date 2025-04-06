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
    class MapWizard;
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

        char* _currentDirectory;

        editor::render::modals::TilesetWizard* _tilesetWizard;
        editor::render::modals::MapWizard* _mapWizard;

        float _zoom = 1.0f;

        int _selectedGridMode = 0;
        int _selectedLayer = -1;

        int _selectedTileset = -1;
        int _selectedTile = -1;
        int _selectedMap = -1;

        bool _isGridShown = true;

        void beforeRender() override;

        void onRender() override;

        void drawGrid();

        void drawTileInGrid(resources::Map* currentMap, int mapWidth, int i, int j, ImVec2 tilePos, ImVec2 tileEnd, ImDrawList* drawList);

        void drawToolbar();

        void drawTileSelector();

        void drawObjectInspector();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_MAPEDITOR_H
