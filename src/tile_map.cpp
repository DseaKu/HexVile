#include "tile_map.h"
#include "defines.h"
#include "raylib.h"

Tiles ::Tiles(const char *path) { tile_map = LoadTexture(path); }
Tiles ::~Tiles() { UnloadTexture(tile_map); }

void Tiles::DrawTile(TileID tileID, int col, int row) {
  Rectangle tile_rect = {(float)Config::TILE_SIZE * 0,
                         (float)Config::TILE_SIZE * tileID, Config::TILE_SIZE,
                         Config::TILE_SIZE};
  if (tileID == VOID) {
    return;
  }

  float x_offset;
  float y_offset = row * Config::SCALED_TILE_SIZE_HALF;

  // Numbers of rows is odd
  if (row & 1) {
    x_offset = col * Config::SCALED_TILE_SIZE - Config::SCALED_TILE_SIZE_HALF;
  }
  // Numbers of rows is even
  else {
    x_offset = col * Config::SCALED_TILE_SIZE;
  }
  Vector2 position = {((float)Config::SCREEN_WIDTH / 2 + x_offset),
                      ((float)Config::SCREEN_HEIGHT / 2 + y_offset)};

  Rectangle dest_rect = {position.x, position.y, Config::SCALED_TILE_SIZE,
                         Config::SCALED_TILE_SIZE};
  Vector2 origin = {0.0f, 0.0f};

  DrawTexturePro(tile_map, tile_rect, dest_rect, origin, 0.0f, WHITE);
}
