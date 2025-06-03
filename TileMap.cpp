#include "TileMap.h"
#include <fstream>
#include <SDL3_image/SDL_image.h>
#include <iostream>

using json = nlohmann::json;

TileMap::TileMap(SDL_Renderer* rend) : renderer(rend) {}
TileMap::~TileMap() { for (auto& ts : tilesets) SDL_DestroyTexture(ts.texture); }

bool TileMap::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error loading file: " << path << std::endl;
        return false;
    }

    json j;
    file >> j;

    // Checking main map parameters
    std::cout << "Loading map: " << path << std::endl;
    std::cout << "Tile size: " << j["tilewidth"] << "x" << j["tileheight"] << std::endl;
    std::cout << "Map size: " << j["width"] << "x" << j["height"] << std::endl;

    tileWidth = j["tilewidth"];
    tileHeight = j["tileheight"];
    mapWidth = j["width"];
    mapHeight = j["height"];

    for (const auto& layer : j["layers"]) {
        if (layer["type"] == "tilelayer") {
            std::cout << "Loaded tile layer: " << layer["name"] << std::endl;
            layers.push_back({ layer["name"], layer["data"].get<std::vector<int>>() });
        }
    }

    loadCollisions(j["layers"]);
    std::string folder = path.substr(0, path.find_last_of("/\\") + 1);
    loadTilesets(folder, j["tilesets"]);

    std::cout << "Map loaded successfully!\n";
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
        std::string tsxPath = folder + entry["source"].get<std::string>();
        std::ifstream tsx(tsxPath);

        if (!tsx.is_open()) {
            std::cerr << "Ошибка загрузки tileset: " << tsxPath << std::endl;
            continue;
        }

        std::string xml((std::istreambuf_iterator<char>(tsx)), {});
        std::string imgPath = xml.substr(xml.find("image source=\"") + 14);
        imgPath = imgPath.substr(0, imgPath.find("\""));

        std::string fullImgPath = folder + imgPath;
        std::cout << "Загрузка изображения тайлсета: " << fullImgPath << std::endl;

        Tileset ts = {
            entry["firstgid"],
            std::stoi(xml.substr(xml.find("columns=\"") + 9, xml.find("\"", xml.find("columns=\"")))),
            tileWidth, tileHeight,
            IMG_LoadTexture(renderer, fullImgPath.c_str())
        };
        if (tilesets.empty()) {
            std::cerr << "Error: No tilesets loaded!" << std::endl;
        }
        std::cout << "Attempting to load tileset: " << folder + entry["source"].get<std::string>() << std::endl;

        tilesets.push_back(ts);
    }
}


void TileMap::loadCollisions(const nlohmann::json& layersJson) {
    std::cout << "Loading collisions...\n";
    for (const auto& layer : layersJson) {
        std::cout << "Found layer: " << layer["name"] << " (" << layer["type"] << ")" << std::endl;

        if (layer["type"] == "objectgroup" && layer["name"] == "Collisions") {
            for (const auto& obj : layer["objects"]) {
                if (obj.contains("name") && obj["name"] == "Spawn") {
                    spawnPoint.x = obj["x"];
                    spawnPoint.y = obj["y"];
                    std::cout << "Player spawn point: (" << spawnPoint.x << ", " << spawnPoint.y << ")\n";
                }
                else {
                    SDL_FRect rect;
                    rect.x = obj["x"];
                    rect.y = obj["y"];
                    rect.w = obj["width"];
                    rect.h = obj["height"];
                    collisionRects.push_back(rect);
                    std::cout << "Added collision box: (" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << ")\n";
                }
            }
        }
    }
}




const std::vector<SDL_FRect>& TileMap::getCollisionRects() const { return collisionRects; }

SDL_FPoint TileMap::getSpawnPoint() const {
    return spawnPoint;
}
