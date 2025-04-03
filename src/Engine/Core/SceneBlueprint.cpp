#include "SceneBlueprint.h"

#include <Load/LuaReader.h>

SceneBlueprint::SceneBlueprint(std::string const &path)
: Resource(path),
  _entities() {
}
bool SceneBlueprint::load() {
    sol::table blueprint = LuaReader::GetTable(_path);
    if (!blueprint.valid())
        return false;
    for (auto&& [key, value] : blueprint) {
        if (value.valid())
            _entities.emplace_back(value);
    }
    _size = 0;
    return true;
}

void SceneBlueprint::unload() {
    _entities.clear();
}

const std::vector<EntityBlueprint> & SceneBlueprint::getEntities() const {
    return _entities;
}
