#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight, float deadZoneWidth, float deadZoneHeight)
	: screenW(screenWidth), screenH(screenHeight), dzW(deadZoneWidth), dzH(deadZoneHeight)
{
    camera = { 0, 0, screenWidth, screenHeight };

    deadZone.w = deadZoneWidth;
    deadZone.h = deadZoneHeight;
    deadZone.x = camera.x + (camera.w - deadZone.w) / 2;
    deadZone.y = camera.y + (camera.h - deadZone.h) / 2;
}

Camera::~Camera() {}

void Camera::update(const SDL_FRect& player) {
    if (player.x < deadZone.x) {
        camera.x -= deadZone.x - player.x;
    }
    else if (player.x + player.w > deadZone.x + deadZone.w) {
        camera.x += (player.x + player.w) - (deadZone.x + deadZone.w);
    }

    if (player.y < deadZone.y) {
        camera.y -= deadZone.y - player.y;
    }
    else if (player.y + player.h > deadZone.y + deadZone.h) {
        camera.y += (player.y + player.h) - (deadZone.y + deadZone.h);
    }

    // Обновляем позицию deadZone
    deadZone.x = camera.x + (camera.w - deadZone.w) / 2;
    deadZone.y = camera.y + (camera.h - deadZone.h) / 2;
}

SDL_FRect Camera::apply(const SDL_FRect& worldRect) const {
    return SDL_FRect{
        worldRect.x - camera.x,
        worldRect.y - camera.y,
        worldRect.w,
        worldRect.h
    };
}