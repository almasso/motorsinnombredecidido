#ifndef AUDIOCLIPDATA_H
#define AUDIOCLIPDATA_H
#include <cstdint>

#include <Load/Resource.h>

typedef struct SDL_AudioSpec AudioSpec;
typedef uint8_t* AudioBuffer;

struct AudioClipData final : public Resource {
    AudioBuffer buffer;
    uint32_t bufferLen;
    AudioSpec* specifier;
    explicit AudioClipData(std::string const& path);
    ~AudioClipData() override;
    bool load() override;
    void unload() override;
};

#endif //AUDIOCLIPDATA_H
