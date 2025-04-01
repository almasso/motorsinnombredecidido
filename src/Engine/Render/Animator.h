#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "SpriteRenderer.h"

#include <sol/forward.hpp>

class ComponentDerived(Animator, SpriteRenderer) {
    std::string _animation {};
    float _frameTimer {};
    int _currentFrame {};
    bool _animationEnded {};
    bool _playing {};
public:
    Animator(ComponentData const* data);
    bool init() override;
    bool update() override;
    void reset();
    void changeAnimation(const std::string& animation);
    void setPlaying(bool playing);

    static void RegisterToLua(sol::state& lua);
};

#endif //ANIMATOR_H
