#pragma once
#include <SDL3/SDL.h>

class Camera {
public:
    Camera(float screenWidth, float screenHeight, float deadZoneWidth, float deadZoneHeight);
    ~Camera();
    void update(const SDL_FRect& player);
    SDL_FRect getView() const { return camera; }

    SDL_FRect apply(const SDL_FRect& worldRect) const;

private:
    SDL_FRect camera;
    SDL_FRect deadZone;
    float screenW, screenH;
    float dzW, dzH;
};