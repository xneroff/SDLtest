#include "Enemy.h"
#include <SDL3_image/SDL_image.h>
#include "Player.h"

Enemy::Enemy(SDL_Renderer* renderer, float x, float y) : renderer(renderer) {
    dest = { x, y, 96, 96 };
    src = { 0, 0, 48, 48 };
    initAnimations();
}

Enemy::~Enemy() {

}

void Enemy::initAnimations() {
    SDL_Texture* tex = IMG_LoadTexture(renderer, "assets/1 Enemies/2/Walk.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["walk"] = { tex, 6, 150 };
    }

    SDL_Texture* idleTex = IMG_LoadTexture(renderer, "assets/1 Enemies/2/Idle.png");
    if (idleTex) {
        SDL_SetTextureScaleMode(idleTex, SDL_SCALEMODE_NEAREST);
        animations["idle"] = { idleTex, 4, 200 };
    }

    SDL_Texture* attackTex = IMG_LoadTexture(renderer, "assets/1 Enemies/2/Attack.png");
    if (attackTex) {
        SDL_SetTextureScaleMode(attackTex, SDL_SCALEMODE_NEAREST);
        animations["attack"] = { attackTex, 6, 120 };
    }

    SDL_Texture* hurtTex = IMG_LoadTexture(renderer, "assets/1 Enemies/2/Hurt.png");
    if (hurtTex) {
        SDL_SetTextureScaleMode(hurtTex, SDL_SCALEMODE_NEAREST);
        animations["hit"] = { hurtTex, 2, 150 };
    }

    SDL_Texture* deathTex = IMG_LoadTexture(renderer, "assets/1 Enemies/2/Death.png");
    if (deathTex) {
        SDL_SetTextureScaleMode(deathTex, SDL_SCALEMODE_NEAREST);
        animations["death"] = { deathTex, 6, 150 };
    }
}

void Enemy::update(const SDL_FRect& playerPos) {
    if (dead) {
        currentAnim = "death";
        animationHandler.update(animations[currentAnim], src, (int)src.w);
        return;
    }

    const float groundY = 800.0f;
    if (dest.y < groundY) dest.y += 5;
    if (dest.y > groundY) dest.y = groundY;

    float dx = playerPos.x - dest.x;

    if (SDL_HasRectIntersectionFloat(&dest, &playerPos)) {
        currentAnim = "attack";
    }
    else if (fabsf(dx) > 10) {
        currentAnim = "walk";
        dest.x += (dx > 0 ? speed : -speed);
        flip = dx > 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    }
    else {
        currentAnim = "idle";
    }

    animationHandler.update(animations[currentAnim], src, (int)src.w);

    // Атака срабатывает на последнем кадре удара

}

void Enemy::render(SDL_Renderer* renderer, Camera* camera) {
    SDL_FRect screenDest = camera->apply(dest);
    SDL_RenderTextureRotated(renderer, animations[currentAnim].texture, &src, &screenDest, 0, nullptr, flip);
}

SDL_FRect Enemy::getRect() const {
    return dest;
}

bool Enemy::canAttack(Uint32 now) {
    if (now - lastAttackTime > 1200) {
        lastAttackTime = now;
        return true;
    }
    return false;
}

bool Enemy::isAttacking() const {
    return currentAnim == "attack" && !dead;
}

void Enemy::takeDamage(int amount) {
    if (dead) return;
    health -= amount;
    if (health <= 0) {
        health = 0;
        dead = true;
        currentAnim = "death";
    }
    else {
        currentAnim = "hit";
    }
    animationHandler.reset();
}

bool Enemy::isDead() const {
    return dead;
}
