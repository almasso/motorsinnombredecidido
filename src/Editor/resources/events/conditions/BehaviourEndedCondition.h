//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef BEHAVIOURENDEDCONDITION_H
#define BEHAVIOURENDEDCONDITION_H

#include "../EventCondition.h"

namespace editor::resources
{
    class Object;
}

namespace editor::resources::events {

    class EventConditionClass(BehaviourEndedCondition, "BehaviourEnded") {
    public:
        BehaviourEndedCondition(Event* event);
        ~BehaviourEndedCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies) override;
        bool writeParams(sol::table& params) override;
    private:
        std::string _conditionObject;
        std::string _conditionEvent;
        int _conditionBehaviour;

        bool renderObjectSelector(Object*& selectedObject);
        bool renderEventSelector(Object* selectedObject, Event*& selectedEvent);
        bool renderBehaviourSelector(Event* selectedEvent);

        static std::string getObjectName(std::string const& map, int object);
    };

}

#endif //BEHAVIOURENDEDCONDITION_H
