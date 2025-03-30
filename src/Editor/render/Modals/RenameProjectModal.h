//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_RENAMEPROJECTMODAL_H
#define MOTORSINNOMBREDECIDIDO_RENAMEPROJECTMODAL_H

#include "render/ModalWindow.h"

namespace editor {
    class Project;
}

namespace editor::render::modals {
    class RenameProjectModal : public ModalWindow {
    public:
        explicit RenameProjectModal(Project* project);
    private:
        Project* _project;

        std::string _projectRoute;

        std::string _projectName;

        bool _isRenaming = false;

        char _nameBuffer[256];

        void onRender() override;

        void beforeRender() override;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_RENAMEPROJECTMODAL_H
