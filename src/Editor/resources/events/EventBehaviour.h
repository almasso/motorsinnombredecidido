//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTBEHAVIOUR_H
#define EVENTBEHAVIOUR_H

#include <unordered_set>
#include <io/LuaManager.h>
#include <sol/table.hpp>

#include <Utils/string_literal.h>

#define idKey "id"
#define paramsKey "params"

namespace editor::resources {
    class Object;
}

namespace editor::resources::events {
    class Event;
    struct EventBuildDependencies;

    class EventBehaviour {
    public:
        EventBehaviour(Event* event);
        virtual ~EventBehaviour();
        virtual bool read(sol::table const& params) = 0;
        bool writeToEngine(std::ostream& behaviours, EventBuildDependencies& dependencies);
        virtual bool write(sol::table& behaviour) = 0;
        virtual bool render() = 0;
        virtual const char* getID() = 0;

    protected:
        Event* _event;

        virtual bool writeParams(sol::table& params) = 0;
        virtual bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies) = 0;
    };

    template<string_literal name>
    class EventBehaviourTemplate : public EventBehaviour {
    public:
        static constexpr const char* id = name.value;

        EventBehaviourTemplate(Event* event) :
            EventBehaviour(event) {
        }

        bool write(sol::table& behaviour) final {
            behaviour[idKey] = id;
            sol::table params = io::LuaManager::GetInstance().getState().create_table();
            if (!writeParams(params))
                return false;
            behaviour[paramsKey] = params;
            return true;
        }
        const char* getID() final {
            return id;
        }
    };

    #define EventBehaviourClass(BehaviourName) BehaviourName : public EventBehaviourTemplate <#BehaviourName>

}

#endif //EVENTBEHAVIOUR_H
