#pragma once
#include <SDL3/SDL.h>
#include <json.hpp>
#include <string>
#include <vector>

struct MapLayer {
    std::string name;
    std::vector<int> data;
};


struct Tileset {
    int firstgid;
    int columns;
    int tilecount;
    int tileWidth;
    int tileHeight;
    SDL_Texture* texture;
};

class TileMap {
public:
    TileMap(SDL_Renderer* renderer);
    ~TileMap();

    void renderLayer(SDL_Renderer* renderer, const SDL_FRect& camera, const std::string& layerName);

    bool loadFromFile(const std::string& path);
    void render(SDL_Renderer* renderer, const SDL_FRect& camera);
    const std::vector<SDL_FRect>& getCollisionRects() const;
    SDL_FPoint getSpawnPoint() const;

private:
    std::vector<MapLayer> layers;
    SDL_FPoint spawnPoint{};
    SDL_Renderer* renderer;
    int tileWidth = 0;
    int tileHeight = 0;
    int mapWidth = 0;
    int mapHeight = 0;

   // тайловые слои
    std::vector<Tileset> tilesets;
    std::vector<SDL_FRect> collisionRects; // коллизии

    void loadTilesets(const std::string& mapFolder, const nlohmann::json& jsonTilesets);
    void loadCollisions(const nlohmann::json& layersJson);
};
