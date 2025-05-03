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
        enum ANIMATION_ACTION{
            PLAY,
            STOP,
            RESET,
            CHANGE,
            MAX_ACTION
        } _action;
        std::string _animationToChange;

        bool renderActionSelector();
        bool renderAnimationSelector();
        std::string getActionName(ANIMATION_ACTION action) const;

    };

}

#endif //ANIMATIONBEHAVIOUR_H
