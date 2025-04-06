//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <string>
#include <sol/forward.hpp>

namespace editor::resources::events {
    class EventBehaviour;
    class EventCondition;

    class Event {
    public:
        ~Event();

        void init(std::string const& name);
        bool read(std::string const& name, sol::table const& eventTable);
        bool write(sol::table& eventTable);
        bool writeToEngine(sol::table& eventTable);

        std::string const& getName() const;
        EventBehaviour* addBehaviour(std::string const& id);
    private:
        std::vector<EventBehaviour*> _behaviours;
        EventCondition* _condition;
        std::string _name;

        bool readBehaviours(sol::table const& behaviours);
    };
}

#endif //EVENT_H
