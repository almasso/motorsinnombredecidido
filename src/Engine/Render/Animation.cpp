#include "Animation.h"

#include <Load/LuaReader.h>

Animation::Animation(std::string const &path) :
    Resource(path),
    frameTime(0),
    loop(false),
    numFrames(0) {
}

bool Animation::load() {
    sol::table table = LuaReader::GetTable(_path);
    if (!table.valid())
        return false;

    sol::table framesTable = LuaReader::GetTable(table, "frames");
    if (!framesTable.valid())
        return false;
    for (auto& [key, frame] : framesTable) {
        if (frame.is<std::string>())
            frames.push_back(frame.as<std::string>());
    }

    frameTime = table.get_or("frameTime", 0.5f);
    loop = table.get_or("loop", false);
    numFrames = static_cast<int>(frames.size());
    return true;
}

void Animation::unload() {
    frames.clear();
    numFrames = 0;
    loop = false;
    frameTime = 0.0f;
}
