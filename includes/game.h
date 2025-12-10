#ifndef GAME_H
#define GAME_H

#include "hex_tile_grid.h"
#include "player.h"
#include "raylib.h"
#include "texture_handler.h"

class Game {
private:
  Camera2D camera;
  HexGrid hexGrid;
  void DrawDebugOverlay(bool is_enabled);
  Vector2 MousePos;
  Vector2 relativeCenter;
  Player player;
  TextureHandler textureHandler;

public:
  Game();
  ~Game();
  void GameLoop();
};

#endif // !GAME_H
