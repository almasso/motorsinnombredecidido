//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTEDITOR_H
#define EVENTEDITOR_H

#include <unordered_map>

#include "render/WindowItem.h"

namespace editor {
    class Project;

    namespace render::modals {
        class EventWizard;
    }

    namespace resources::events {
        class Event;
    }
}

namespace editor::render::tabs {

    class EventEditor : public WindowItem {
    public:
        explicit EventEditor(editor::Project* project);
        ~EventEditor() override;
        void save();
    protected:
        void beforeRender() override;
        void onRender() override;

    private:
        Project* _project;

        modals::EventWizard* _eventWizard;
        bool _eventOpened;

        resources::events::Event* _selectedEvent;
        resources::events::Event* _createdEvent;

        bool _somethingModified;

        void renderEventDropDown();
        bool renderSelectableEvent(std::unordered_map<std::string, resources::events::Event*>::const_iterator& it);
        bool renderSelectableEventOptions(std::unordered_map<std::string, resources::events::Event*>::const_iterator& it);
        void handleEventWizard();
        void renderConditionEditor();
        void renderConditionSelector();
        void renderBehaviourEditor();
    };

}

#endif //EVENTEDITOR_H
