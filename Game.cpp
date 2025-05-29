#include "Game.h"
Game::Game()
{
}

Game::~Game()
{
}
SDL_AppResult Game::SDL_AppInit()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Ошибка инициализации SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!SDL_CreateWindowAndRenderer("SDL3 Game", 1000, 800, 0, &window, &renderer)) {
		SDL_Log("Не удалось создать окно и рендерер: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	player = new Player(renderer, "C:/Users/wein/Downloads/Warrior_Yellow.png");
	gold = new Resources(renderer, { 300, 280 }, "C:/Users/wein/Downloads/G_Idle.png");
	return SDL_AppResult();
}

SDL_AppResult Game::SDL_AppEvent(SDL_Event* event)
{
	switch (event->type) {
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
		break;
	default:
		break;
	}
	player->obrabotkaklavish(event);
	return SDL_APP_CONTINUE;
}

SDL_AppResult Game::SDL_AppIterate()
{
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);
	gold->obnovlenieResources();
	gold->otrisovka();

	player->otrisovka();
	player->obnovleniepersa();
	SDL_RenderPresent(renderer);
	SDL_Delay(16);
	return SDL_APP_CONTINUE;
}

void Game::SDL_AppQuit(SDL_AppResult result)
{
}
