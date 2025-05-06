//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef CHOICESBEHAVIOUR_H
#define CHOICESBEHAVIOUR_H

#include <list>

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(ChoicesBehaviour) {
    public:
        ChoicesBehaviour(Event* event);
        ~ChoicesBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        using optionText = char*;
        using optionValue = char*;
        char* _variable;
        std::list<std::pair<optionText, optionValue>> _options;
        using optionsIterator = std::list<std::pair<optionText, optionValue>>::iterator;

        bool renderVariableToModify();
        bool renderOptions();
        bool renderOptionValue(optionValue value, int i);
        bool renderOptionText(optionText text, int i);
        bool renderRemoveOptionButton(std::list<optionsIterator>& optionsToRemove, optionsIterator it, int i);
        bool renderAddOptionButton();
    };

}

#endif //CHOICESBEHAVIOUR_H
