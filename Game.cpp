#include "Game.h"
#include "Player.h"
#include <vector>
#include <string>
#include "Enemy.h"
#include <cstdlib>
#include <ctime>

std::vector<Enemy*> enemies;

Game::Game() {}

Game::~Game() {}

SDL_AppResult Game::SDL_AppInit() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_CreateWindowAndRenderer("SDL3 Game", 1920, 1080, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    camera = new Camera(1920, 1080, 400, 200);
    tileMap = new TileMap(renderer);
    tileMap->loadFromFile("assets/map/MEGATEST.json");
    tileMap->renderLayer(renderer, camera->getView(), "Tile Layer 1");
    tileMap->renderLayer(renderer, camera->getView(), "Tile Layer 2");

    enemies.push_back(new Enemy(renderer, 600, 800));
    enemies.push_back(new Enemy(renderer, 1200, 800));

    font = TTF_OpenFont("assets/fonts/Orbitron-VariableFont_wght.ttf", 32);
    player = new Player(renderer, font, camera);

    player->setCollisions(tileMap->getCollisionRects());

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
        camera->update(player->gedDest());
        tileMap->renderLayer(renderer, camera->getView(), u8"Tile Layer 1");
        tileMap->renderLayer(renderer, camera->getView(), u8"Tile Layer 2");

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
