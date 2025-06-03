#pragma once
#include <SDL3/SDL.h>
#include <map>
#include <string>
#include "Animation.h"
#include "Camera.h"

class Enemy {
public:
    Enemy(SDL_Renderer* renderer, float x, float y);
    ~Enemy();

    void update(const SDL_FRect& playerPos);
    void render(SDL_Renderer* renderer, Camera* camera);

    SDL_FRect getRect() const;
    bool canAttack(Uint32 now);
    bool isAttacking() const;
    bool isDead() const;
    void takeDamage(int amount);

private:
    void initAnimations();

    SDL_Renderer* renderer;
    SDL_FRect dest;
    SDL_FRect src;
    SDL_FlipMode flip;

    std::map<std::string, AnimationSet> animations;
    std::string currentAnim = "idle";
    Animation animationHandler;

    float speed = 2.0f;
    int health = 100;
    bool dead = false;

    Uint32 lastAttackTime = 0;
    Uint32 lastHitTime = 0;
};
