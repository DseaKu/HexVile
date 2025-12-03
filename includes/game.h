#pragma once

#include "debug.h"
#include "raylib.h"
#include "tile_map.h"

class Game {
public:
  Game();
  ~Game();
  void GameLoop();

private:
  HexTiles tiles;
  Camera2D camera;
  Debugger debugger;
};
