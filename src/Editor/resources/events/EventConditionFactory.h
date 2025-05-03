//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTCONDITIONFACTORY_H
#define EVENTCONDITIONFACTORY_H

#include <string>
#include <functional>
#include <unordered_map>
#include <set>
#include <sol/forward.hpp>

namespace editor::resources::events {
    class Event;

    class EventCondition;

    class EventConditionFactory {
    public:
        static EventCondition* Create(std::string const& id, Event* event);
        static EventCondition* Create(sol::table const& condition, Event* event);
        static std::set<std::string> const& GetKeys();
    private:
        static std::unordered_map <std::string, std::function<EventCondition*(Event*)>> _conditions;
        static std::set<std::string> _conditionKeys;
        static bool _initialized;

        static void Init();
        template <typename ConditionType>
        static void RegisterCondition() {
            _conditions.insert({ConditionType::id, [](Event* event) -> EventCondition* {
                return new ConditionType(event);
            }});
            _conditionKeys.insert(ConditionType::id);
        }
    };

}

#endif //EVENTCONDITIONFACTORY_H
