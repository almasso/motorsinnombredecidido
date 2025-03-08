#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <cstdint>
#include <string>
#include <unordered_map>
#include <Load/ResourceHandler.h>
#include "AudioClipData.h"

typedef uint32_t AudioDevice;
class AudioMixer;
class AudioClip;
struct AudioMixerData;


class AudioManager {
private:
    AudioDevice audioDeviceId;
    std::unordered_map<std::string, AudioMixer*> mixers_;

    ResourceHandler<std::string, AudioClipData> clipDataHandler_;
    std::unordered_map<AudioClip*, std::string> clipNames_;

    AudioClipData* testClipData_;
    AudioClip* testClip_;
    bool testFlag_ = true;


    bool initTest();
    void updateTest();
    void shutdownTest();
public:
    AudioManager();
    ~AudioManager();
    bool init();
    void update();
    void shutdown();

    bool registerAudioMixer(AudioMixerData const& data);
    AudioMixer* getMixer(std::string const& mixer);
    AudioClip* createAudioClip(std::string const& key);
    void releaseAudioClip(AudioClip* clip);
};

#endif //AUDIOMANAGER_H