#ifndef GAME_H
#define GAME_H

#include "hex_tile_grid.h"
#include "raylib.h"

class Game {
private:
  Camera2D camera;
  HexGrid hexGrid;
  void DrawDebugOverlay(bool is_enabled);
  Vector2 MousePos;

public:
  Game();
  ~Game();
  void GameLoop();
};

#endif // !GAME_H
