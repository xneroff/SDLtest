#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <map>
#include <string>
#include "Structs.h"
#include "Animation.h"
#include "Interface.h"
#include "Camera.h"

class Player {
public:
    Player(SDL_Renderer* renderer, TTF_Font* font, Camera* camera);
    ~Player();
    void loadAnimation(const std::string& name, const std::string& path, int frames, int delay);

    void setPosition(float x, float y);
    void setCollisions(const std::vector<SDL_FRect>& rects);
    void obrabotkaklavish(SDL_Event* event);
    void obnovleniepersa();
    void otrisovka();
    SDL_FRect gedDest() { return dest; }
    void addMoney(int addedMoney);

private:
    bool checkCollision(float nextX, float nextY);
    void defineLook(const bool* keys);
    void attackHandler();
    void moveHandler(const bool* keys);
    void initAnimations();

    const std::vector<SDL_FRect>* collisionRects = nullptr;

    SDL_Renderer* renderer;
    TTF_Font* font;
    Interface* interface;
    Camera* camera;

    std::map<std::string, AnimationSet> animations;
    std::string currentAnim = "idle";
    Animation animationHandler;

    SDL_FRect src;
    SDL_FRect dest;
    SDL_FlipMode flip;

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
