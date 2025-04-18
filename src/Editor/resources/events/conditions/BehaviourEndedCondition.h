//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef BEHAVIOURENDEDCONDITION_H
#define BEHAVIOURENDEDCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(BehaviourEndedCondition, "BehaviourEnded") {
    public:
        BehaviourEndedCondition();
        ~BehaviourEndedCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(sol::table& params) override;
        bool writeParams(sol::table& params) override;
    private:
        std::string _entity;
        std::string _event;
        int _behaviour;
    };

}

#endif //BEHAVIOURENDEDCONDITION_H
