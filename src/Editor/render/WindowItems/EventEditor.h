//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTEDITOR_H
#define EVENTEDITOR_H

#include <list>
#include <unordered_map>

#include "render/WindowItem.h"

namespace editor::resources::events
{
    class EventBehaviour;
}

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
        void renderBehaviourDropDown(resources::events::EventBehaviour*& behaviour);
        bool renderBehaviourRemoveButton(std::list<resources::events::EventBehaviour*>::iterator& behaviour);
        void renderBehaviourMoveUpButton(std::list<resources::events::EventBehaviour*>::iterator& behaviour);
        void renderBehaviourMoveDownButton(std::list<resources::events::EventBehaviour*>::iterator& behaviour);
        void renderAddBehaviourButton();
    };

}

#endif //EVENTEDITOR_H
