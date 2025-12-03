#ifndef GAME_H
#define GAME_H

#include "debug.h"
#include "raylib.h"
#include "tile_map.h"
class Game {
private:
  Vector2 mouse_pos;
  HexCoords selcted_tile;
  Debugger debugger;
  Camera2D camera;
  HexTiles tiles;

public:
  Game();
  void GameLoop();
  ~Game();
};

#endif // !GAME_H
