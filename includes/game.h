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
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

struct DebugData {
  std::string section;
  std::vector<std::string> subSection;
};

struct InputState {
  // Mouse
  Vector2 mouseScreenPos;
  Vector2 mouseWorldPos;
  bool leftMouseClicked;
  bool rightMouseClicked;
  MouseMask mouseMask;

  // Keyboard
  bool keyOne, keyTwo, keyThree, keyFour, keyFive;
  bool keySix, keySeven, keyEight, keyNine, keyZero;

  PlayerInputState playerInput;
  float frameTime; // Delta time
  
  // Screen
  int screenWidth;
  int screenHeight;
};

struct GameState {
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
  GameState gameState;
  InputState currentInput;

  // Rendering/System
  GFX_Manager gfxManager;
  FontHandler fontHandler;
  UI_Handler uiHandler;
  IO_Handler ioHandler;
  
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

  // Methods
  void UpdateInputs(); // Gathers inputs from Raylib
  void LogicLoop();    // The function running in the separate thread
  void RunLogic();     // Calls the update functions on gameState

  void DrawDebugOverlay(bool is_enabled);

public:
  Game();
  ~Game();
  void GameLoop();
  void Unload();
  const char *MouseMaskToString(MouseMask m);
};

#endif // !GAME_H
