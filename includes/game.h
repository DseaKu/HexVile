#ifndef GAME_H
#define GAME_H

#include "GFX_manager.h"
#include "debugger.h"
#include "defines.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "player.h"
#include "raylib.h"
#include "structs.h"
#include "ui_handler.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

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

class Game {
private:
  unsigned char *hackFontRegular;

  // Logic/State
  WorldState worldState;
  FrameContext frameContext;
  RenderState renderStates[2];
  int renderStateIndex = 0;

  // Rendering/System
  GFX_Manager gfxManager;
  FontHandler fontHandler;
  UI_Handler uiHandler;
  Debugger debugger;

  // Threading
  std::thread logicThread;
  std::mutex logicMutex;
  std::condition_variable mainToLogicCV;
  std::condition_variable logicToMainCV;
  std::atomic<bool> isRunning;
  bool isFullscreenMode = false;
  bool logicUpdateReady;
  bool logicUpdateDone;
  bool isUnloaded = false;

  // Profiling
  std::atomic<double> logicExecutionTime;
  std::atomic<double> renderExecutionTime;

  float debugUpdateTimer;
  double displayRenderTime;
  double displayLogicTime;
  double displayVisTime;
  double displayRamUsage;

  // Main Threat - Render, update frame context and draw debug data;
  void UpdateFrameContext();

  // Logic Threat - Process frame context, update world state
  void RunLogic();
  void LogicLoop();

public:
  Game();
  ~Game();
  void GameLoop();
  void Unload();
};

#endif // !GAME_H
