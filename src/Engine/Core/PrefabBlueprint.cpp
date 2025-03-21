#include "PrefabBlueprint.h"
/*
#include <Load/LuaReader.h>

PrefabBlueprint::PrefabBlueprint(std::string const& path) :
    Resource(path) {
}

bool PrefabBlueprint::load() {
    sol::table blueprint = LuaReader::GetTable(_path);
    if (blueprint == nullptr)
        return false;
    EntityBlueprint::read(blueprint);
    return true;
}

void PrefabBlueprint::unload() {
    EntityBlueprint::clear();
}
*/