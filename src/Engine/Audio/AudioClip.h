#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H
#include <cstdint>

typedef uint32_t AudioDevice;
typedef struct SDL_AudioStream AudioStream;
struct AudioClipData;
class AudioMixer;

class AudioClip {
private:
    enum State {
        PLAYING,
        PAUSED,
        STOPPED
    } state_;
    AudioClipData const* data_;
    AudioStream* stream_;
    AudioMixer* mixer_;
    AudioDevice device_;
    float localVolume_, volume_;
    bool loop_;

    static void Update(void* userdata, AudioStream* stream, int additional_amount, int total_amount);
    void reset();
public:
    explicit AudioClip(AudioClipData const* data);
    ~AudioClip();
    bool play();
    bool stop();
    void pause();
    bool resume();
    bool isPlaying() const;
    bool isPaused() const;
    float getVolume() const;
    void setVolume(float volume);
    bool isLooped() const;
    void setLoop(bool loop);
    void updateVolume();
    void assignMixer(AudioMixer* mixer);
    void assignDevice(AudioDevice device);
};


#endif //AUDIOCLIP_H
