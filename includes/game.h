#ifndef GAME_H
#define GAME_H

#include "debug.h"
#include "hex_grid.h"
#include "raylib.h"
#include "tile_map.h"

class Game {
private:
  HexTileManager TileMap;
  Camera2D Camera;
  Debugger Debugger;
  HexCoords HighlightedTile;
  HexGrid hexGrid;

public:
  Game();
  ~Game();
  void GameLoop();
};

#endif // !GAME_H
