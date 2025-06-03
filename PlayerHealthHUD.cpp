#include "PlayerHealthHUD.h"

PlayerHealthHUD::PlayerHealthHUD(SDL_Renderer* renderer, TTF_Font* font, int health, int totalHealth)
    : renderer(renderer), font(font), Health(health), TotalHealth(totalHealth)
{
    destHealthBar = { 20, 20, 600, 60 };
    destBorderHealthBar = { 22, 22, 596, 56 };
    destTotalHealthBar = { 22, 22, 596, 56 };

    float percent = static_cast<float>(Health) / TotalHealth;
    destHealthBar.w = 600 * percent;

    updateText();
}

PlayerHealthHUD::~PlayerHealthHUD() {
    if (Texture) SDL_DestroyTexture(Texture);
}

void PlayerHealthHUD::setHealth(int newHealth) {
    Health = newHealth;
    float percent = static_cast<float>(Health) / TotalHealth;
    destHealthBar.w = 600 * percent;
    updateText();
}

void PlayerHealthHUD::obnovlenieHUD() {
    // можно расширить логику при необходимости
}

void PlayerHealthHUD::otrisovka() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &destBorderHealthBar);

    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
    SDL_RenderFillRect(renderer, &destTotalHealthBar);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &destHealthBar);

    if (Texture)
        SDL_RenderTexture(renderer, Texture, nullptr, &destText);
}

void PlayerHealthHUD::updateText() {
    Text = std::to_string(Health) + "/" + std::to_string(TotalHealth);

    if (Texture) SDL_DestroyTexture(Texture);
    SDL_Surface* surface = TTF_RenderText_Solid(font, Text.c_str(), Text.length(), TextColor);
    Texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_GetTextureSize(Texture, &destText.w, &destText.h);
    destText.x = destBorderHealthBar.x + (destBorderHealthBar.w - destText.w) / 2;
    destText.y = destBorderHealthBar.y + (destBorderHealthBar.h - destText.h) / 2;
    SDL_DestroySurface(surface);
}
