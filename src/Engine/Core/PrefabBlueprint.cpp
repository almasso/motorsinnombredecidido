#include "PrefabBlueprint.h"

#include <Load/LuaReader.h>

PrefabBlueprint::PrefabBlueprint(std::string const& path) :
    Resource(path) {
}

bool PrefabBlueprint::load() {
    sol::table blueprint = LuaReader::GetTable(_path);
    if (!blueprint.valid())
        return false;
    EntityBlueprint::read(blueprint);
    _size = 0;
    return true;
}

void PrefabBlueprint::unload() {
    EntityBlueprint::clear();
}
