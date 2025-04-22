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

        void save();

        ~MapEditor() override;
    private:
        enum GridDrawingMode {
            DRAW_SELECTED_LAYER_ONLY = 0,
            DRAW_SELECTED_LAYER_BELOW_TRANSPARENT = 1,
            DRAW_SELECTED_LAYER_BELOW_OPAQUE = 2,
            DRAW_ALL = 3,
            DRAW_COLLISIONS = 4
        };

        editor::Project* _project = nullptr;
        std::vector<ImTextureID> _uiTextures;
        std::vector<std::string> _buttonTooltips;
        resources::Map* _createdMap = nullptr;
        resources::Tileset* _createdTileset = nullptr;
        bool _tilesetOpened = false;
        bool _mapOpened = false;
        char* _currentDirectory;
        editor::render::modals::TilesetWizard* _tilesetWizard = nullptr;
        editor::render::modals::MapWizard* _mapWizard = nullptr;
        float _zoom = 1.0f;
        int _selectedGridMode = 0;
        int _tmpTileMode = 0;
        int _selectedLayer = -1;
        editor::resources::Tileset* _selectedTileset = nullptr;
        int _selectedTile = -1;
        editor::resources::Map* _selectedMap = nullptr;
        bool _isGridShown = true;
        bool _collisionsShown = false;
        bool _somethingModified = false;

        void beforeRender() override;

        void onRender() override;

        void drawGrid();

        void drawTileInGrid(resources::Map* currentMap, int mapWidth, int i, int j, ImVec2 tilePos, ImVec2 tileEnd, ImDrawList* drawList);

        void drawToolbar();

        void drawTileSelector();

        void drawObjectInspector();

        void drawX(const ImVec2& tilePos, const ImVec2& tileEnd, ImDrawList* drawList);
    };
}


#endif //MOTORSINNOMBREDECIDIDO_MAPEDITOR_H
