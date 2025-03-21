#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "SpriteRenderer.h"
#include "Animation.h"

class Animator : public ComponentTemplate<"Animator",SpriteRenderer> {
    const Animation* _animation {};
    float _frameTimer {};
    int _currentFrame {};
    bool _animationEnded {};
public:
    bool update() override;
    void reset();
    void changeAnimation(const Animation* animation);
};

#endif //ANIMATOR_H
