#include "tile_map.h"
#include "defines.h"
#include "raylib.h"

TileMap ::TileMap(const char *path) { tile_map = LoadTexture(path); }
TileMap ::~TileMap() { UnloadTexture(tile_map); }

void TileMap::DrawTile(TileID tileID) {
  Rectangle tile_rect = {(float)Config::TILE_SIZE * 0,
                         (float)Config::TILE_SIZE * tileID, Config::TILE_SIZE,
                         Config::TILE_SIZE};

  Vector2 position = {(int)(Config::SCREEN_WIDTH / 2),
                      (int)(Config::SCREEN_HEIGHT / 2)};

  DrawTextureRec(tile_map, tile_rect, position, WHITE);
}
