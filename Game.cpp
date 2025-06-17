#include "Game.h"
#include "Player.h"
#include <vector>
#include <string>
#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include "TileMap.h"
#include "NPC.h"
#include "Game.h"
std::vector<NPC*> npcs;
std::vector<Enemy*> enemies;

Game::Game() {}

Game::~Game() {}    

SDL_AppResult Game::SDL_AppInit() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_CreateWindowAndRenderer("SDL3 Game", 1920, 1080, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    camera = new Camera(1920, 1080);
    camera->zoom = 2.0f; // увеличить всё в 2 раза

    tileMap = new TileMap(renderer);
    tileMap->loadFromFile("assets/map/MEGATEST.json");
    

    tileMap->renderLayer(renderer, camera, "Tile Layer 1");
    tileMap->renderLayer(renderer, camera, "Tile Layer 2");
    tileMap->renderLayer(renderer, camera, "Tile Layer 3");
    tileMap->renderLayer(renderer, camera, "Tile Layer 4");
    tileMap->renderLayer(renderer, camera, "Tile Layer 5");
    tileMap->renderLayer(renderer, camera, "Tile Layer 6");

    enemies.push_back(new Enemy(renderer, 600, 800));
    enemies.push_back(new Enemy(renderer, 1200, 800));

    npcs.push_back(new NPC(renderer, 400, 1300));
   


    font = TTF_OpenFont("assets/fonts/Jacquard12-Regular.ttf", 34);
    player = new Player(renderer, font, camera);

    player->setCollisions(tileMap->getCollisionRects());
    player->setPosition(tileMap->getSpawnPoint().x, tileMap->getSpawnPoint().y);


    SDL_FPoint spawn = tileMap->getSpawnPoint();
    player->setPosition(spawn.x, spawn.y);
    menu = new MainMenu(renderer, font, window);

    std::srand((unsigned int)time(nullptr));

    return SDL_AppResult();
}

SDL_AppResult Game::SDL_AppEvent(SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) {
        if (showMenu) {
            if (!menu->isInSettings()) showMenu = false;
        }
        else {
            showMenu = true;
        }
    }

    if (showMenu) {
        bool resume = false;
        menu->handleEvent(*event, resume, quit);
        if (resume) showMenu = false;
    }
    else {
        player->obrabotkaklavish(event);
    }

    return quit ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

SDL_AppResult Game::SDL_AppIterate() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    if (showMenu) {
        menu->render();
    }
    else {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        camera->update(player->gedDest(), tileMap->getMapWidth(), tileMap->getMapHeight());
   

        if (keys[SDL_SCANCODE_EQUALS]) camera->zoom += 0.01f;
        if (keys[SDL_SCANCODE_MINUS]) camera->zoom -= 0.01f;
        if (camera->zoom < 0.5f) camera->zoom = 0.5f;
        if (camera->zoom > 4.0f) camera->zoom = 4.0f;

        

        tileMap->renderLayer(renderer, camera, "Tile Layer 1");
        tileMap->renderLayer(renderer, camera, "Tile Layer 2");
        tileMap->renderLayer(renderer, camera, "Tile Layer 3");
        tileMap->renderLayer(renderer, camera, "Tile Layer 4");
        tileMap->renderLayer(renderer, camera, "Tile Layer 5");
        tileMap->renderLayer(renderer, camera, "Tile Layer 6");

        

        Uint32 now = SDL_GetTicks();

        for (auto enemy : enemies) {
            if (!enemy->isDead()) {
                enemy->update(player->gedDest());

                SDL_FRect enemyRect = enemy->getRect();
                SDL_FRect playerRect = player->gedDest();
                Uint32 now = SDL_GetTicks();

                if (SDL_HasRectIntersectionFloat(&enemyRect, &playerRect)) {
                    if (enemy->isAttacking() && enemy->canAttack(now)) {
                        int damage = 8 + rand() % 3;  // 8–10
                        player->takeDamage(damage);
                    }

                    if (player->isAttacking() && player->readyToDealDamage()) {
                        int damage = 12 + rand() % 4; // 12–15
                        enemy->takeDamage(damage);
                    }
                }
            }
            enemy->render(renderer, camera);
        }

        // Получаем состояние клавиш
        
        static bool ePreviouslyPressed = false;
        bool ePressed = keys[SDL_SCANCODE_E];

        if (ePressed && !ePreviouslyPressed) {
            for (auto& npc : npcs) {
                if (npc->isNearPlayer(player->gedDest())) {
                    npc->showDialog = !npc->showDialog;  // переключаем диалог
                    break;  // только один NPC
                }
            }
        }
        ePreviouslyPressed = ePressed;

        float deltaTime = 1.0f / 60.0f;  // Можно позже заменить на реальное время между кадрами

        for (auto npc : npcs) {
            if (npc->showDialog && !npc->isNearPlayer(player->gedDest())) {
                npc->showDialog = false; // Закрываем диалог, если игрок отошёл
            }

            npc->update(deltaTime);
            npc->render(renderer, camera);

            if (npc->showDialog) {
                SDL_FRect dialogBox = { npc->getRect().x - 20, npc->getRect().y - 70, 180, 40 };
                SDL_FRect screenBox = camera->apply(dialogBox);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                SDL_RenderFillRect(renderer, &screenBox);

                SDL_Color color = { 255, 255, 255, 255 };
                std::string text = "Hello, hero!";

                // SDL3: используем length, как требует сигнатура
                SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), color);


                int w = surface->w;
                int h = surface->h;

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_DestroySurface(surface); // Surface больше не нужен

                SDL_FRect textRect = { screenBox.x + 10, screenBox.y + 10, (float)w, (float)h };
                SDL_RenderTexture(renderer, texture, nullptr, &textRect);
                SDL_DestroyTexture(texture);

            }

        }

        player->otrisovka();
        player->obnovleniepersa();
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    return quit ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

void Game::SDL_AppQuit(SDL_AppResult result) {
    delete menu;
    delete camera;

    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
