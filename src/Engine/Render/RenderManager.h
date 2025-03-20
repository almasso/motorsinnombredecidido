#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include <Utils/Rect.h>
#include "Color.h"

class SDL_Window;
class SDL_Renderer;

class RenderManager {
    public:
    RenderManager();
    bool init(const int& width, const int& height);
    void present() const;
    void clear() const;
    void drawRect(const Rect &rect, const Color& color) const;
    void getWindowSize(int *width, int *height) const;
    void shutdown() const;
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif //RENDERMANAGER_H
