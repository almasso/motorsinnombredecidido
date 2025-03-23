#include "SceneBlueprint.h"

#include <Load/LuaReader.h>

bool SceneBlueprint::load() {
    sol::table blueprint = LuaReader::GetTable(_path);
    if (!blueprint.valid())
        return false;
    for (auto&& [key, value] : blueprint) {
        if (value.valid())
            _entities.emplace_back(blueprint);

    }
    return true;
}

void SceneBlueprint::unload() {
    _entities.clear();
}
