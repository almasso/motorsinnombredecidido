#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include "Texture.h"
#include "Color.h"

class TextureLoader {
    private:
    static SDL_Renderer* _renderer;
    public:
    static bool Init(SDL_Renderer* renderer);
    static Texture* GetTexture(const Color& color);
    static Texture* GetTexture(const char* filePath);
};



#endif //TEXTURELOADER_H
