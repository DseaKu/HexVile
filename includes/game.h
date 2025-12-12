#ifndef GAME_H
#define GAME_H

#include "font_handler.h"
#include "hex_tile_grid.h"
#include "player.h"
#include "raylib.h"
#include "texture_handler.h"
#include "ui_handler.h"
#include <vector>

struct DebugData {
  std::string section;
  std::vector<std::string> subSection;
};
class Game {
private:
  void DrawDebugOverlay(bool is_enabled);
  unsigned char *hackFontRegular;
  Camera2D camera;
  Vector2 MousePos;
  Vector2 relativeCenter;
  Vector2 cameraTopLeft;
  TextureHandler textureHandler;
  std::vector<DebugData> debugData;
  Rectangle cameraRect;
  HexGrid hexGrid;
  Player player;
  FontHandler fontHandler;
  UIHandler uiHandler;

public:
  Game();
  ~Game();
  void GameLoop();
};

#endif // !GAME_H
