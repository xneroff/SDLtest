// Animation.cpp
#include "Animation.h"
#include <SDL3/SDL.h>

Animation::Animation() {}

Animation::~Animation() {}

void Animation::reset() {
    currentFrame = 0;
    lastUpdate = SDL_GetTicks();
}

void Animation::update(const AnimationSet& anim, SDL_FRect& src, int frameWidth) {
    Uint64 now = SDL_GetTicks();
    if (now - lastUpdate >= anim.frameDelay) {
        currentFrame = (currentFrame + 1) % anim.frameCount;
        src.x = currentFrame * frameWidth;
        src.y = 0;
        lastUpdate = now;
    }
}
