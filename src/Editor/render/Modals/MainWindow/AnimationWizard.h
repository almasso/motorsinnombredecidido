//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_ANIMATIONWIZARD_H
#define MOTORSINNOMBREDECIDIDO_ANIMATIONWIZARD_H

#include "Render/ModalWindow.h"
#include <list>
#include <filesystem>
#include <imgui.h>

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

        char _nameBuffer[256];

        bool _loop = false;

        std::list<resources::Sprite*> _frames;

        std::list<ImTextureID> _frameTextures;

        int _selectedFrame = 0;

        int _reproductionFrame = 0;

        float _timeBetweenFrames = 1.0f;

        void beforeRender() override;

        void onRender() override;

        void drawControls();

        void drawAnimation();

        void drawThumbnail();

        void drawAnimationControls();

        void copyFramesToList(const std::vector<std::filesystem::path>& frames);

        void framesToTextures();

        void clearTextures();

        void copyFramesToVector(std::vector<std::filesystem::path>& frames);
    };
}

#endif //ANIMATIONWIZARD_H
