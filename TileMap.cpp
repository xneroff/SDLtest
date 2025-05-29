#include "TileMap.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <SDL3_image/SDL_image.h>
#include "json.hpp"

using json = nlohmann::json;

TileMap::TileMap(SDL_Renderer* rend) : renderer(rend) {}
TileMap::~TileMap() {
    for (auto& t : tilesets) SDL_DestroyTexture(t.texture);
}

bool TileMap::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть карту: " << path << "\n";
        return false;
    }

    nlohmann::json j;
    file >> j;

    tileWidth = j["tilewidth"];
    tileHeight = j["tileheight"];
    mapWidth = j["width"];
    mapHeight = j["height"];

    for (const auto& layer : j["layers"]) {
        if (layer["type"] == "tilelayer" && layer.contains("data")) {
            MapLayer mapLayer;
            mapLayer.name = layer["name"].get<std::string>();
            mapLayer.data = layer["data"].get<std::vector<int>>();
            layers.push_back(mapLayer);
        }
    }

    

    loadCollisions(j["layers"]);

    std::string mapFolder = path.substr(0, path.find_last_of("/\\") + 1);
    loadTilesets(mapFolder, j["tilesets"]);
    return true;
}


void TileMap::renderLayer(SDL_Renderer* renderer, const SDL_FRect& camera, const std::string& layerName) {
    SDL_FRect dest{ 0, 0, (float)tileWidth, (float)tileHeight };

    for (const auto& layer : layers) {
        if (layer.name != layerName) continue;

        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int tileID = layer.data[y * mapWidth + x];
                if (tileID == 0) continue;

                // Найти нужный tileset
                const Tileset* ts = nullptr;
                for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; --i) {
                    if (tileID >= tilesets[i].firstgid) {
                        ts = &tilesets[i];
                        break;
                    }
                }

                if (!ts || !ts->texture) continue;

                int localID = tileID - ts->firstgid;
                SDL_FRect src = {
                    (localID % ts->columns) * tileWidth,
                    (localID / ts->columns) * tileHeight,
                    (float)tileWidth, (float)tileHeight
                };

                dest.x = x * tileWidth - camera.x;
                dest.y = y * tileHeight - camera.y;

                SDL_RenderTexture(renderer, ts->texture, &src, &dest);
            }
        }
    }
}



void TileMap::loadTilesets(const std::string& mapFolder, const nlohmann::json& jsonTilesets) {
    using json = nlohmann::json;
    for (const auto& entry : jsonTilesets) {
        std::string tsxPath = mapFolder + entry["source"].get<std::string>();

        std::ifstream tsxFile(tsxPath);
        if (!tsxFile.is_open()) {
            std::cerr << "Не удалось открыть TSX: " << tsxPath << "\n";
            continue;
        }

        std::string xml((std::istreambuf_iterator<char>(tsxFile)), std::istreambuf_iterator<char>());

        // Простой парсинг пути к картинке
        std::size_t start = xml.find("image source=\"") + 14;
        std::size_t end = xml.find("\"", start);
        std::string imgPath = xml.substr(start, end - start);

        Tileset ts;
        ts.firstgid = entry["firstgid"];
        ts.tileWidth = tileWidth;
        ts.tileHeight = tileHeight;
        ts.tilecount = 0; // необязательно
        std::size_t colPos = xml.find("columns=\"");
        if (colPos != std::string::npos) {
            colPos += 9;
            std::size_t colEnd = xml.find("\"", colPos);
            std::string colStr = xml.substr(colPos, colEnd - colPos);
            ts.columns = std::stoi(colStr);
        }
        else if (entry.contains("columns")) {
            ts.columns = entry["columns"];
        }
        else {
            std::cerr << "Не удалось найти 'columns' для tileset " << tsxPath << "\n";
            ts.columns = 1; // по умолчанию
        }

        std::string fullImagePath = imgPath;
        if (imgPath.find(":") == std::string::npos) {
            fullImagePath = mapFolder + imgPath;
        }

        ts.texture = IMG_LoadTexture(renderer, fullImagePath.c_str());
        /*if (!ts.texture) {
            std::cerr << "Ошибка загрузки: " << fullImagePath << " - " << IMG_GetError() << "\n";
            continue;
        }*/

        tilesets.push_back(ts);
    }
}

void TileMap::render(SDL_Renderer* renderer, const SDL_FRect& camera) {
    SDL_FRect dest{ 0, 0, (float)tileWidth, (float)tileHeight };

    for (const auto& layer : layers) {
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int tileID = layer.data[y * mapWidth + x];
                if (tileID == 0) continue;

                // Поиск tileset
                const Tileset* ts = nullptr;
                for (int i = static_cast<int>(tilesets.size()) - 1; i >= 0; --i) {
                    if (tileID >= tilesets[i].firstgid) {
                        ts = &tilesets[i];
                        break;
                    }
                }

                if (!ts || !ts->texture) continue;

                int localID = tileID - ts->firstgid;
                SDL_FRect src = {
                    (localID % ts->columns) * tileWidth,
                    (localID / ts->columns) * tileHeight,
                    (float)tileWidth, (float)tileHeight
                };

                dest.x = x * tileWidth - camera.x;
                dest.y = y * tileHeight - camera.y;

                SDL_RenderTexture(renderer, ts->texture, &src, &dest);
            }
        }
    }
}

void TileMap::loadCollisions(const nlohmann::json& layersJson) {
    for (const auto& layer : layersJson) {
        if (layer["type"] == "objectgroup" && layer["name"] == u8"Collisions") {
            for (const auto& obj : layer["objects"]) {
                SDL_FRect rect;
                rect.x = obj["x"];
                rect.y = obj["y"];
                rect.w = obj["width"];
                rect.h = obj["height"];
                collisionRects.push_back(rect);

                // 👇 Обнаруживаем точку спавна
                if (obj.contains("name") && obj["name"] == "Spawn") {
                    spawnPoint.x = obj["x"];
                    spawnPoint.y = obj["y"];
                }
            }
        }
    }
    std::cout << "Загружено " << collisionRects.size() << " прямоугольников коллизий\n";
}

    
const std::vector<SDL_FRect>& TileMap::getCollisionRects() const {
    return collisionRects;  
}

SDL_FPoint TileMap::getSpawnPoint() const {
    return spawnPoint;
}
