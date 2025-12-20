#ifndef GAME_H
#define GAME_H

#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "player.h"
#include "raylib.h"
#include "ui_handler.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

struct DebugData {
  std::string section;
  std::vector<std::string> subSection;
};

struct WorldState {
  HexGrid hexGrid;
  Player player;
  ItemHandler itemHandler;
  Camera2D camera;
  Vector2 relativeCenter;
  Vector2 cameraTopLeft;
  Rectangle cameraRect;
  float timer;
  float updateGridTreshold;
};

struct FrameContext {
  Inputs inputs;
  Vector2 mouseScreenPos;
  Vector2 mouseWorldPos;
  MouseMask mouseMask;

  // Screen
  int screenWidth;
  int screenHeight;

  float deltaTime;
};

struct RenderState {
  Camera2D camera;

  // Resources / Stats
  int tilesTotal;
  int tilesUsed;
  int tilesVisible;
  int mapRadius;
  double visCalcTime;

  // Mouse Hover
  HexCoord mouseTileCoord;
  TileID mouseTileType;

  // Player
  Vector2 playerPos;
  HexCoord playerTileCoord;
  TileID playerTileType;
  std::string playerStateStr;
  std::string playerDirStr;
  int playerFrame;
  float playerSpeed;

  // ToolBar
  std::string selectedItemType;
  int selectedToolBarSlot;
};

class Game {
private:
  unsigned char *hackFontRegular;

  // Logic/State
  WorldState worldState;
  FrameContext frameContext;
  RenderState renderStates[2];
  int renderStateIndex = 0; // Index of the front buffer (used for rendering)

  // Rendering/System
  GFX_Manager gfxManager;
  FontHandler fontHandler;
  UI_Handler uiHandler;

  std::vector<DebugData> debugData;

  // Threading
  std::thread logicThread;
  std::mutex logicMutex;
  std::condition_variable mainToLogicCV;
  std::condition_variable logicToMainCV;
  std::atomic<bool> isRunning;
  bool isFullscreenMode = false;
  bool logicUpdateReady; // Input ready for logic
  bool logicUpdateDone;  // Logic finished for this frame

  // Profiling
  std::atomic<double> logicExecutionTime;
  std::atomic<double> renderExecutionTime;

  float debugUpdateTimer;
  double displayRenderTime;
  double displayLogicTime;
  double displayVisTime;
  double displayRamUsage;

  // Methods
  void UpdateInputs(); // Gathers inputs from Raylib
  void LogicLoop();    // The function running in the separate thread
  void RunLogic();     // Calls the update functions on worldState

  void DrawDebugOverlay(bool is_enabled);

public:
  Game();
  ~Game();
  void GameLoop();
  void Unload();
  const char *MouseMaskToString(MouseMask m);
};

#endif // !GAME_H
