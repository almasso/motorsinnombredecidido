#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <cstdint>
#include <string>
#include <unordered_map>
#include "AudioClipKey.h"
#include "AudioClipData.h"

typedef uint32_t AudioDevice;
class AudioMixer;
class AudioClip;
struct AudioMixerData;

class AudioManager {
private:
    static AudioManager* _instance;

    AudioDevice _audioDeviceId;
    std::unordered_map<std::string, AudioMixer*> _mixers;

    std::unordered_map<AudioClip*, AudioClipKey> _clipNames;

    AudioClipData* testClipData_;
    AudioClip* testClip_;
    bool testFlag_ = true;


    bool initTest();
    void updateTest();
    void shutdownTest();
    explicit AudioManager();
    bool init();
    AudioMixer* registerAudioMixer(AudioMixerData const* data);

public:
    static bool Init();
    static AudioManager* Instance();
    static void Shutdown();
    ~AudioManager();
    void update();

    AudioMixer* getMixer(std::string const& mixer);
    AudioClip* createAudioClip(AudioClipKey const& key);
    void releaseAudioClip(AudioClip* clip);
};

#endif //AUDIOMANAGER_H