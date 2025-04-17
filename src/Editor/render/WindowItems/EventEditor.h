//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTEDITOR_H
#define EVENTEDITOR_H

#include "render/WindowItem.h"

namespace editor {
    class Project;
}

namespace editor::render::tabs {

    class EventEditor : public WindowItem {
    public:
        EventEditor(editor::Project* project);
        ~EventEditor() override;
        void save();
    protected:
        void beforeRender() override;
        void onRender() override;
    private:
        Project* _project;
    };

}

#endif //EVENTEDITOR_H
