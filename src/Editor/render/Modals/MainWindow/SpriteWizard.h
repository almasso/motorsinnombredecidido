//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_SPRITEWIZARD_H
#define MOTORSINNOMBREDECIDIDO_SPRITEWIZARD_H

#include "render/ModalWindow.h"
#include <imgui.h>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Sprite;
}

namespace editor::render::modals {
    class SpriteWizard : public ModalWindow {
    public:
        SpriteWizard();

        void setSpriteToModify(editor::resources::Sprite* sprite, bool modify = false);

        ~SpriteWizard();
    private:
        Project* _project = nullptr;

        ImTextureID _loadedTexture = 0;

        editor::resources::Sprite* _spriteToModify = nullptr;

        int _dimensions[2];

        int _coords[2];

        int _maxX;
        int _maxY;
        int _maxWidth;
        int _maxHeight;

        bool _isGivingName = false;

        bool _modify = false;

        bool _sameName = false;

        char _nameBuffer[256];

        char _routeBuffer[1024];

        float _color[3];

        void beforeRender() override;

        void onRender() override;

        void drawControls();

        void drawSprite();
    };
}



#endif //SPRITEWIZARD_H
