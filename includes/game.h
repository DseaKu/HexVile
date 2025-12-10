#ifndef GAME_H
#define GAME_H

#include "hex_tile_grid.h"
#include "player.h"
#include "raylib.h"
#include "texture_handler.h"
#include <vector>

struct DebugData {
  std::string section;
  std::vector<std::string> subSection;
};
class Game {
private:
  Camera2D camera;
  HexGrid hexGrid;
  void DrawDebugOverlay(bool is_enabled);
  Vector2 MousePos;
  Vector2 relativeCenter;
  Player player;
  TextureHandler textureHandler;
  std::vector<DebugData> debugData;

public:
  Game();
  ~Game();
  void GameLoop();
};

#endif // !GAME_H
