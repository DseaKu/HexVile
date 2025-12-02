#include "tile_map.h"
#include "defines.h"
#include "raylib.h"

Tiles ::Tiles(const char *path) { tile_map = LoadTexture(path); }
Tiles ::~Tiles() { UnloadTexture(tile_map); }

void Tiles::DrawTile(TileID tileID, int q, int r) {
  Rectangle tile_rect = {(float)Config::TILE_SIZE * 0,
                         (float)Config::TILE_SIZE * tileID, Config::TILE_SIZE,
                         Config::TILE_SIZE};

  Vector2 position = {(int)(Config::SCREEN_WIDTH / 2),
                      (int)(Config::SCREEN_HEIGHT / 2)};

  DrawTextureRec(tile_map, tile_rect, position, WHITE);
}
