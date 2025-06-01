#pragma once
#include <SDL3/SDL.h>
#include <json.hpp>
#include <vector>
#include <string>

struct Tileset {
    int firstgid;
    int columns;
    int tileWidth, tileHeight;
    SDL_Texture* texture;
};

struct MapLayer {
    std::string name;
    std::vector<int> data;
};

class TileMap {
public:
    TileMap(SDL_Renderer* renderer);
    ~TileMap();

    bool loadFromFile(const std::string& path);
    void renderLayer(SDL_Renderer* renderer, const SDL_FRect& camera, const std::string& name);
    SDL_FPoint getSpawnPoint() const;
    const std::vector<SDL_FRect>& getCollisionRects() const;

private:

    SDL_FPoint spawnPoint{ 0, 0 };

    SDL_Renderer* renderer;
    std::vector<Tileset> tilesets;
    std::vector<MapLayer> layers;
    std::vector<SDL_FRect> collisionRects;

    int tileWidth = 0, tileHeight = 0, mapWidth = 0, mapHeight = 0;

    void loadTilesets(const std::string& folder, const nlohmann::json& tilesetsJson);
    void loadCollisions(const nlohmann::json& layersJson);
};