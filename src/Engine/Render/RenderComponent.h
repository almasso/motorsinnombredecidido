#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include "Core/ComponentTemplate.h"

class RenderManager;
class Transform;

class RenderComponent : public Component {
    private:
protected:
    int _layer;
    Transform* _transform {};
    public:
    RenderComponent(ComponentData const* data);
    virtual bool render(RenderManager* manager) = 0;
    bool init() override;
    void onEnable() override;
    void onDisable() override;
};



#endif //RENDERCOMPONENT_H
