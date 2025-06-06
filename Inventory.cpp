#include "Inventory.h"
#include <SDL3_image/SDL_image.h>

Inventory::Inventory(SDL_Renderer* renderer) : renderer(renderer) {
    inventoryBackground = IMG_LoadTexture(renderer, "assets/ui/inventory_bg.png");
    int startX = (1920 - 1000) / 2 + 50;
    int startY = (1080 - 800) / 2 + 50;
    int cellSize = 64;
    int spacing = 10;
    int columns = 5;

    for (int i = 0; i < 20; ++i) {
        int x = startX + (i % columns) * (cellSize + spacing);
        int y = startY + (i / columns) * (cellSize + spacing);
        SDL_FRect slot = { x, y, cellSize, cellSize };
        itemSlots.push_back(slot);

    }

}

Inventory::~Inventory() {}

void Inventory::addItem(const std::string& name, SDL_Texture* icon, int quantity) {
    items.push_back({ name, icon, quantity });
}

void Inventory::removeItem(const std::string& name) {
    items.erase(std::remove_if(items.begin(), items.end(),
        [&](const Item& item) { return item.name == name; }), items.end());
}

void Inventory::render(SDL_Renderer* renderer) {
    if (items.empty()) return; // Если нет предметов, не рисуем

    // Центрируем инвентарь
    int screenWidth = 1920; // Укажи реальное разрешение
    int screenHeight = 1080;
    SDL_FRect inventoryRect = { (screenWidth - 1000) / 2, (screenHeight - 800) / 2, 1000, 800 };

    // Рисуем фон
    SDL_RenderTexture(renderer, inventoryBackground, nullptr, &inventoryRect);

    // Отрисовка предметов в ячейках
    int startX = inventoryRect.x + 50;
    int startY = inventoryRect.y + 50;
    int cellSize = 64;
    int spacing = 10;
    int columns = 5;

    for (size_t i = 0; i < items.size(); ++i) {
        int x = startX + (i % columns) * (cellSize + spacing);
        int y = startY + (i / columns) * (cellSize + spacing);
        SDL_FRect itemRect = { x, y, cellSize, cellSize };
        SDL_RenderTexture(renderer, items[i].icon, nullptr, &itemRect);
    }
}



