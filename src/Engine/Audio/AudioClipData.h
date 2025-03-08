#ifndef AUDIOCLIPDATA_H
#define AUDIOCLIPDATA_H
#include <cstdint>

typedef struct SDL_AudioSpec AudioSpec;
typedef uint8_t* AudioBuffer;

struct AudioClipData {
    AudioBuffer buffer;
    uint32_t bufferLen;
    AudioSpec* specifier;
    ~AudioClipData();
};

#endif //AUDIOCLIPDATA_H
