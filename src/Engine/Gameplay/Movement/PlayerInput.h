#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H
#include <Core/ComponentTemplate.h>

class MovementComponent;
class Camera;

class ComponentClass(PlayerInput) {
    private:
        Camera* _camera;
        MovementComponent* _movement;
        bool _active;
    public:
        PlayerInput(ComponentData const* data);
        bool init() override;
        bool update() override;

        void setActive(bool active);
        bool isActive() const;
};

#endif //PLAYERINPUT_H
