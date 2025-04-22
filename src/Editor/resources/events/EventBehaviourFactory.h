//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTBEHAVIOURFACTORY_H
#define EVENTBEHAVIOURFACTORY_H

#include <functional>
#include <string>
#include <unordered_map>
#include <set>
#include <sol/forward.hpp>

namespace editor::resources::events {
    class Event;
    class EventBehaviour;

    class EventBehaviourFactory {
    public:
        static EventBehaviour* Create(std::string const& id, Event* event);
        static EventBehaviour* Create(sol::table const& behaviour, Event* event);
        static std::set<std::string> const& GetKeys();
    private:
        static std::unordered_map <std::string, std::function<EventBehaviour*(Event*)>> _behaviours;
        static std::set<std::string> _behaviourKeys;
        static bool _initialized;

        static void Init();
        template <typename BehaviourType>
        static void RegisterBehaviour() {
            _behaviours.insert({BehaviourType::id, [](Event* event) -> EventBehaviour* {
                return new BehaviourType(event);
            }});
            _behaviourKeys.insert(BehaviourType::id);
        }
    };

}

#endif //EVENTBEHAVIOURFACTORY_H
