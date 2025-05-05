//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_SPRITEANIMVIEWER_H
#define RPGBAKER_SPRITEANIMVIEWER_H

#include "render/WindowItem.h"
#include <vector>
#include <imgui.h>
#include <unordered_map>
#include <string>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Sprite;
    class Animation;
}

namespace editor::render::modals {
    class AnimationWizard;
    class SpriteWizard;
}

namespace editor::render::tabs {
    class SpriteAnimViewer : public WindowItem {
    public:
        SpriteAnimViewer(editor::Project* project);

        void save();

        ~SpriteAnimViewer() override;
    private:
        editor::Project* _project = nullptr;
        bool _somethingModified = false;

        editor::render::modals::SpriteWizard* _spriteWizard = nullptr;
        editor::render::modals::AnimationWizard* _animationWizard = nullptr;

        editor::resources::Sprite* _createdSprite = nullptr;
        editor::resources::Animation* _createdAnim = nullptr;

        bool _spriteOpened = false;
        bool _animationOpened = false;

        void beforeRender() override;

        void onRender() override;

        void drawAnimGrid();

        void drawSpriteGrid();
    };
}

#endif //SPRITEEDITOR_H
