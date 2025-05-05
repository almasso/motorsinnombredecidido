//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_CREATEPROJECTMODAL_H
#define RPGBAKER_CREATEPROJECTMODAL_H

#include "render/ModalWindow.h"

namespace editor {
    class Project;
}

namespace editor::render::modals {
    class CreateProjectModal : public ModalWindow {
    public:
        explicit CreateProjectModal();

        ~CreateProjectModal() override;

        editor::Project* getCreatedProject();
    private:
        editor::Project* _projectCreated = nullptr;

        bool _isGivingName = false;

        char _nameBuffer[256];

        char _routeBuffer[1024];

        char _fullRoute[1280];

        int _dimensions[2];

        bool _sameName = false;

        void onRender() override;

        void beforeRender() override;
    };
}


#endif //RPGBAKER_CREATEPROJECTMODAL_H
