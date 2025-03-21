#include "Animation.h"

Animation::Animation(std::string const &path) : Resource(path), frameTime(0), loop(false), numFrames(0) {
}

bool Animation::load() {
    frames = {};
    frameTime = 0.5;
    loop = false;
    numFrames = static_cast<int>(frames.size());
    return true;
}

void Animation::unload() {
}
