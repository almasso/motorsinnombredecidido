#ifndef CAMERA_H
#define CAMERA_H
#include "RenderComponent.h"
#include <Utils/Vector2.h>

class Camera : public ComponentTemplate<"Camera",RenderComponent>{
private:
    Vector2 _size = {1,1};
    Vector2 _screenOffset = {0,0};
    float _screenScale = 1;
public:
    Camera(ComponentData const* data);
    bool init() override;
    bool render(RenderManager* manager) override;
    Vector2 screenToWorld(const Vector2& screenPos) const;
};


#endif //CAMERA_H
