#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include "Core/Component.h"

class RenderManager;
class Transform;

class RenderComponent : public Component {
    private:
    int _layer {};
    protected:
    Transform* _transform {};
    public:
    virtual bool render(RenderManager* manager) = 0;
    bool init() override;
};



#endif //RENDERCOMPONENT_H
