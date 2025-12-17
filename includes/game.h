#ifndef GAME_H
#define GAME_H

#include "GFX_manager.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "io_handler.h"
#include "item_handler.h"
#include "player.h"
#include "raylib.h"
#include "ui_handler.h"
#include <vector>

struct DebugData {
  std::string section;
  std::vector<std::string> subSection;
};
class Game {
private:
  float timer;
  float updateGridTreshold;
  unsigned char *hackFontRegular;
  Camera2D camera;
  Vector2 relativeCenter;
  Vector2 cameraTopLeft;
  Rectangle cameraRect;
  std::vector<DebugData> debugData;

  // --- Objects
  HexGrid hexGrid;
  Player player;
  GFX_Manager GFX_Manager;
  FontHandler fontHandler;
  UI_Handler uiHandler;
  ItemHandler itemHandler;
  IO_Handler ioHandler;

  // --- Methods ---
  void UpdateInputs();
  void DrawDebugOverlay(bool is_enabled);

public:
  Game();
  ~Game();
  void GameLoop();
  void Unload();
  const char *MouseMaskToString(MouseMask m);
};

#endif // !GAME_H
