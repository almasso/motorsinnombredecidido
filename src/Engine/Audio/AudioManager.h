#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <cstdint>
#include <string>
#include <unordered_map>

typedef uint32_t AudioDevice;
class AudioMixer;
class AudioClip;
struct AudioMixerData;
struct AudioClipData;

class AudioManager {
private:
    AudioDevice audioDeviceId;
    std::unordered_map<std::string, AudioMixer*> mixers_;

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
    AudioClip* createAudioClip(AudioClipData const* data) const;
};

#endif //AUDIOMANAGER_H