#pragma once
#include <SDL3/SDL.h>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);
    ~Camera();
    void update(const SDL_FRect& player, float mapWidth, float mapHeight);
    SDL_FRect getView() const { return camera; }

    SDL_FRect apply(const SDL_FRect& worldRect) const;

private:
    SDL_FRect camera;
    float screenW, screenH;
};