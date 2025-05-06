//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MODIFYLOCALVARIABLEBEHAVIOUR_H
#define MODIFYLOCALVARIABLEBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(ModifyLocalVariableBehaviour) {
    public:
        ModifyLocalVariableBehaviour(Event* event);
        ~ModifyLocalVariableBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        char* _variable;
        char* _newValue;

        bool renderVariable();
        bool renderValue();
    };

}

#endif //MODIFYLOCALVARIABLEBEHAVIOUR_H
