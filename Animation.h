#pragma once
#include <SDL3/SDL.h>
#include "Structs.h"

class Animation {
public:
    Animation();
    ~Animation();
    void update(const AnimationSet& anim, SDL_FRect& src, int frameWidth);
    void reset();
    int getCurrentFrame() const { return currentFrame; }

private:
    int currentFrame = 0;
    Uint64 lastUpdate = 0;
};