#include "tile_map.h"
#include "defines.h"
#include "raylib.h"
#include <cmath>

HexTileManager ::HexTileManager() {
  pHighlightedTile = nullptr;
  TileMapAsset = {0};
}
HexTileManager ::~HexTileManager() { UnloadTexture(TileMapAsset); }

void HexTileManager::LoadTileMapAsset(const char *path) {
  TileMapAsset = LoadTexture(path);
}

void HexTileManager::DrawTile(TileID tileID, int q, int r) {
  Rectangle tile_rect = {(float)Config::TILE_SIZE * 0,
                         (float)Config::TILE_SIZE * tileID, Config::TILE_SIZE,
                         Config::TILE_SIZE};
  if (tileID == VOID) {
    return;
  }

  float x_offset;
  float y_offset = r * Config::TILE_SIZE_HALF;

  // Shift tiles every second row by a half tile
  if (r & 1) {
    x_offset = q * Config::TILE_SIZE - Config::TILE_SIZE_HALF;
  } else {
    x_offset = q * Config::TILE_SIZE;
  }

  Vector2 position = {((float)Config::SCREEN_WIDTH / 2 + x_offset),
                      ((float)Config::SCREEN_HEIGHT / 2 + y_offset)};

  Rectangle dest_rect = {position.x, position.y, Config::TILE_SIZE,
                         Config::TILE_SIZE};
  Vector2 origin = {0.0f, 0.0f};

  // Tile is highlighted, if the mouse hover tile
  if (pHighlightedTile->q == q && pHighlightedTile->r == r) {
    DrawTexturePro(TileMapAsset, tile_rect, dest_rect, origin, 0.0f, RED);
  } else {
    DrawTexturePro(TileMapAsset, tile_rect, dest_rect, origin, 0.0f, WHITE);
  }
}

HexCoords HexTileManager::MouseToHexCoords() {
  Vector2 mouse_position = GetMousePosition();

  // Calculate mouse position relative to the center of the screen
  Vector2 scaled_mouse_pos = {
      (mouse_position.x - (Config::SCREEN_WIDTH / 2.0f)) / Config::ZOOM,
      (mouse_position.y - (Config::SCREEN_HEIGHT / 2.0f)) / Config::ZOOM};

  float x = scaled_mouse_pos.x;
  float y = scaled_mouse_pos.y;

  // Use floating point constants
  float tile_w = (float)Config::TILE_SIZE;           // 32
  float tile_h_step = (float)Config::TILE_SIZE_HALF; // 16

  // 1. Convert to continuous Axial coords
  // Formula for your specific aspect ratio (2:1 iso):
  float r = y / tile_h_step;
  float q = (x - y) / tile_w;

  // 2. Cube Rounding Algorithm
  float cube_x = q;
  float cube_z = r;
  float cube_y = -cube_x - cube_z;

  int rx = round(cube_x);
  int ry = round(cube_y);
  int rz = round(cube_z);

  float x_diff = std::abs(rx - cube_x);
  float y_diff = std::abs(ry - cube_y);
  float z_diff = std::abs(rz - cube_z);

  if (x_diff > y_diff && x_diff > z_diff) {
    rx = -ry - rz;
  } else if (y_diff > z_diff) {
    ry = -rx - rz;
  } else {
    rz = -rx - ry;
  }

  // 3. Convert Axial (rx, rz) to your Offset (col, row)
  // Your offset logic shifts odd rows by +0.5 width.
  // Formula: col = axial_q + floor(axial_r / 2)
  int row = rz;
  int col = rx + (int)floor(row / 2.0f);

  return {0, 0};
}

void HexTileManager::SetHighlightedTilePointer(HexCoords *HighlightedTile) {
  pHighlightedTile = HighlightedTile;
}
