//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENT_H
#define EVENT_H

#include <filesystem>
#include <vector>
#include <string>
#include <sol/forward.hpp>

namespace editor::resources::events {
    class EventBehaviour;
    class EventCondition;

    class Event {
    public:
        Event();
        ~Event();

        bool readFromLua(std::string const& name);
        bool writeToLua();
        void init(std::string const& name);
        void init(std::string const& name, std::string const& condition);
        bool read(std::string const& name, sol::table const& eventTable);
        bool write(sol::table& eventTable);
        bool writeToEngine(sol::table& eventTable, std::vector<std::string>& componentDependencies);

        std::string const& getName() const;
        EventBehaviour* addBehaviour(std::string const& id);
        bool isInitialized() const;

        static void SetEventsDirectory(std::filesystem::path const& eventsDirectory);
    private:
        static std::filesystem::path _eventsDirectory;

        std::vector<EventBehaviour*> _behaviours;
        EventCondition* _condition;
        std::string _name;

        bool initialized;

        bool readBehaviours(sol::table const& behaviours);
        bool writeBehaviours(sol::table& behaviours);

        static std::string GetFilePath(std::string const& eventName);
    };
}

#endif //EVENT_H
