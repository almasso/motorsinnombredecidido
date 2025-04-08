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

namespace editor::resources::events {

    class EventCondition {
    public:
        virtual ~EventCondition();
        virtual bool read(sol::table const& params) = 0;
        virtual bool write(sol::table& condition) = 0;
        virtual bool writeToEngine(sol::table& condition) = 0;
    protected:
        virtual bool writeParams(sol::table& params) = 0;
    };

    template<string_literal name>
    class EventConditionTemplate : public EventCondition {
    public:
        static constexpr const char* id = name.value;

        bool write(sol::table& condition) final {
            condition[typeKey] = id;
            sol::table params = io::LuaManager::GetInstance().getState().create_table();
            if (!writeParams(params))
                return false;
            condition[paramsKey] = params;
            return true;
        }
    };

    #define EventConditionClass(ConditionType, ConditionName) ConditionType : public EventConditionTemplate<ConditionName>

}

#endif //EVENTCONDITION_H
