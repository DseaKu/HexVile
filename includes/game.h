#ifndef GAME_H
#define GAME_H

#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "item.h"
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
  int selectedItemSlot;
  Rectangle cameraRect;
  std::vector<DebugData> debugData;

  // --- Objects
  HexGrid hexGrid;
  Player player;
  TextureHandler textureHandler;
  FontHandler fontHandler;
  UI_Handler uiHandler;
  ItemHandler itemHandler;

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
