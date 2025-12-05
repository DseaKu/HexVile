#include "tile_map.h"
#include "defines.h"
#include "raylib.h"

HexTileManager ::HexTileManager() {
  pHighlightedTile = nullptr;
  TileMapAsset = {0};
}
HexTileManager ::~HexTileManager() {}

void HexTileManager::LoadTileMapAsset(const char *path) {
  TileMapAsset = LoadTexture(path);
}
void HexTileManager::UnloadTileMapAsset() { UnloadTexture(TileMapAsset); }

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

  Vector2 position = {Config::SCREEN_CENTER_FOR_HEX_TILES.x + x_offset,
                      Config::SCREEN_CENTER_FOR_HEX_TILES.y + y_offset};

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

// This function should determine in which hextile a 2D point is
HexCoords HexTileManager::PosToHexCoords(Vector2 pos) {
  int q;
  int r;
  float tile_size = Config::TILE_SIZE;

  // return {q, r};
  return {0, 0};
}

void HexTileManager::SetHighlightedTilePointer(HexCoords *HighlightedTile) {
  pHighlightedTile = HighlightedTile;
}
