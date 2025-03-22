#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <Core/ComponentTemplate.h>

class AudioClip;
class AudioMixer;

class AudioSource : public ComponentTemplate<"AudioSource"> {
private:
    AudioClip* _clip;
    AudioMixer* _mixer;
public:
    AudioSource();
    ~AudioSource() override;
    bool init() override;
    void onEnable() override;
    void onDisable() override;
    void play();
    void stop();
    void pause();
    void resume();

    bool isPlaying() const;

    bool isPaused() const;

    float getVolume() const;

    void setVolume(float volume);

    bool isLooped() const;

    void setLoop(bool loop);
};



#endif //AUDIOSOURCE_H
