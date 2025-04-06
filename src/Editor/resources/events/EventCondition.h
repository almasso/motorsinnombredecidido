//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef EVENTCONDITION_H
#define EVENTCONDITION_H

#include <sol/table.hpp>
#include <Utils/string_literal.h>

namespace editor::resources::events {

    class EventCondition {
    public:
        virtual ~EventCondition();
        virtual bool read(sol::table const& condition) = 0;
        virtual bool write(sol::table& condition) = 0;
        virtual bool writeToEngine(sol::table& condition) = 0;
    };

    template<string_literal name>
    class EventConditionTemplate : public EventCondition {
    public:
        static constexpr const char* id = name.value;
    };

    #define EventConditionClass(ConditionName) ConditionName : public EventConditionTemplate<#ConditionName>

}

#endif //EVENTCONDITION_H
