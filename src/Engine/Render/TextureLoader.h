#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <SDL3/SDL_render.h>

#include "Texture.h"
#include "Color.h"

struct SDL_Surface;
class SDL_Renderer;

class TextureLoader {
    private:
    static SDL_Renderer* _renderer;
    public:
    static bool Init(SDL_Renderer* renderer);
    static SDL_Texture* GetTexture(const Color& color);
    static SDL_Texture* GetTexture(const std::string& filePath);
    static SDL_Texture* GetTexture(SDL_Surface* surface);
    static void ResizeTexture(SDL_Texture *&texture, int width, int height);
};



#endif //TEXTURELOADER_H
