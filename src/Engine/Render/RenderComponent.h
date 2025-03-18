#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include "Core/Component.h"

class RenderManager;

class RenderComponent : public Component {
    public:
    virtual bool render(RenderManager* manager) = 0;
};



#endif //RENDERCOMPONENT_H
