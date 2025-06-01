#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "PlayerMoneyHUD.h"
#include "PlayerHealthHUD.h"
#include <vector>
class Interface
{
public:
    Interface(SDL_Renderer* renderer, TTF_Font* font, int currentHealth, int TotalHealth);
    ~Interface();
    void obnovlenieHUD();
    void otrisovka() const;
    void setMoney(int newMoney);
    void changeMoney();
    InventorySlot slots[5];

private:
    SDL_Texture* slotTex = nullptr;
    PlayerMoneyHUD* playerMoneyHUD;
    PlayerHealthHUD* playerHealthHUD;
    SDL_Renderer* renderer;
    TTF_Font* font;
};
