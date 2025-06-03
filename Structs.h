// Structs.h
#pragma once
#include <SDL3/SDL.h>

struct AnimationSet {
    SDL_Texture* texture;
    int frameCount;
    int frameDelay;
};
