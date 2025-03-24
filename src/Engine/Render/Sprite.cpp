#include "Sprite.h"

#include <Load/LuaReader.h>

#include "RenderManager.h"
#include "Texture.h"
#include <Load/ResourceHandler.h>

bool Sprite::getRectField(float& field, std::string const& key, sol::table& rectTable) {
  field = rectTable.get_or(key, -1.0f);
  if (field < 0.0f)
    return false;
  return true;
}

Sprite::Sprite(std::string const &path) :
  Resource(path),
  _rect() {
}

bool Sprite::load() {
  sol::table table = LuaReader::GetTable(_path);
  if (!table.valid())
    return false;
  _texturePath = table.get_or<std::string>("texture", "");
  if (_texturePath.empty())
    return false;

  sol::table rect = LuaReader::GetTable(table, "rect");
  if (!rect.valid())
    return false;

  if (!getRectField(_rect.x, "x", rect))
    return false;
  if (!getRectField(_rect.y, "y", rect))
    return false;
  if (!getRectField(_rect.w, "w", rect))
    return false;
  if (!getRectField(_rect.h, "h", rect))
    return false;
  _size = 0;
  return true;
}

void Sprite::unload() {
  _texturePath = "";
  _rect = Rect();
}

const Texture* Sprite::getTexture() const{
  return ResourceHandler<Texture>::Instance()->get(_texturePath);
}

const Rect & Sprite::getRect() const{
  return _rect;
}
