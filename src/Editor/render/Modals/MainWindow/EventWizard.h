//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTWIZARD_H
#define EVENTWIZARD_H

#include "render/ModalWindow.h"

namespace editor {
    class Project;
}

namespace editor::resources::events {
    class Event;
}

namespace editor::render::modals {

    class EventWizard : public ModalWindow {
    public:
        explicit EventWizard(Project* project);
        ~EventWizard() override;
        void setEventToModify(editor::resources::events::Event* event, bool modify = false);

    protected:
        void onRender() override;
        void beforeRender() override;

    private:
        Project* _project;

        editor::resources::events::Event* _eventToModify;
        bool _modify;

        bool _isGivingName;
        bool _sameName;
        char _nameBuffer[256];

        std::string _condition;

        void renderNameSelector();
        void renderConditionSelector();
        void renderAddCancelButtons();
    };

}

#endif //EVENTWIZARD_H
