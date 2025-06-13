#include "Inventory.h"
#include <iostream>

Inventory::Inventory(SDL_Renderer* renderer) : renderer(renderer) {
    background = IMG_LoadTexture(renderer, "assets/MoiInventory/Inventory_style_02d.png");
    SDL_SetTextureScaleMode(background, SDL_SCALEMODE_NEAREST);
    slotHighlight = IMG_LoadTexture(renderer, "assets/MoiInventory/Inventory_select.png");
    SDL_SetTextureScaleMode(slotHighlight, SDL_SCALEMODE_NEAREST);
    previewRect = { 662, 450, 104, 140 };

    SDL_Texture* idleTex = IMG_LoadTexture(renderer, "assets/1 Woodcutter/Woodcutter_idle.png");
    if (idleTex) {
        SDL_SetTextureScaleMode(idleTex, SDL_SCALEMODE_NEAREST);
        previewAnim = { idleTex, 4, 200 };  // 4 кадра, задержка 200 мс
    }


    // Центр инвентаря
    inventoryRect = { 554, 306, 812, 468 };  // 203x117 * 4

    // Слоты (абсолютные позиции под фон)
    slots = {
        {586, 338, 64, 64}, {662, 338, 64, 64}, {738, 338, 64, 64},
        {814, 338, 64, 64}, {890, 338, 64, 64}, {966, 338, 64, 64},
        {1042, 338, 64, 64}, {1118, 338, 64, 64}, {1194, 338, 64, 64},
        {1270, 338, 64, 64}, //Верхняя панель инвентаря 

        {586, 450, 64, 64}, {586, 526, 64, 64}, {662, 450, 104, 140},//ДЛЯ ПЕРСОНАЖА КООРДИНАТЫ
        {778, 450, 64, 64}, {778, 526, 64, 64},

        {586,602,256,140},//Для описания предмета

        {890, 450, 64, 64}, {966, 450, 64, 64}, {1042, 450, 64, 64}, {1118, 450, 64, 64}, {1194, 450, 64, 64},
        {966, 450, 64, 64}, {1042, 450, 64, 64}, {1118, 450, 64, 64}, {1194, 450, 64, 64}, {1270, 450, 64, 64},//1 СТРОКА инвентаря

        {890, 526, 64, 64}, {966, 526, 64, 64}, {1042, 526, 64, 64}, {1118, 526, 64, 64}, {1194, 526, 64, 64},
        {966, 526, 64, 64}, {1042, 526, 64, 64}, {1118, 526, 64, 64}, {1194, 526, 64, 64}, {1270, 526, 64, 64},//2 СТРОКА инвентаря

        {890, 602, 64, 64}, {966, 602, 64, 64}, {1042, 602, 64, 64}, {1118, 602, 64, 64}, {1194, 602, 64, 64},
        {966, 602, 64, 64}, {1042, 602, 64, 64}, {1118, 602, 64, 64}, {1194, 602, 64, 64}, {1270, 602, 64, 64},//3 СТРОКА инвентаря

        {890, 678, 64, 64}, {966, 678, 64, 64}, {1042, 678, 64, 64}, {1118, 678, 64, 64}, {1194, 678, 64, 64},
        {966, 678, 64, 64}, {1042, 678, 64, 64}, {1118, 678, 64, 64}, {1194, 678, 64, 64}, {1270, 678, 64, 64},//4 СТРОКА инвентаря

    };
}

Inventory::~Inventory() {
    SDL_DestroyTexture(background);
    for (auto& item : items)
        SDL_DestroyTexture(item.texture);
    SDL_DestroyTexture(slotHighlight);

}

SDL_Texture* Inventory::loadTexture(const std::string& path) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (!tex) std::cerr << "❌ Не удалось загрузить: " << path << std::endl;
    else SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
    return tex;
}

void Inventory::addItem(const std::string& name, const std::string& path) {
    if (items.size() >= slots.size()) return;

    SDL_Texture* tex = loadTexture(path);
    if (!tex) return;

    SDL_FRect rect = slots[items.size()];
    items.push_back({ name, tex, rect });
}

