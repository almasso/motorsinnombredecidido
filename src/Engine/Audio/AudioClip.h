#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H
#include <cstdint>

typedef uint32_t AudioDevice;
typedef struct SDL_AudioStream AudioStream;
typedef struct SDL_AudioSpec AudioSpec;
class AudioMixer;
typedef uint8_t AudioData;

class AudioClip {
private:
    enum State {
        PLAYING,
        PAUSED,
        STOPPED
    } state_;
    AudioData* buffer_;
    uint32_t bufferLen_;
    AudioSpec* specifier_;
    AudioStream* stream_;
    AudioMixer* mixer_;
    AudioDevice device_;
    float localVolume_, volume_;

    void reset();
public:
    AudioClip(AudioData* buffer, uint32_t bufferLen, AudioSpec* specifier);
    ~AudioClip();
    bool play();
    bool stop();
    void pause();
    bool resume();
    bool isPlaying() const;
    float getVolume() const;
    void setVolume(float volume);
    void updateVolume();
    void assignMixer(AudioMixer* mixer);
    void assignDevice(AudioDevice device);
};


#endif //AUDIOCLIP_H
