#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "enums.h"
#include "raylib.h"

class TileMap {
private:
  Texture2D tile_map;

public:
  TileMap(const char *path);
  void DrawTile(TileID tileID);

  ~TileMap();
};
#endif // !TILE_MAP_H
