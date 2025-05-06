//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef DIALOGUEBEHAVIOUR_H
#define DIALOGUEBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(DialogueBehaviour) {
    public:
        DialogueBehaviour(Event* event);
        ~DialogueBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        char* _text;
    };

}

#endif //DIALOGUEBEHAVIOUR_H
