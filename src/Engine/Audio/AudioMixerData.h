#ifndef AUDIOMIXERDATA_H
#define AUDIOMIXERDATA_H

#include <vector>
#include <string>

struct AudioMixerData {
    std::string name;
    std::string device;
    std::string output;
    std::vector<std::string> inputs;
};

#endif //AUDIOMIXERDATA_H
