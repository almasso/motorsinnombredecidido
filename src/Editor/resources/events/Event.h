//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENT_H
#define EVENT_H

#include <filesystem>
#include <vector>
#include <list>
#include <string>
#include <sol/forward.hpp>

namespace editor {
    class Project;
}

namespace editor::resources::events {
    class EventBehaviour;
    class EventCondition;

    class Event {
    public:
        Event(Project* project);
        ~Event();

        bool readFromLua(std::string const& name);
        bool writeToLua();
        bool eraseFromLua();
        static bool EraseFromLua(std::string const& name);
        void init(std::string const& name);
        void init(std::string const& name, std::string const& condition);
        bool read(std::string const& name, sol::table const& eventTable);
        bool write(sol::table& eventTable);
        bool writeToEngine(sol::table& eventTable, std::vector<std::string>& componentDependencies);

        std::string const& getName() const;
        EventBehaviour* addBehaviour(std::string const& id);
        bool isInitialized() const;
        EventCondition* getCondition();
        void setCondition(std::string const& condition);
        std::list<EventBehaviour*>& getBehaviours();
        void changeBehaviour(EventBehaviour* previousBehaviour, std::string const& newBehaviour);
        void removeBehaviour(std::list<EventBehaviour*>::iterator& behaviour);
        void moveBehaviourUp(std::list<EventBehaviour*>::iterator& behaviour);
        void moveBehaviourDown(std::list<EventBehaviour*>::iterator& behaviour);

        Project* getProject();

        static void SetEventsDirectory(std::filesystem::path const& eventsDirectory);
    private:
        static std::filesystem::path _eventsDirectory;

        Project* _project;

        std::list<EventBehaviour*> _behaviours;
        EventCondition* _condition;
        std::string _name;

        bool _initialized;

        bool readBehaviours(sol::table const& behaviours);
        bool writeBehaviours(sol::table& behaviours);

        static std::string GetFilePath(std::string const& eventName);
    };
}

#endif //EVENT_H
