#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "enums.h"
#include "raylib.h"

struct HexCoords {
  int q;
  int r;
};

class HexTiles {
private:
  Texture2D tile_map;

public:
  HexTiles();
  void SetTileMap(const char *path);
  void DrawTile(TileID tileID, int q, int r);
  HexCoords MouseToHexCoords();

  ~HexTiles();
};
#endif // !TILE_MAP_H
