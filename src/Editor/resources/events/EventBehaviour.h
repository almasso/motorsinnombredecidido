//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTBEHAVIOUR_H
#define EVENTBEHAVIOUR_H

#include <sol/table.hpp>

#include <Utils/string_literal.h>

namespace editor::resources::events {

    class EventBehaviour {
    public:
        virtual ~EventBehaviour();
        virtual bool read(sol::table const& params) = 0;
        virtual bool write(sol::table& behaviour) = 0;
        virtual bool writeToEngine(sol::table& behaviour) = 0;
    };

    template<string_literal name>
    class EventBehaviourTemplate : public EventBehaviour {
    public:
        static constexpr const char* id = name.value;
    };

    #define EventBehaviourClass(BehaviourName) BehaviourName : public EventBehaviourTemplate <#BehaviourName>

}

#endif //EVENTBEHAVIOUR_H
