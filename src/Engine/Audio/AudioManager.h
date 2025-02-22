#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <cstdint>
#include <string>
#include <unordered_map>

typedef uint32_t AudioDevice;
typedef struct SDL_AudioSpec AudioSpec;
typedef struct SDL_AudioStream AudioStream;
class AudioMixer;
class AudioClip;

class AudioManager {
private:
    AudioDevice audioDeviceId;
    std::unordered_map<std::string, AudioMixer*> mixers_;

    AudioClip* testClip_;
    bool testFlag_ = true;

public:
    AudioManager();
    ~AudioManager();
    bool init();
    void update();
    void shutdown();
};

#endif //AUDIOMANAGER_H