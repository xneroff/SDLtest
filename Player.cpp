#include "Player.h"
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <iostream>


Player::Player(SDL_Renderer* renderer, TTF_Font* font, Camera* camera)
    : renderer(renderer), font(font), camera(camera)
{
    initAnimations();
    src = { 0, 0, 48, 48 };
    dest = { 0, 0, 86, 86 }; // Увеличено 2x
    speed = 5;
    currentHealth = 100;
    TotalHealth = 100;
    interface = new Interface(renderer, font, currentHealth, TotalHealth);
    inventory = new Inventory(renderer);
    inventory->addItem("Topor", "assets/MoiInventory/Topor.png");
    inventory->addItem("eda", "assets/MoiInventory/eda.png");
}

Player::~Player() {
    delete interface;
}


bool Player::readyToDealDamage() const {
    return readyToHit;
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

    tex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_hurt.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["hit"] = { tex, 3, 100 };
    }

    tex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_death.png");
    if (tex) {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        animations["death"] = { tex, 6, 150 };
    }
}

void Player::otrisovka() {
    const AnimationSet& anim = animations[currentAnim];
    SDL_FRect screenDest = camera->apply(dest);
    SDL_RenderTextureRotated(renderer, anim.texture, &src, &screenDest, 0, nullptr, flip);

    interface->otrisovka();
    if (inventoryOpen) {
        inventory->render();
    }

    // Хитбокс — красный прямоугольник
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 120);
    SDL_FRect screenHitbox = camera->apply(hitbox);
    SDL_RenderRect(renderer, &screenHitbox);
}


void Player::addMoney(int addedMoney) {
    money += addedMoney;
    interface->setMoney(money);
}

void Player::defineLook(const bool* keys) {
    bool wasFlipped = (flip == SDL_FLIP_HORIZONTAL); // Запоминаем предыдущее состояние

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

    // ✅ Если `flip` изменился, корректируем `dest.x`, чтобы персонаж не смещался
    if (wasFlipped != (flip == SDL_FLIP_HORIZONTAL)) {
        dest.x += (flip == SDL_FLIP_HORIZONTAL) ? -20 : 20; // Коррекция смещения
    }
}



void Player::setAnim(const std::string& animName) {
    if (currentAnim != animName) {
        currentAnim = animName;
        animationHandler.reset();
    }
}


void Player::attackHandler() {
    if (isAttack) {
        currentAnim = "attack";
        animationHandler.update(animations[currentAnim], src, (int)src.w);
        int frameIndex = static_cast<int>(src.x / src.w);
        readyToHit = (frameIndex >= animations[currentAnim].frameCount - 1);

    }
    else {
        readyToHit = false;
    }

}




//void Player::updateHitbox() {
//    hitbox.w = dest.w - 50; // ✅ Уменьшаем ширину
//    hitbox.h = dest.h - +25;  // ✅ Чуть уменьшаем высоту
//
//    // 🏷 Смещаем хитбокс так, чтобы он остался по центру персонажа
//    hitbox.x = dest.x + 15;
//    hitbox.y = dest.y + 20;
//}
void Player::updateHitbox() {
    hitbox.w = dest.w - 50; // ✅ Уменьшаем ширину
    hitbox.h = dest.h - 25; // ✅ Чуть уменьшаем высоту
    hitbox.y = dest.y + 20; // ✅ Оставляем одинаковую высоту

    if (flip == SDL_FLIP_HORIZONTAL) {
        hitbox.x = dest.x + 35; // ✅ Смещаем влево при повороте
    }
    else {
        hitbox.x = dest.x + 15; // ✅ Оставляем как есть при движении вправо
    }

}
    
void Player::moveHandler(const bool* keys) {
    isWalk = false;
    isRunning = false;
    defineLook(keys);

    int actualSpeed = speed;
    if (keys[SDL_SCANCODE_LSHIFT]) {
        isRunning = true;
        actualSpeed = speed * 2;
    }

    bool moveLeft = keys[SDL_SCANCODE_A];
    bool moveRight = keys[SDL_SCANCODE_D];
    
    float oldX = dest.x;

    if (moveLeft && !moveRight) {
        dest.x -= actualSpeed;
        flip = SDL_FLIP_HORIZONTAL;
        isWalk = true;
    }
    else if (moveRight && !moveLeft) {
        dest.x += actualSpeed;
        flip = SDL_FLIP_NONE;
        isWalk = true;
    }
    else {
        isWalk = false;
    }

    for (const auto& rect : collisionRects) {
        if (SDL_HasRectIntersectionFloat(&hitbox, &rect)) {
            dest.x = oldX; // откат назад
            break;
        }
    }

    // Прыжок
    if (keys[SDL_SCANCODE_SPACE] && isOnGround) {
        velocityY = sila_prizhka;
        isjump = true;
        /*isOnGround = false;*/
    }

    // Применение гравитации
    if (!isOnGround) { // Гравитация только если персонаж НЕ стоит на земле
        velocityY += gravity;
    }

    dest.y += velocityY;

    updateHitbox();
    bool wasOnGround = isOnGround; // Запоминаем состояние до проверки
    isOnGround = false;

    for (const auto& rect : collisionRects) {
        if (SDL_HasRectIntersectionFloat(&hitbox, &rect)) {
            // Проверяем, если персонаж падает и достигает платформы
            if (velocityY > 0 && hitbox.y + hitbox.h - velocityY <= rect.y) {
                dest.y = rect.y - dest.h;   
                isjump = false;
                isOnGround = true;
            }
            else if (velocityY < 0 && hitbox.y - velocityY >= rect.y + rect.h) {
                dest.y = rect.y + rect.h;
                
            }
            break;
        }
    }

    std::string prevAnim = currentAnim;

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


    

    
    updateHitbox();
}





void Player::setPosition(float x, float y) {
    dest.x = x;
    dest.y = y - dest.h; // Смещаем вниз, чтобы не оказаться ВНУТРИ блока
    updateHitbox();
}

void Player::setCollisions(const std::vector<SDL_FRect>& rects) {
    collisionRects = rects;
}

void Player::obnovleniepersa() {
    if (isDead()) return;

    const bool* keys = SDL_GetKeyboardState(nullptr);
    moveHandler(keys);
    attackHandler();
    interface->obnovlenieHUD();
}

    
void Player::obrabotkaklavish(SDL_Event* event) {
    if (inventoryOpen) {
        inventory->handleEvent(event);  // Перетаскивание работает
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
        if (!inventoryOpen) { // 🚫 Не атакуем, если инвентарь открыт
            isAttack = true;
            animationHandler.reset();
        }
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT) {
        isAttack = false;
        currentAnim = "idle";
    }

    // ✅ Переключение инвентаря по клавише I
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_I) {
        inventoryOpen = !inventoryOpen;
        std::cout << "📦 Inventory status: " << (inventoryOpen ? "OPEN" : "CLOSED") << std::endl;
    }
}



void Player::takeDamage(int amount) {
    if (currentHealth <= 0) return;

    currentHealth -= amount;
    if (currentHealth <= 0) {
        currentHealth = 0;
        currentAnim = "death";
    }
    else {
        currentAnim = "hit";
    }

    animationHandler.reset();
    interface->setHealth(currentHealth);
}


bool Player::isAttacking() const {
    return isAttack;
}

int Player::getHealth() const {
    return currentHealth;
}

bool Player::isDead() const {
    return currentHealth <= 0;
}
