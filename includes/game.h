#ifndef GAME_H
#define GAME_H

#include "hex_tile_grid.h"
#include "raylib.h"
#include "tile_map.h"

class Game {
private:
  HexTileManager TileMap;
  Camera2D camera;
  HexCoords highlightedTile;
  HexGrid hexGrid;
  void DrawDebugOverlay(bool is_enabled);
  Vector2 MousePos;

public:
  Game();
  ~Game();
  void GameLoop();
};

#endif // !GAME_H
