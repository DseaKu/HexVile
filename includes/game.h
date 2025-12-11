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
  void DrawDebugOverlay(bool is_enabled);
  Camera2D camera;
  HexGrid hexGrid;
  Vector2 MousePos;
  Vector2 relativeCenter;
  Vector2 cameraTopLeft;
  Player player;
  TextureHandler textureHandler;
  std::vector<DebugData> debugData;
  Rectangle cameraRect;

public:
  Game();
  ~Game();
  void GameLoop();
};

#endif // !GAME_H
