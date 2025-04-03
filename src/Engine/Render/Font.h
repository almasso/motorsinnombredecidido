#ifndef FONT_H
#define FONT_H
#include <Load/Resource.h>
#include <SDL3_ttf/SDL_ttf.h>

class TTF_Font;

class Font : public Resource {
    private:
        TTF_Font* _font;
    public:
        explicit Font(std::string const &path);
        bool load() override;
        void unload() override;

        TTF_Font *getFont() const { return _font; }
};

#endif //FONT_H
