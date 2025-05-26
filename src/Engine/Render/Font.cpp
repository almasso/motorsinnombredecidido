#include "Font.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <Utils/Error.h>


#include <SDL3/SDL.h>
#ifdef __APPLE__
#define GetCurrentDir strdup(SDL_GetBasePath())
#else
#define GetCurrentDir SDL_GetCurrentDirectory()
#endif

Font::Font(std::string const &path) : Resource(path), _font(nullptr) {
}

bool Font::load() {
    size_t index = 0;
    while (index < _path.size() && std::isdigit(_path[index])) {
        index++;
    }
    int fontSize = std::stoi(_path.substr(0, index));
#ifdef __APPLE__
    auto currDir = GetCurrentDir;
    std::string fontPath = currDir + _path.substr(index);
    SDL_free(currDir);
#else
    std::string fontPath = _path.substr(index);
#endif
    _font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (_font == nullptr) {
        Error::ShowError("Fallo al cargar la fuente " + fontPath, "Fallo al cargar la fuente " + fontPath + "\n" + SDL_GetError());
        return false;
    }
    int width, height;
    int glyphs = 95;
    TTF_GetGlyphMetrics(_font,'M',nullptr,&width, nullptr,&height,nullptr);
    _size = glyphs * width * height;
    return true;
}

void Font::unload() {
    TTF_CloseFont(_font);
}
