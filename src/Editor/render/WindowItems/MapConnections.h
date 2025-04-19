//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MAPCONNECTIONS_H
#define MAPCONNECTIONS_H

#include "render/WindowItem.h"
#include <vector>
#include <unordered_map>
#include <imgui.h>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Map;
}

namespace editor::render::tabs {
    class MapConnections : public WindowItem {
    public:
        MapConnections(Project* project);

        void save();

        void getAdjacentMaps();

        ~MapConnections() override;
    private:
        std::vector<std::vector<resources::Map*>> _worlds;
        int _activeWorld = 0;
        Project* _project = nullptr;
        float _zoom = 0.75f;
        bool _isGridShown = true;
        bool _somethingModified = false;
        bool _isPanning = false;
        bool _createCentered = false;
        ImVec2 _lastMousePos;
        bool _isDragging = false;
        resources::Map * _draggedMap = nullptr;
        int _dragOffsetX, _dragOffsetY;
        int _oldMapX, _oldMapY;

        void beforeRender() override;

        void onRender() override;

        void drawGrid();

        void drawSelector();

        void handleMapDrag(int gridWidth, int gridHeight, float scaledSize, ImVec2 gridStartPos);

        void zoomAndPan();

        bool drawTileInGrid(int gridPosX, int gridPosY, ImVec2 tilePos, ImVec2 tileEnd, ImDrawList *drawList);

        bool findFreePosition(resources::Map *map, int maxRadius = 100);
    };
}



#endif //MAPCONNECTIONS_H
