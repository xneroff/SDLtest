#pragma once
#include <vector>
#include <string>
#include <SDL3/SDL.h>

struct Item {
    std::string name;
    SDL_Texture* icon;
    int quantity;
};

class Inventory {
public:
    Inventory(SDL_Renderer* renderer);
    ~Inventory();

    void addItem(const std::string& name, SDL_Texture* icon, int quantity);
    void removeItem(const std::string& name);
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* inventoryBackground;
    std::vector<SDL_FRect> itemSlots;

    std::vector<Item> items;
    SDL_Renderer* renderer;
};
