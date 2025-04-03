#ifndef TEXTTEXTURE_H
#define TEXTTEXTURE_H
#include "Color.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class Font;
struct SDL_Texture;

class TextTexture {
    private:
        SDL_Texture* _texture;
    public:
        TextTexture();
        ~TextTexture();
        TextTexture(const Font *font, Color color, std::string text, float maxWidth, float maxHeight);
        void free();
        SDL_Texture* getTexture() const;
};


#endif //TEXTTEXTURE_H
