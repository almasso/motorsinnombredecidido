#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include "Core/ComponentTemplate.h"

class RenderManager;
class Transform;

class RenderComponent : public Component {
    private:
    int _layer {};
    protected:
    Transform* _transform {};
    public:
    RenderComponent(ComponentData *data);
    virtual bool render(RenderManager* manager) = 0;
    bool init() override;
    void onEnable() override;
    void onDisable() override;
};



#endif //RENDERCOMPONENT_H
