#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <cstdint>
#include <string>
#include <unordered_map>

typedef uint32_t AudioDevice;
class AudioMixer;
class AudioClip;
struct AudioMixerData;

class AudioClipData;

class AudioManager {
private:
    static AudioManager* _instance;

    AudioDevice _audioDeviceId;
    std::unordered_map<std::string, AudioMixer*> _mixers;

    AudioClip* testClip_;
    bool testFlag_ = true;


    explicit AudioManager();
    bool init();
    AudioMixer* registerAudioMixer(AudioMixerData const* data);

public:
    static bool Init();
    static AudioManager* Instance();
    static void Shutdown();
    ~AudioManager();

    AudioMixer* getMixer(std::string const& mixer);
};

#endif //AUDIOMANAGER_H