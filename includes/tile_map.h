#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "enums.h"
#include "raylib.h"

struct HexCoords {
  int q;
  int r;
};

class HexTileManager {
private:
  Texture2D TileMapAsset;
  // HexCoords *pHighlightedTile;

public:
  HexTileManager();
  void LoadTileMapAsset(const char *path);
  void DrawTile(TileID tileID, int q, int r);
  // void SetHighlightedTilePointer(HexCoords *HighlightedTile);
  HexCoords MouseToHexCoords();

  ~HexTileManager();
};
#endif // !TILE_MAP_H
