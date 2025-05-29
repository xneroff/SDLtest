#include "PlayerHealthHUD.h"

PlayerHealthHUD::PlayerHealthHUD(SDL_Renderer* renderer, TTF_Font* font, int currentHealth, int TotalHealth)
	: renderer(renderer), font(font), Health(currentHealth), TotalHealth(TotalHealth)
{
	destHealthBar = { 20, 20, 600, 60 }; // в 2 раза шире
	destBorderHealthBar = { 22, 22, 596, 56 };
	destTotalHealthBar = { 22, 22, 596, 56 };
	float HealthProcent = static_cast <float> (Health) / TotalHealth;
	static_cast <int>  (destHealthBar.w *= HealthProcent);
	updateText();
}

PlayerHealthHUD::~PlayerHealthHUD()
{
}

void PlayerHealthHUD::obnovlenieHUD()
{
	
}

void PlayerHealthHUD::otrisovka() const 
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &destBorderHealthBar);
	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	SDL_RenderFillRect(renderer, &destTotalHealthBar);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &destHealthBar);

	SDL_RenderTexture(renderer, Texture, NULL, &destText);
}

void PlayerHealthHUD::changeMoney()
{

}

void PlayerHealthHUD::updateText()
{
	Text = std::to_string(Health) + "/" + std::to_string(TotalHealth);
	SDL_Surface* surface = TTF_RenderText_Solid(font, Text.c_str(), strlen(Text.c_str()), TextColor);
	Texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_GetTextureSize(Texture, &destText.w, &destText.h);
	destText.x = destBorderHealthBar.x + (destBorderHealthBar.w - destText.w) / 2;
	destText.y = destBorderHealthBar.y + (destBorderHealthBar.h - destText.h) / 2;
	
	SDL_DestroySurface(surface);

	
}

