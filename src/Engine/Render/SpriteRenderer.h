#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
#include "RenderComponent.h"
#include <Utils/Vector2.h>
#include <string>

class SpriteRenderer : public ComponentTemplate<"SpriteRenderer",RenderComponent> {
    private:
    std::string _sprite {};
    Vector2 _size {};
    public:
    SpriteRenderer(ComponentData const*data);
    bool init() override;
    bool render(RenderManager* manager) override;
    bool setSprite(const std::string& spritePath);
};


#endif //SPRITERENDERER_H
