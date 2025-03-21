#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <Load/Resource.h>

class SDL_Texture;

class Texture : public Resource {
    public:
    SDL_Texture* texture;
    Texture(std::string const &path);
    bool load() override;
    void unload() override;
};


#endif //TEXTURE_H
