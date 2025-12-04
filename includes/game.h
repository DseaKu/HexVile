#ifndef GAME_H
#define GAME_H

#include "debug.h"
#include "raylib.h"
#include "tile_map.h"

class Game {
public:
  Game();
  ~Game();
  void GameLoop();

private:
  HexTileManager TileMap;
  Camera2D Camera;
  Debugger Debugger;
  HexCoords HighlightedTile;
};

#endif // !GAME_H
