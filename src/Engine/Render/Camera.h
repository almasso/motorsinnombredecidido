#ifndef CAMERA_H
#define CAMERA_H
#include "RenderComponent.h"
#include <Utils/Vector2.h>

class Camera : public RenderComponent{
private:
    Vector2 _size = {1,1};
public:
    static const std::string id;
    Camera() = default;
    bool render(RenderManager* manager) override;
};


#endif //CAMERA_H
