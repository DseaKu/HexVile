#include "tile_map.h"
#include "defines.h"
#include "raylib.h"

Tiles ::Tiles(const char *path) { tile_map = LoadTexture(path); }
Tiles ::~Tiles() { UnloadTexture(tile_map); }

void Tiles::DrawTile(TileID tileID, int q, int r) {
  Rectangle tile_rect = {(float)Config::TILE_SIZE * 0,
                         (float)Config::TILE_SIZE * tileID, Config::TILE_SIZE,
                         Config::TILE_SIZE};

  float x_offset;
  float y_offset;
  if (r & 1) {
    x_offset = q * Config::TILE_SIZE - Config::TILE_SIZE_HALF;
    y_offset = r * Config::TILE_SIZE - Config::TILE_SIZE_HALF;
  } else {
    x_offset = q * Config::TILE_SIZE;
    y_offset = r * Config::TILE_SIZE;
  }
  Vector2 position = {((float)Config::SCREEN_WIDTH / 2 + x_offset),
                      ((float)Config::SCREEN_HEIGHT / 2 + y_offset)};

  DrawTextureRec(tile_map, tile_rect, position, WHITE);
}
