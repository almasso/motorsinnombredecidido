#ifndef CAMERA_H
#define CAMERA_H
#include "RenderComponent.h"
#include <Utils/Vector2.h>

class Camera : public ComponentTemplate<"Camera",RenderComponent>{
private:
    Vector2 _size = {1,1};
public:
    Camera(ComponentData* data);
    bool render(RenderManager* manager) override;
};


#endif //CAMERA_H
