#include "TextTexture.h"
#include "TextureLoader.h"
#include "Font.h"
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <Utils/Rect.h>

TextTexture::TextTexture() : _texture(nullptr){
}

TextTexture::~TextTexture() {
    free();
}

TextTexture::TextTexture(const Font *font, Color color, std::string text, float maxWidth, float maxHeight, bool centerText) {
    if (text == "") text = " ";
    _texture = TextureLoader::GetTexture(
        TTF_RenderText_Solid_Wrapped(font->getFont(), text.c_str(), text.size(), color, maxWidth));
    TextureLoader::ResizeTexture(_texture, maxWidth, maxHeight, centerText);
}

void TextTexture::free() {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
}

SDL_Texture * TextTexture::getTexture() const {
    return _texture;
}