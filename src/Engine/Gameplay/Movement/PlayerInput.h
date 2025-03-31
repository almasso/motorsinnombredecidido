#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H
#include <Core/ComponentTemplate.h>

class MovementComponent;
class Camera;

class PlayerInput : public ComponentTemplate<PlayerInput, "PlayerInput">{
    private:
        Camera* _camera;
        MovementComponent* _movement;
    public:
        PlayerInput(ComponentData const* data);
        bool init() override;
        bool update() override;
};

#endif //PLAYERINPUT_H
