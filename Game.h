
#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Player.h"
#include "MainMenu.h"
#include "Camera.h"
#include "TileMap.h"


class Game
{
public:
	Game();
	~Game();	
	SDL_AppResult SDL_AppInit();
	SDL_AppResult SDL_AppEvent(SDL_Event* event);
	SDL_AppResult SDL_AppIterate();
	void SDL_AppQuit(SDL_AppResult result);

private:
	bool showMenu = false;
	bool quit = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Player* player = nullptr;
	TTF_Font* font = nullptr;
	MainMenu* menu = nullptr;
	TileMap* tileMap = nullptr;
	Camera* camera;
};

