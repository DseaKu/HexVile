#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "enums.h"
#include "raylib.h"

class Tiles {
private:
  Texture2D tile_map;

public:
  Tiles(const char *path);
  void DrawTile(TileID tileID, int q, int r);

  ~Tiles();
};
#endif // !TILE_MAP_H
