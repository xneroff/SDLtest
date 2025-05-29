#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
class PlayerHealthHUD
{
public:
    PlayerHealthHUD(SDL_Renderer* renderer, TTF_Font* font, int currentHealth, int TotalHealth);
    ~PlayerHealthHUD();

    void obnovlenieHUD();
    void otrisovka() const;
    void setMoney(int newHealth) { Health = newHealth; };
    void changeMoney();  

private:
    void updateText();
    int Health;
    int TotalHealth;
    std::string Text;
    SDL_Color TextColor = { 255, 255, 255, 255 };  // Цвет текста для денег
    SDL_Renderer* renderer;  // Рендерер
    TTF_Font* font;  // Шрифт для текста
    SDL_FRect dest;  // Прямоугольник для текста с деньгами
    SDL_Texture* Texture;  // Текстура для текста с деньгами
    SDL_FRect destHealthBar;
    SDL_FRect destTotalHealthBar;
    SDL_FRect destBorderHealthBar;
    SDL_FRect destText;
};