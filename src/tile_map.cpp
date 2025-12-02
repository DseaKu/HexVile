#include "tile_map.h"
#include "defines.h"
#include "raylib.h"

TileMap ::TileMap(const char *path) { tile_map = LoadTexture(path); }
TileMap ::~TileMap() { UnloadTexture(tile_map); }

void TileMap::DrawTile(TileID) {
  DrawTexture(tile_map, Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2,
              WHITE);
}
