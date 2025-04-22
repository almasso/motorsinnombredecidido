//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ANIMATIONBEHAVIOUR_H
#define ANIMATIONBEHAVIOUR_H
#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(AnimationBehaviour) {
    public:
        AnimationBehaviour(Event* event);
        ~AnimationBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;

    private:
        enum {
            PLAY,
            STOP,
            RESET,
            CHANGE
        } _action;
        std::string _animationToChange;
    };

}

#endif //ANIMATIONBEHAVIOUR_H
