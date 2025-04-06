//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTBEHAVIOUR_H
#define EVENTBEHAVIOUR_H

#include <sol/table.hpp>

#include <Utils/string_literal.h>

#define idKey "id"
#define paramsKey "params"

namespace editor::resources::events {

    class EventBehaviour {
    public:
        virtual ~EventBehaviour();
        virtual bool read(sol::table const& params) = 0;
        virtual bool write(sol::table& behaviour) = 0;
        virtual bool writeToEngine(sol::table& behaviour) = 0;
    protected:
        virtual bool writeParams(sol::table& params) = 0;
    };

    template<string_literal name>
    class EventBehaviourTemplate : public EventBehaviour {
    public:
        static constexpr const char* id = name.value;

        bool write(sol::table& behaviour) final {
            behaviour[idKey] = id;
            sol::table params;
            if (!writeParams(params))
                return false;
            behaviour[paramsKey] = params;
            return true;
        }
    };

    #define EventBehaviourClass(BehaviourName) BehaviourName : public EventBehaviourTemplate <#BehaviourName>

}

#endif //EVENTBEHAVIOUR_H
