//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTCONDITIONFACTORY_H
#define EVENTCONDITIONFACTORY_H

#include <string>
#include <functional>
#include <unordered_map>
#include <sol/forward.hpp>

namespace editor::resources::events {

    class EventCondition;

    class EventConditionFactory {
    public:
        static EventCondition* Create(std::string const& id);
        static EventCondition* Create(sol::table const& condition);
    private:
        static std::unordered_map <std::string, std::function<EventCondition*()>> _conditions;
        static bool _initialized;

        static void Init();
        template <typename ConditionType>
        static void RegisterCondition() {
            _conditions.insert({ConditionType::id, []() -> EventCondition* {
                return new ConditionType();
            }});
        }
    };

}

#endif //EVENTCONDITIONFACTORY_H
