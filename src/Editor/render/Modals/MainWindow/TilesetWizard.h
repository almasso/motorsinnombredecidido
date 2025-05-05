//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_TILESETWIZARD_H
#define RPGBAKER_TILESETWIZARD_H

#include "render/ModalWindow.h"
#include <imgui.h>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Tileset;
}

namespace editor::render::modals {
    class TilesetWizard : public ModalWindow {
    public:
        TilesetWizard(Project* project);

        void setTilesetToModify(editor::resources::Tileset* tileset, bool modify = false);

        ~TilesetWizard();

    private:
        Project* _project = nullptr;

        ImTextureID _loadedTexture = 0;

        editor::resources::Tileset* _tilesetToModify = nullptr;

        int _dimensions[2];

        bool _isGivingName = false;

        bool _modify = false;

        bool _sameName = false;

        char _nameBuffer[256];

        char _routeBuffer[1024];

        float _color[3];

        int _offset[2];

        void beforeRender() override;

        void onRender() override;

        void drawControls();

        void drawGrid();
    };
}


#endif //RPGBAKER_TILESETWIZARD_H
