#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include "Texture.h"
#include "Color.h"

class TextureLoader {
    private:
    static TextureLoader* _instance;
    SDL_Renderer* _renderer;
    explicit TextureLoader(SDL_Renderer* renderer);
    public:
    static TextureLoader* Init(SDL_Renderer* renderer);
    static Texture* GetTexture(const Color& color);
};



#endif //TEXTURELOADER_H
