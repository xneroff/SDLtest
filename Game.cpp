    #include "Game.h"
    #include "Player.h"
    #include <vector>
    #include <string>

    Game::Game()
    {

    }

    Game::~Game()
    {
    }
    SDL_AppResult Game::SDL_AppInit()
    {
	    SDL_Init(SDL_INIT_VIDEO);
	    TTF_Init();
        SDL_CreateWindowAndRenderer("SDL3 Game", 1920, 1080, SDL_WINDOW_RESIZABLE, &window, &renderer);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        tileMap = new TileMap(renderer);
        tileMap->loadFromFile("assets/map/MEGATEST.json");  // путь к JSON
        
        font = TTF_OpenFont("assets/fonts/Orbitron-VariableFont_wght.ttf", 32);
        camera = new Camera(1920, 1080, 400, 200);  // СНАЧАЛА камера
        player = new Player(renderer, font, camera);
        auto collisions = tileMap->getCollisionRects();
        player->setCollisions(collisions);

        // ❗ Установим позицию из точки спавна
        SDL_FPoint spawn = tileMap->getSpawnPoint();
        player->setPosition(spawn.x, spawn.y);
        menu = new MainMenu(renderer,font, window); 
    
	
        return SDL_AppResult();
    }

    SDL_AppResult Game::SDL_AppEvent(SDL_Event* event)
    {
        if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

        if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
        {
            if (showMenu) {
                if (!menu->isInSettings())  // 👈 если НЕ в подменю настроек
                    showMenu = false;
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

    SDL_AppResult Game::SDL_AppIterate()
    {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        if (showMenu) {
        
            menu->render();
        }
        else {
            camera->update(player->gedDest());
            tileMap->renderLayer(renderer, camera->getView(), "Background");
            tileMap->renderLayer(renderer, camera->getView(), "Blocks"); // слой с плитками
            player->otrisovka();  // персонаж всегда над тайлами
            tileMap->renderLayer(renderer, camera->getView(), "Foreground"); // над игроком (например деревья)
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

