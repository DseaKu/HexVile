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
  HexTiles tile_map;
  Camera2D camera;
  Debugger debugger;
  HexCoords selected_tile;
};
