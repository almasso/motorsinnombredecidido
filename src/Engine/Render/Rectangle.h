#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "RenderComponent.h"
#include "Utils/Vector2.h"
#include "Color.h"

class ComponentDerived(Rectangle, RenderComponent) {
    private:
    Vector2 _size = {1,1};
    Color _color = {0};
    public:
    Rectangle(ComponentData const*data);
    bool init() override;
    bool render(RenderManager* manager) override;
};



#endif //RECTANGLE_H
