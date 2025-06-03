#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class PlayerHealthHUD {
public:
    PlayerHealthHUD(SDL_Renderer* renderer, TTF_Font* font, int health, int totalHealth);
    ~PlayerHealthHUD();

    void obnovlenieHUD();
    void otrisovka() const;
    void setHealth(int newHealth);

private:
    void updateText();

    int Health;
    int TotalHealth;
    std::string Text;
    SDL_Color TextColor = { 255, 255, 255, 255 };

    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* Texture = nullptr;

    SDL_FRect destHealthBar;
    SDL_FRect destTotalHealthBar;
    SDL_FRect destBorderHealthBar;
    SDL_FRect destText;
};
