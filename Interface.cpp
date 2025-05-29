#include "Interface.h"

Interface::Interface(SDL_Renderer* renderer, TTF_Font* font, int currentHealth, int TotalHealth): renderer(renderer), font(font)
{
	playerMoneyHUD = new  PlayerMoneyHUD(renderer, font);
	playerHealthHUD = new  PlayerHealthHUD(renderer, font,  currentHealth,TotalHealth);
}

Interface::~Interface()
{
	TTF_CloseFont(font);
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
}

void Interface::setMoney(int newMoney) 
{
	playerMoneyHUD->setMoney(newMoney);
}
