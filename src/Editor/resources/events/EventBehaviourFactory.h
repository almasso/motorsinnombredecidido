//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTBEHAVIOURFACTORY_H
#define EVENTBEHAVIOURFACTORY_H

#include <functional>
#include <string>
#include <unordered_map>
#include <sol/forward.hpp>

namespace editor::resources::events {
    class EventBehaviour;

    class EventBehaviourFactory {
    public:
        static EventBehaviour* Create(std::string const& id);
        static EventBehaviour* Create(sol::table const& behaviour);
    private:
        static std::unordered_map <std::string, std::function<EventBehaviour*()>> _behaviours;

        void init();
        template <typename BehaviourType>
        static void RegisterBehaviour() {
            _behaviours.insert({BehaviourType::id, []() -> EventBehaviour* {
                return new BehaviourType();
            }});
        }
    };

}

#endif //EVENTBEHAVIOURFACTORY_H
