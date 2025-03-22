#include "AudioMixerData.h"

#include <Load/LuaReader.h>

AudioMixerData::AudioMixerData(std::string const& path) :
    Resource(path) {
}

bool AudioMixerData::load() {
    /*auto table = LuaReader::GetTable(_path);
    if (table == nullptr)
        return false;
    name = table.get_or<std::string>("name", "");
    if (name == "")
        return false;

    output = table.get_or<std::string>("output", "");

    auto inputsTable = LuaReader::GetTable(table, "inputs");
    if (inputsTable == nullptr)
        return true;
    for (auto const& [key, input] : inputsTable) {
        if (input.is<std::string>())
            inputs.push_back(input.as<std::string>());
    }*/
    return true;
}

void AudioMixerData::unload() {
    inputs.clear();
}