void Inventory::render() {
    if (background) {
        SDL_RenderTexture(renderer, background, nullptr, &inventoryRect);
    }

    float mx, my;
    SDL_GetMouseState(&mx, &my);

    // Отрисовать выделение, если курсор над каким-то слотом
    for (const auto& slot : slots) {
        // Пропускаем прямоугольник персонажа
        if (slot.x == previewRect.x && slot.y == previewRect.y &&
            slot.w == previewRect.w && slot.h == previewRect.h) {
            continue;
        }

        if (mx > slot.x && mx < slot.x + slot.w &&
            my > slot.y && my < slot.y + slot.h) {
            if (slotHighlight) {
                SDL_FRect outlineRect = {
                    slot.x - 4,
                    slot.y - 4,
                    slot.w + 8,
                    slot.h + 8
                };
                SDL_RenderTexture(renderer, slotHighlight, nullptr, &outlineRect);
            }
            break;
        }
    }


    // 🎮 Отображение idle-анимации персонажа в превью-окне
    if (previewAnim.texture) {
        SDL_FRect src;
        src.x = previewHandler.getCurrentFrame() * 48;  // кадры идут по ширине
        src.y = 0;
        src.w = 48;
        src.h = 48;

        previewHandler.update(previewAnim, src, 48);

        SDL_FRect dst;
        dst.x = previewRect.x;
        dst.y = previewRect.y - 30;
        dst.w = previewRect.w + 80;
        dst.h = previewRect.h + 30;

        SDL_RenderTexture(renderer, previewAnim.texture, &src, &dst);
    }



    // Предметы
    for (const auto& item : items) {
        if (&item == draggingItem) continue;
        SDL_RenderTexture(renderer, item.texture, nullptr, &item.rect);
    }

    if (draggingItem) {
        SDL_RenderTexture(renderer, draggingItem->texture, nullptr, &draggingItem->rect);
    }
}

void Inventory::handleEvent(SDL_Event* event) {
    float mx, my;
    SDL_GetMouseState(&mx, &my);

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
        if (!draggingItem) {
            // Попытка взять предмет
            for (auto& item : items) {
                if (mx > item.rect.x && mx < item.rect.x + item.rect.w &&
                    my > item.rect.y && my < item.rect.y + item.rect.h) {
                    draggingItem = &item;
                    draggingItemOriginalRect = item.rect;
                    break;
                }
            }
        }
        else {
            // Пытаемся положить предмет в слот
            SDL_FRect* closestSlot = nullptr;
            float minDist = std::numeric_limits<float>::max();

            for (auto& slot : slots) {
                float dx = mx - (slot.x + slot.w / 2.0f);
                float dy = my - (slot.y + slot.h / 2.0f);
                float dist = dx * dx + dy * dy;

                if (dist < minDist) {
                    minDist = dist;
                    closestSlot = &slot;
                }
            }

            // Проверка: занят ли слот
            bool slotOccupied = false;
            for (const auto& item : items) {
                if (&item != draggingItem &&
                    item.rect.x == closestSlot->x &&
                    item.rect.y == closestSlot->y) {
                    slotOccupied = true;
                    break;
                }
            }

            if (!slotOccupied && closestSlot) {
                // ❌ Запрещаем класть в слот персонажа (previewRect)
                if (!(closestSlot->x == previewRect.x &&
                    closestSlot->y == previewRect.y &&
                    closestSlot->w == previewRect.w &&
                    closestSlot->h == previewRect.h)) {

                    draggingItem->rect.x = closestSlot->x;
                    draggingItem->rect.y = closestSlot->y;
                }
                else {
                    // откат, если пытались положить в previewRect
                    draggingItem->rect = draggingItemOriginalRect;
                }
            }
            else {
                draggingItem->rect = draggingItemOriginalRect; // откат
            }

        

            draggingItem = nullptr; // отпускаем предмет
        }
    }

    if (draggingItem) {
        // предмет следует за курсором
        draggingItem->rect.x = mx - draggingItem->rect.w / 2.0f;
        draggingItem->rect.y = my - draggingItem->rect.h / 2.0f;
    }

}