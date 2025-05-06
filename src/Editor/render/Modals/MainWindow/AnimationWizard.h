//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_ANIMATIONWIZARD_H
#define RPGBAKER_ANIMATIONWIZARD_H

#include "Render/ModalWindow.h"
#include <list>
#include <filesystem>
#include <imgui.h>
#include <vector>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Animation;
    class Sprite;
}

namespace editor::render::modals {
    class AnimationWizard : public ModalWindow {
    public:
        AnimationWizard(editor::Project* project);

        void setAnimationToModify(editor::resources::Animation* animation, bool modify = false);

        ~AnimationWizard();
    private:
        Project* _project = nullptr;

        editor::resources::Animation* _animationToModify = nullptr;

        bool _isGivingName = false;

        bool _modify = false;

        bool _sameName = false;

        bool _openSpriteSelector = false;

        char _nameBuffer[256];

        bool _loop = false;

        bool _playing = false;

        std::list<resources::Sprite*> _frames;

        editor::resources::Sprite* _selectedFrame = nullptr;

        int _reproductionFrame = 0;

        float _timeBetweenFrames = 1.0f;

        float _frameTimer = 0.0f;

        bool _animationRunning = false;

        void beforeRender() override;

        void onRender() override;

        void drawControls();

        void drawAnimation();

        void drawThumbnail();

        void drawSpriteSelector();

        void drawAnimationControls();
    };
}

#endif //ANIMATIONWIZARD_H
