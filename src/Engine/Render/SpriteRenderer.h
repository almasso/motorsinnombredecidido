#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
#include <Utils/Vector2.h>

#include "RenderComponent.h"
#include "Sprite.h"


class SpriteRenderer : public RenderComponent {
    private:
    const Sprite* _sprite{};
    Vector2 _size {};
    public:
    static const std::string id;
    bool render(RenderManager* manager) override;
    void setSprite(const Sprite* sprite);
};



#endif //SPRITERENDERER_H
