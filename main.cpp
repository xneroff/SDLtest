    #define SDL_MAIN_USE_CALLBACKS 1;
    #include <SDL3/SDL.h>
    #include <SDL3/SDL_main.h>
    #include <SDL3_image/SDL_image.h>


    int speed = 10;
    const float gravity = 0.5;  // Гравитация
    const float jumpStrength = 10.0;

    class Player
    {
    public:
        Player(SDL_Renderer* renderer);
        void obrabotkaklavish();
        void obnovleniepersa();
        void otrisovka();
        ~Player();

    private:
        SDL_Renderer* renderer;
        SDL_FRect src;
        SDL_FRect dest;
        SDL_Texture* texture;
        int speed;

    };
    void Player::otrisovka() {
        SDL_RenderTexture(renderer, texture, &src, &dest);
    }
    void Player::obnovleniepersa() {
        SDL_RenderTexture(renderer, texture, &src, &dest);
        const bool* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LSHIFT] && keys[SDL_SCANCODE_A]) dest.x -= 40;
        if (keys[SDL_SCANCODE_LSHIFT] && keys[SDL_SCANCODE_D]) dest.x += 40;
        if (keys[SDL_SCANCODE_W]) dest.y -= speed;
        if (keys[SDL_SCANCODE_S]) dest.y += speed;
        if (keys[SDL_SCANCODE_A]) dest.x -= speed;
        if (keys[SDL_SCANCODE_D]) dest.x += speed;
    }
    void Player::obrabotkaklavish() {
    
    }
    Player::Player(SDL_Renderer* renderer) :renderer(renderer) 
    {
        texture = IMG_LoadTexture(renderer, "player.png");
        if (!texture) {
            SDL_Log("Failed to load player texture: %s");
        }
        dest = { 100,100,100,100 };
        src = { 0,0,32,32 };
        speed = 10;
    }

    Player::~Player()
    {
        SDL_DestroyTexture(texture);
    }

    static SDL_Window* window = nullptr;
    static SDL_Renderer* renderer = nullptr;
    static Player* player = nullptr;


    SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer("SDL3 Game", 800, 640, 0, &window, &renderer);
        player = new Player(renderer);
        return SDL_APP_CONTINUE;
    }

    SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
        switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
            break;
        default:    
            break;
        }
        player->obrabotkaklavish();
        return SDL_APP_CONTINUE;
    }

    SDL_AppResult SDL_AppIterate(void* appstate) {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        player->otrisovka();
        player->obnovleniepersa();
        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        return SDL_APP_CONTINUE;
    }

    void SDL_AppQuit(void* appstate, SDL_AppResult result) {
        delete player;
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
