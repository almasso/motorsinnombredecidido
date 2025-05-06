//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTCONDITION_H
#define EVENTCONDITION_H

#include <io/LuaManager.h>
#include <sol/table.hpp>
#include <Utils/string_literal.h>

#define typeKey "type"
#define paramsKey "params"
namespace editor::resources {
    class Object;
}
namespace editor::resources::events {
    class Event;
    struct EventBuildDependencies;

    class EventCondition {
    public:
        explicit EventCondition(Event* event);
        virtual ~EventCondition();
        virtual bool read(sol::table const& params) = 0;
        virtual bool write(sol::table& condition) = 0;
        virtual bool writeToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) = 0;
        virtual const char* getID() const = 0;
        virtual bool render() = 0;
    protected:
        Event* _event;

        virtual bool writeParams(sol::table& params) = 0;
        virtual bool writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) = 0;
    };

    template<string_literal name>
    class EventConditionTemplate : public EventCondition {
    public:
        static constexpr const char* id = name.value;

        EventConditionTemplate(Event* event) :
            EventCondition(event) {
        }

        bool write(sol::table& condition) final {
            condition[typeKey] = id;
            sol::table params = io::LuaManager::GetInstance().getState().create_table();
            if (!writeParams(params))
                return false;
            condition[paramsKey] = params;
            return true;
        }
        bool writeToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) final {
            condition << typeKey << " = \"" << id << "\",\n";
            condition << paramsKey << " = {\n";
            if (!writeParamsToEngine(condition, dependencies, container))
                return false;
            condition << "\n}\n";
            return true;
        }
        const char* getID() const final {
            return id;
        }
    };

    #define EventConditionClass(ConditionType, ConditionName) ConditionType : public EventConditionTemplate<ConditionName>

}

#endif //EVENTCONDITION_H
