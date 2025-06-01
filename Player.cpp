#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <vector>

Player::Player(SDL_Renderer* renderer, TTF_Font* font, Camera* camera)
    : renderer(renderer), font(font), camera(camera)
{
    initAnimations();
    src = { 0, 0, 48, 48 };
    dest = { 200, 800, 96, 96 }; // Увеличено 2x
    speed = 5;
    currentHealth = 50;
    TotalHealth = 100;
    interface = new Interface(renderer, font, currentHealth, TotalHealth);
    SDL_Texture* icon = IMG_LoadTexture(renderer, "assets/items/wood.png");
    if (icon) {
        interface->slots[0].icon = icon;
        interface->slots[0].count = 99;
    }
}

Player::~Player() {
    /*for (auto& [_, anim] : animations) {
        if (anim.texture) SDL_DestroyTexture(anim.texture);
    }*/
    delete interface;
}

void Player::initAnimations() {
    SDL_Texture* tex = nullptr;

    tex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_idle.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["idle"] = { tex, 4, 200 };
    }

    tex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_walk.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["walk"] = { tex, 6, 100 };
    }

    tex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_run.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["run"] = { tex, 6, 100 };
    }

    tex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_attack2.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["attack"] = { tex, 6, 125 };
    }

    tex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_jump.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["jump"] = { tex, 6, 150 };
    }
}


void Player::otrisovka() {
    const AnimationSet& anim = animations[currentAnim];
    SDL_FRect screenDest = camera->apply(dest);
    SDL_RenderTextureRotated(renderer, anim.texture, &src, &screenDest, 0, nullptr, flip);
    interface->otrisovka();
}

void Player::addMoney(int addedMoney) {
    money += addedMoney;
    interface->setMoney(money);
}

void Player::defineLook(const bool* keys) {
    if (keys[SDL_SCANCODE_A]) {
        flip = SDL_FLIP_HORIZONTAL;
        currentAnim = "walk";
    }
    else if (keys[SDL_SCANCODE_D]) {
        flip = SDL_FLIP_NONE;
        currentAnim = "walk";
    }
    else {
        currentAnim = "idle";
    }
}

void Player::attackHandler() {
    if (isAttack) {
        currentAnim = "attack";
        animationHandler.update(animations[currentAnim], src, (int)src.w);
    }
}

void Player::moveHandler(const bool* keys) {
    isWalk = false;
    isRunning = false;
    defineLook(keys);

    int actualSpeed = speed;

    if (keys[SDL_SCANCODE_LSHIFT]) {
        isRunning = true;
        actualSpeed = speed * 2;  // ускорение
    }

    if (keys[SDL_SCANCODE_SPACE] && !isjump) {
        velocityY = sila_prizhka;
        isjump = true;
    }

    if (isjump) {
        dest.y += velocityY;
        velocityY += gravity;
        currentAnim = "jump";
    }

    if (dest.y >= 250) {
        dest.y = 250;
        isjump = false;
        velocityY = 0;
    }

    if (keys[SDL_SCANCODE_A]) {
        dest.x -= actualSpeed;
        flip = SDL_FLIP_HORIZONTAL;
        isWalk = true;
    }
    if (keys[SDL_SCANCODE_D]) {
        dest.x += actualSpeed;
        flip = SDL_FLIP_NONE;
        isWalk = true;
    }

    // Выбор анимации
    if (!isAttack) {
        if (isjump) {
            currentAnim = "jump";
        }
        else if (isWalk) {
            currentAnim = isRunning ? "run" : "walk";
        }
        else {
            currentAnim = "idle";
        }
        animationHandler.update(animations[currentAnim], src, (int)src.w);
    }
}


void Player::setPosition(float x, float y) {
    dest.x = x;
    dest.y = y;
}
void Player::setCollisions(const std::vector<SDL_FRect>& rects) {
    collisionRects = rects;
}


void Player::obnovleniepersa() {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    moveHandler(keys);
    attackHandler();
    interface->obnovlenieHUD();
}

void Player::obrabotkaklavish(SDL_Event* event) {
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
        isAttack = true;
        animationHandler.reset();
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT) {
        isAttack = false;
        currentAnim = "idle";
    }
}