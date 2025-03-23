#include "AudioMixerData.h"

#include <Load/LuaReader.h>

AudioMixerData::AudioMixerData(std::string const& path) :
    Resource(path),
    volume(0.0f) {
}

bool AudioMixerData::load() {
    auto table = LuaReader::GetTable(_path);
    if (!table.valid())
        return false;
    name = table.get_or<std::string>("name", "");
    if (name == "")
        return false;

    output = table.get_or<std::string>("output", "");

    auto inputsTable = LuaReader::GetTable(table, "inputs");
    if (!inputsTable.valid())
        return true;
    for (auto const& [key, input] : inputsTable) {
        if (input.is<std::string>())
            inputs.push_back(input.as<std::string>());
    }
    volume = table.get_or<float, std::string, float>("volume", 1.0f);
    _size = 0;
    return true;
}

void AudioMixerData::unload() {
    inputs.clear();
}
