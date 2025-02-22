#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H
#include <cstdint>
#include <vector>

class AudioClip;
typedef uint32_t AudioDevice;

class AudioMixer {
private:
    AudioDevice audioDevice_;
    AudioMixer* output_;
    std::vector<AudioMixer*> mixers_;
    std::vector<AudioClip*> clips_;

    float localVolume_, volume_;
public:
    AudioMixer();
    void connect(AudioClip* clip);
    void connect(AudioMixer* mixer);
    void setVolume(float volume);
    float getGlobalVolume() const;
    void updateVolume();
    void assignDevice(AudioDevice device);
    AudioDevice getDevice() const;
};


#endif //AUDIOMIXER_H
