#ifndef GAME_H
#define GAME_H

#include "enums.h"
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
  unsigned char *hackFontRegular;
  Camera2D camera;
  Vector2 mousePos;
  MouseMask mouseMask;
  Vector2 relativeCenter;
  Vector2 cameraTopLeft;
  std::vector<DebugData> debugData;
  ItemsID selectedItem;
  Rectangle cameraRect;

  // --- Objects
  TextureHandler textureHandler;
  HexGrid hexGrid;
  Player player;
  FontHandler fontHandler;
  UI_Handler uiHandler;

  // --- Methods ---
  void ProcessInputs();
  void DrawDebugOverlay(bool is_enabled);

public:
  Game();
  ~Game();
  void GameLoop();
  const char *MouseMaskToString(MouseMask m);
};

#endif // !GAME_H
