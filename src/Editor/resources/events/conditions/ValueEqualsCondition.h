//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef VALUEEQUALSCONDITION_H
#define VALUEEQUALSCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(ValueEqualsCondition, "ValueEquals") {
    public:
        ValueEqualsCondition(Event* event);
        ~ValueEqualsCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(sol::table& params) override;
        bool writeParams(sol::table& params) override;
    private:
        char* _variable;
        char* _equals;

        bool renderVariable();
        bool renderValue();
    };

}

#endif //VALUEEQUALSCONDITION_H
