#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>
#include <vector>
#include "Structs.h"
#include "Animation.h"
#include "Interface.h"
#include "Camera.h"
#include "Inventory.h"




class Player {
public:
    Player(SDL_Renderer* renderer, TTF_Font* font, Camera* camera);
    ~Player();

    void obrabotkaklavish(SDL_Event* event);
    void obnovleniepersa();
    void otrisovka();

    void setPosition(float x, float y);
    void setCollisions(const std::vector<SDL_FRect>& rects);

    SDL_FRect gedDest() { return dest; }
    void addMoney(int addedMoney);

    void takeDamage(int amount);
    int getHealth() const;
    bool isDead() const;
    bool isAttacking() const;
    bool readyToDealDamage() const;

private:
    void setAnim(const std::string& animName);
    SDL_FRect hitbox;
    void updateHitbox();
    bool isOnGround = false;
    bool inventoryOpen = false;
    void defineLook(const bool* keys);
    void attackHandler();
    void moveHandler(const bool* keys);
    void initAnimations();

    SDL_Renderer* renderer;
    TTF_Font* font;
    Interface* interface;
    Camera* camera;
    Inventory* inventory;

    std::vector<SDL_FRect> collisionRects;

    std::map<std::string, AnimationSet> animations;
    std::string currentAnim = "idle";
    Animation animationHandler;

    SDL_FRect src;
    SDL_FRect dest;
    SDL_FlipMode flip;

    bool readyToHit = false;
    int speed;
    int currentHealth;
    int TotalHealth;
    int money = 0;

    bool isWalk = false;
    bool isAttack = false;
    bool isjump = false;
    bool isRunning = false;

    int velocityY = 0;
    static const int gravity = 1;
    static const int sila_prizhka = -15;

};