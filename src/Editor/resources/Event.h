//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <string>
#include <sol/forward.hpp>

namespace editor::resources {
    class EventBehaviour;
    class EventCondition;

    class Event {
    public:
        bool read(std::string const& name, sol::table const& eventTable);
        bool write(sol::table& eventTable);
        bool writeToEngine(sol::table& eventTable);
        std::string const& getName() const;
    private:
        std::vector<EventBehaviour*> _behaviours;
        EventCondition* _condition;
        std::string _name;
    };
}

#endif //EVENT_H
