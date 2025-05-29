#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
class PlayerMoneyHUD
{
public:
    PlayerMoneyHUD(SDL_Renderer* renderer, TTF_Font* font);
    ~PlayerMoneyHUD();
    void obnovlenieHUD();
    void otrisovka() const;
    void setMoney(int newMoney) { money = newMoney; };
    void changeMoney();

private:
    void obnovlenieText();
    int money;
    std::string Text;
    SDL_Color TextColor = { 255, 255, 255, 255 };  // Цвет текста для денег
    SDL_Renderer* renderer;  // Рендерер
    TTF_Font* font;  // Шрифт для текста
    SDL_FRect dest  ;  // Прямоугольник для текста с деньгами
    SDL_Texture* Texture;  // Текстура для текста с деньгами
};

