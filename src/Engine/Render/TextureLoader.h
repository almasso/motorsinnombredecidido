#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include "Texture.h"
#include "Color.h"

class SDL_Renderer;

class TextureLoader {
    private:
    static SDL_Renderer* _renderer;
    public:
    static bool Init(SDL_Renderer* renderer);
    static SDL_Texture* GetTexture(const Color& color);
    static SDL_Texture* GetTexture(const std::string& filePath);
};



#endif //TEXTURELOADER_H
