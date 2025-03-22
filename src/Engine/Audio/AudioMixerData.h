#ifndef AUDIOMIXERDATA_H
#define AUDIOMIXERDATA_H

#include <vector>
#include <string>
#include <Load/Resource.h>

class AudioMixerData : public Resource {
public:
    std::string name;
    std::string output;
    std::vector<std::string> inputs;
    explicit AudioMixerData(std::string const& path);
    bool load() override;
    void unload() override;
};

#endif //AUDIOMIXERDATA_H
