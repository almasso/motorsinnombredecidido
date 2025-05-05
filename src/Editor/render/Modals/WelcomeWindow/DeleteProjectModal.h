//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_DELETEPROJECTMODAL_H
#define RPGBAKER_DELETEPROJECTMODAL_H

#include "render/ModalWindow.h"

namespace editor {
    class Project;
}

namespace editor::render::modals {
    class DeleteProjectModal : public ModalWindow {
    public:
        explicit DeleteProjectModal(Project* project);

        [[nodiscard]] bool hasConfirmedDeletion() const {return _confirmedDeletion;}
    private:
        Project* _project;

        std::string _projectRoute;

        std::string _projectName;

        bool _confirmedDeletion = false;

        bool _isRenaming = false;

        char _nameBuffer[256];

        void onRender() override;

        void beforeRender() override;
    };
}


#endif //RPGBAKER_DELETEPROJECTMODAL_H
