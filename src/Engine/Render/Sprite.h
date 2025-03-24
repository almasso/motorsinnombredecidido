#ifndef SPRITE_H
#define SPRITE_H
#include <Utils/Rect.h>
#include <string>
#include <Load/Resource.h>
#include <sol/forward.hpp>

class Texture;

class Sprite : public Resource {
    std::string _texturePath;
    Rect _rect;

    bool getRectField(float& field, std::string const& key, sol::table& rectTable);
public:
    explicit Sprite(std::string const &path);
    bool load() override;
    void unload() override;
    const Texture* getTexture() const;
    const Rect& getRect() const;
};
#endif //SPRITE_H
