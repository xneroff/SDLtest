#include "TileMap.h"
#include <fstream>
#include <SDL3_image/SDL_image.h>
#include <iostream>
using json = nlohmann::json;

TileMap::TileMap(SDL_Renderer* rend) : renderer(rend) {}
TileMap::~TileMap() { for (auto& ts : tilesets) SDL_DestroyTexture(ts.texture); }

bool TileMap::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    json j;
    file >> j;
    tileWidth = j["tilewidth"];
    tileHeight = j["tileheight"];
    mapWidth = j["width"];
    mapHeight = j["height"];

    for (const auto& layer : j["layers"]) {
        if (layer["type"] == "tilelayer") {
            layers.push_back({ layer["name"], layer["data"].get<std::vector<int>>() });
        }
    }

    loadCollisions(j["layers"]);
    std::string folder = path.substr(0, path.find_last_of("/\\") + 1);
    loadTilesets(folder, j["tilesets"]);
    return true;
}

void TileMap::renderLayer(SDL_Renderer* renderer, const SDL_FRect& camera, const std::string& name) {
    SDL_FRect dest = { 0, 0, (float)tileWidth, (float)tileHeight };
    for (const auto& layer : layers) {
        if (layer.name != name) continue;
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int tileID = layer.data[y * mapWidth + x];
                if (tileID == 0) continue;

                const Tileset* ts = nullptr;
                for (int i = tilesets.size() - 1; i >= 0; --i) {
                    if (tileID >= tilesets[i].firstgid) { ts = &tilesets[i]; break; }
                }
                if (!ts) continue;

                SDL_FRect src = {
                    (tileID - ts->firstgid) % ts->columns * tileWidth,
                    (tileID - ts->firstgid) / ts->columns * tileHeight,
                    (float)tileWidth, (float)tileHeight
                };

                dest.x = x * tileWidth - camera.x;
                dest.y = y * tileHeight - camera.y;
                SDL_RenderTexture(renderer, ts->texture, &src, &dest);
            }
        }
    }
}

void TileMap::loadTilesets(const std::string& folder, const json& tilesetsJson) {
    for (const auto& entry : tilesetsJson) {
        std::ifstream tsx(folder + entry["source"].get<std::string>());
        std::string xml((std::istreambuf_iterator<char>(tsx)), {});
        std::string imgPath = xml.substr(xml.find("image source=\"") + 14);
        imgPath = imgPath.substr(0, imgPath.find("\""));
        Tileset ts = {
            entry["firstgid"],
            std::stoi(xml.substr(xml.find("columns=\"") + 9, xml.find("\"", xml.find("columns=\"")))),
            tileWidth, tileHeight,
            IMG_LoadTexture(renderer, (folder + imgPath).c_str())
        };
        tilesets.push_back(ts);
    }
}

void TileMap::loadCollisions(const nlohmann::json& layersJson) {
    for (const auto& layer : layersJson) {
        if (layer["type"] == "objectgroup" && layer["name"] == "Collisions") {
            for (const auto& obj : layer["objects"]) {
                if (obj.contains("name") && obj["name"] == "Spawn") {
                    spawnPoint.x = obj["x"];
                    spawnPoint.y = obj["y"];
                }
                else {
                    SDL_FRect rect;
                    rect.x = obj["x"];
                    rect.y = obj["y"];
                    rect.w = obj["width"];
                    rect.h = obj["height"];
                    collisionRects.push_back(rect);
                }
            }
        }
    }

    std::cout << "Spawn: (" << spawnPoint.x << ", " << spawnPoint.y << ")\n";
    std::cout << "Загружено " << collisionRects.size() << " прямоугольников коллизий\n";
}



const std::vector<SDL_FRect>& TileMap::getCollisionRects() const { return collisionRects; }

SDL_FPoint TileMap::getSpawnPoint() const {
    return spawnPoint;
}
