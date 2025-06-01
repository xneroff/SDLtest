#include "Interface.h"
#include "Structs.h"
#include "SDL3/SDL3.h"
Interface::Interface(SDL_Renderer* renderer, TTF_Font* font, int currentHealth, int TotalHealth): renderer(renderer), font(font)
{
	playerMoneyHUD = new  PlayerMoneyHUD(renderer, font);
	playerHealthHUD = new  PlayerHealthHUD(renderer, font,  currentHealth, TotalHealth);
    slotTex = IMG_(renderer, "assets/ui/slot.png");
}

Interface::~Interface()
{
	TTF_CloseFont(font);
    if (slotTex) SDL_DestroyTexture(slotTex);
    for (int i = 0; i < 5; ++i)
        if (slots[i].icon) SDL_DestroyTexture(slots[i].icon);
} 

void Interface::obnovlenieHUD()
{
	playerMoneyHUD->obnovlenieHUD();
	playerHealthHUD->obnovlenieHUD();
}


void Interface::otrisovka()const
{
	playerMoneyHUD->otrisovka();
	playerHealthHUD->otrisovka();
    float x = 20;
    float y = 100;
    float size = 64;

    SDL_FRect slotRect = { 30, 150, 64, 64 };  // слева на экране
    for (int i = 0; i < 5; ++i) {
        if (slotTex)
            SDL_RenderTexture(renderer, slotTex, nullptr, &slotRect);

        if (slots[i].icon) {
            SDL_FRect iconRect = { slotRect.x + 8, slotRect.y + 8, 48, 48 };
            SDL_RenderTexture(renderer, slots[i].icon, nullptr, &iconRect);
        }

        slotRect.y += 70;  // вертикальное смещение
    }
}

void Interface::setMoney(int newMoney) 
{
	playerMoneyHUD->setMoney(newMoney);
}
