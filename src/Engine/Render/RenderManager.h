#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include <Utils/Vector2.h>
#include <Utils/Rect.h>
#include "Color.h"
#include "Sprite.h"

class SDL_Window;
class SDL_Renderer;

class RenderManager {
    public:
    RenderManager();
    bool init(const int& width, const int& height);
    void present() const;
    void clear() const;
    bool drawRect(const Rect &rect, const Color& color) const;
    bool drawSprite(const Rect &rect, const Sprite *sprite, float rotation) const;
    void getWindowSize(int *width, int *height) const;
    void shutdown() const;
    std::pair<float,const Vector2&> setViewRect(const Vector2& viewPosition, const Vector2& viewSize);
    Rect convertRect(const Rect& rect) const;
    private:
    Vector2 _viewOffset;
    Vector2 _screenOffset;
    float _screenScale;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    float _width, _height;
};

#endif //RENDERMANAGER_H
