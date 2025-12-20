#include "game.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include <string>

#ifdef __APPLE__
#include <mach/mach.h>
#endif

double GetRamUsageMB() {
#ifdef __APPLE__
  struct mach_task_basic_info info;
  mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info,
                &infoCount) == KERN_SUCCESS) {
    return (double)info.resident_size / (1024 * 1024);
  }
#endif
  return 0.0;
}

// --- Initialization ---
Game::Game()
    : isRunning(true), isFullscreenMode(false), logicUpdateReady(false),
      logicUpdateDone(true), logicExecutionTime(0.0), renderExecutionTime(0.0),
      debugUpdateTimer(0.0f), displayRenderTime(0.0), displayLogicTime(0.0),
      displayVisTime(0.0), displayRamUsage(0.0) {
  gfxManager.LoadAssets(Conf::TEXTURE_ATLAS_PATH);

  gameState.timer = 0.0f;
  gameState.updateGridTreshold = Conf::GRID_UPDATE_PLAYER_MOVE_THRESHOLD;
  int fileSize = 0;
  hackFontRegular = LoadFileData(Conf::FONT_HACK_REGULAR_PATH, &fileSize);

  gameState.hexGrid.InitGrid(Conf::MAP_RADIUS);
  gameState.hexGrid.SetGFX_Manager(&gfxManager);
  gameState.hexGrid.SetCamRectPointer(&gameState.cameraRect);

  gameState.player.SetHexGrid(&gameState.hexGrid);
  gameState.player.SetGFX_Manager(&gfxManager);

  gameState.camera.target = Conf::SCREEN_CENTER;
  gameState.camera.offset = Conf::SCREEN_CENTER;
  gameState.camera.zoom = Conf::INITIAL_CAMERA_ZOOM;
  gameState.camera.rotation = 0.0f;
  gameState.cameraRect = {0, 0, 0, 0};
  gameState.cameraTopLeft = {0, 0};

  fontHandler.LoadFonts();

  uiHandler.SetGFX_Manager(&gfxManager);
  uiHandler.SetItemHandler(&gameState.itemHandler);
  uiHandler.SetFontHandler(&fontHandler);
  uiHandler.SetHexGrid(&gameState.hexGrid);
  uiHandler.SetToolBarActive(true);

  // Start the logic thread
  logicThread = std::thread(&Game::LogicLoop, this);

  // Set mouse to center by default
  SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
}

// --- Main Loop (Rendering Thread) ---
void Game::GameLoop() {
  while (!WindowShouldClose()) {
    // 1. Gather Input (Main Thread)
    UpdateInputs();
    // inputHandler.SetInputState(&gameContext);

    // 2. Sync: Send Input to Logic
    {
      std::unique_lock<std::mutex> lock(logicMutex);
      logicUpdateReady = true;
      logicUpdateDone = false;
    }
    mainToLogicCV.notify_one();

    // 3. Render (Main Thread) - Uses current GameState
    {
      auto startRender = std::chrono::high_resolution_clock::now();
      BeginDrawing();
      ClearBackground(WHITE);

      BeginMode2D(gameState.camera);
      gfxManager.RenderLayer(DRAW_MASK_GROUND_0);
      gfxManager.RenderLayer(DRAW_MASK_GROUND_1);
      gfxManager.RenderLayer(DRAW_MASK_SHADOW);
      gfxManager.RenderLayer(DRAW_MASK_ON_GROUND);
      EndMode2D();

      gfxManager.RenderLayer(DRAW_MASK_UI_0);
      gfxManager.RenderLayer(DRAW_MASK_UI_1);

      DrawDebugOverlay(Conf::IS_DEBUG_OVERLAY_ENABLED);

      EndDrawing();
      auto endRender = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsedRender =
          endRender - startRender;
      renderExecutionTime = elapsedRender.count();
    }

    // 4. Sync: Wait for Logic to finish (Barrier)
    // We wait here to ensure we don't start the next frame's input gathering
    // until the logic is done with the current frame's input.
    // Also, this ensures that the GameState is consistent before we loop again.
    {
      std::unique_lock<std::mutex> lock(logicMutex);
      logicToMainCV.wait(lock, [this] { return logicUpdateDone; });
    }

    gfxManager.SwapBuffers();
  }

  // Signal logic thread to stop
  isRunning = false;
  mainToLogicCV.notify_one();
}

// --- Logic Loop (Worker Thread) ---
void Game::LogicLoop() {
  while (isRunning) {
    std::unique_lock<std::mutex> lock(logicMutex);
    mainToLogicCV.wait(lock, [this] { return logicUpdateReady || !isRunning; });

    if (!isRunning)
      break;

    // Copy input to local variable to minimize locking time if we wanted to
    // but here we just process it.
    auto startLogic = std::chrono::high_resolution_clock::now();
    RunLogic();
    auto endLogic = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsedLogic =
        endLogic - startLogic;
    logicExecutionTime = elapsedLogic.count();

    logicUpdateReady = false;
    logicUpdateDone = true;
    lock.unlock();
    logicToMainCV.notify_one();
  }
}

void Game::UpdateInputs() {
  if (IsKeyPressed(KEY_F)) {
    isFullscreenMode = !isFullscreenMode;
    ToggleBorderlessWindowed();
  }

  gameContext.deltaTime = GetFrameTime();

  gameContext.screenWidth = GetScreenWidth();
  gameContext.screenHeight = GetScreenHeight();

  // Update Camera Offset to match new screen size
  gameState.camera.offset = Vector2{(float)gameContext.screenWidth / 2.0f,
                                    (float)gameContext.screenHeight / 2.0f};

  gameContext.mouseScreenPos = GetMousePosition();

  gameContext.mouseWorldPos =
      GetScreenToWorld2D(gameContext.mouseScreenPos, gameState.camera);

  gameContext.inputs.mousePress.left = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  gameContext.inputs.mousePress.right =
      IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

  gameContext.inputs.keyPress.One = IsKeyPressed(KEY_ONE);
  gameContext.inputs.keyPress.Two = IsKeyPressed(KEY_TWO);
  gameContext.inputs.keyPress.Three = IsKeyPressed(KEY_THREE);
  gameContext.inputs.keyPress.Four = IsKeyPressed(KEY_FOUR);
  gameContext.inputs.keyPress.Five = IsKeyPressed(KEY_FIVE);
  gameContext.inputs.keyPress.Six = IsKeyPressed(KEY_SIX);
  gameContext.inputs.keyPress.Seven = IsKeyPressed(KEY_SEVEN);
  gameContext.inputs.keyPress.Eight = IsKeyPressed(KEY_EIGHT);
  gameContext.inputs.keyPress.Nine = IsKeyPressed(KEY_NINE);
  gameContext.inputs.keyPress.Zero = IsKeyPressed(KEY_ZERO);

  gameContext.inputs.keyPress.Left = IsKeyDown(KEY_A);
  gameContext.inputs.keyPress.Right = IsKeyDown(KEY_D);
  gameContext.inputs.keyPress.Up = IsKeyDown(KEY_W);
  gameContext.inputs.keyPress.Down = IsKeyDown(KEY_S);

  gameState.cameraTopLeft = GetScreenToWorld2D(Vector2{0, 0}, gameState.camera);

  float camWidth = (float)gameContext.screenWidth / gameState.camera.zoom;
  float camHeight = (float)gameContext.screenHeight / gameState.camera.zoom;

  gameState.cameraRect = {gameState.cameraTopLeft.x, gameState.cameraTopLeft.y,
                          camWidth, camHeight};
}

void Game::RunLogic() {
  // Use gameContext and gameState
  gameState.timer += gameContext.deltaTime;

  // Update UI Layout
  uiHandler.UpdateScreenSize(gameContext.screenWidth, gameContext.screenHeight);

  // Update Grid
  gameState.hexGrid.Update(gameState.camera, gameContext.deltaTime);
  uiHandler.Update(gameContext.mouseWorldPos);

  int toolBarSel = gameState.itemHandler.GetSelectionToolBar();

  if (gameContext.inputs.mousePress.left) {
    // Clicked on item bar
    if (uiHandler.GetToolBarAvailability() &&
        CheckCollisionPointRec(
            gameContext.mouseScreenPos, // ToolBar is Screen Space usually?
            uiHandler.GetToolBarRect())) {
      // inputHandler.SetMouseMask(MOUSE_MASK_ITEM_BAR);
      gameContext.mouseMask =
          MOUSE_MASK_ITEM_BAR; // Update local state if needed
      toolBarSel = uiHandler.GetItemSlotAt(gameContext.mouseScreenPos);

    } else {
      // inputHandler.SetMouseMask(MOUSE_MASK_PLAY_GROUND);
      gameContext.mouseMask = MOUSE_MASK_PLAY_GROUND;

      HexCoord clickedHex =
          gameState.hexGrid.PointToHexCoord(gameContext.mouseWorldPos);
      Item *selectedItem =
          gameState.itemHandler.GetToolBarItemPointer(toolBarSel);
      TileID tileToPlace =
          gameState.itemHandler.ConvertItemToTileID(selectedItem->id);

      if (selectedItem->id != ITEM_NULL &&
          gameState.hexGrid.SetTile(clickedHex, tileToPlace)) {
        gameState.itemHandler.TakeItemFromToolBar(selectedItem, 1);
      }
    }
  }

  if (gameContext.inputs.mousePress.right) {
    HexCoord clickedHex =
        gameState.hexGrid.PointToHexCoord(gameContext.mouseWorldPos);
    gameState.hexGrid.SetTile(clickedHex, TILE_NULL);
  }

  toolBarSel =
      uiHandler.GetToolBarSelction(gameContext.inputs.keyPress, toolBarSel);

  uiHandler.SetSelectedItem(toolBarSel);
  gameState.itemHandler.SetItemSelection(toolBarSel);

  // Player Update
  gameState.player.Update(&gameContext.inputs.keyPress, gameContext.deltaTime);

  // Update Camera Target (Logic)
  gameState.camera.target = gameState.player.GetPosition();
}

const char *Game::MouseMaskToString(MouseMask m) {
  switch (m) {
  case MOUSE_MASK_NULL:
    return "Null";
  case MOUSE_MASK_PLAY_GROUND:
    return "Ground";
  case MOUSE_MASK_ITEM_BAR:
    return "Tool Bar";
  default:
    return "Undefined";
  }
}

// --- Debug overlay ---
void Game::DrawDebugOverlay(bool is_enabled) {
  if (!is_enabled)
    return;

  float sectionPosX = Conf::DEBUG_OVERLAY_SECTION_X;
  int sectionPosY = Conf::DEBUG_OVERLAY_SECTION_Y;
  int sectionGapY = Conf::DEBUG_OVERLAY_SECTION_Y_GAP;
  // int sectionFontSize = Conf::DEBUG_OVERLAY_SECTION_FONT_SIZE;
  Color sectionColor = Conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float subSectionPosX = Conf::DEBUG_OVERLAY_SUBSECTION_X_POS;
  int subSectionGapY = Conf::DEBUG_OVERLAY_SUBSECTION_Y_GAP;
  // int subSectionFontSize = Conf::DEBUG_OVERLAY_SUBSECTION_FONT_SIZE;
  Color subSectionColor = Conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float currentY = sectionPosY;

  debugUpdateTimer += GetFrameTime();
  if (debugUpdateTimer >= 1.0f) {
    displayRenderTime = renderExecutionTime.load();
    displayLogicTime = logicExecutionTime.load();
    displayVisTime = gameState.hexGrid.GetVisCalcTime();
    displayRamUsage = GetRamUsageMB();
    debugUpdateTimer = 0.0f;
  }

  debugData.clear();
  debugData.push_back(
      {"Resources",
       {
           TextFormat("FPS: %i", GetFPS()),
           TextFormat("RAM: %.2f MB", displayRamUsage),
           TextFormat("Screen: %ix%i", GetScreenWidth(), GetScreenHeight()),
           TextFormat("Render: %ix%i", GetRenderWidth(), GetRenderHeight()),
           TextFormat("Tiles Total: %i", gameState.hexGrid.GetTilesInTotal()),
           TextFormat("Tiles Used: %i", gameState.hexGrid.GetTilesInUse()),
           TextFormat("Tiles Visible: %i", gameState.hexGrid.GetTilesVisible()),
           TextFormat("Map radius: %i", gameState.hexGrid.GetMapRadius()),
           TextFormat("Render Time: %.2f ms", displayRenderTime),
           TextFormat("Logic Time: %.2f ms", displayLogicTime),
           TextFormat("Culling Time: %.2f ms", displayVisTime),
       }});

  // Use currentInput for debug display
  HexCoord mapTile =
      gameState.hexGrid.PointToHexCoord(gameContext.mouseWorldPos);
  TileID tileMouseType =
      gameState.hexGrid.PointToType(gameContext.mouseWorldPos);

  debugData.push_back(
      {"Mouse",
       {
           TextFormat("X,Y: %.1f,%.1f", gameContext.mouseWorldPos.x,
                      gameContext.mouseWorldPos.y),
           TextFormat("Tile Q,R: %i,%i", mapTile.q, mapTile.r),
           TextFormat("Type: %s",
                      gameState.hexGrid.TileToString(tileMouseType)),
           TextFormat("Clicked on: %s",
                      this->MouseMaskToString(gameContext.mouseMask)),
       }});

  // --- Player ---
  Vector2 playerPos = gameState.player.GetPosition();
  HexCoord playerTile = gameState.hexGrid.PointToHexCoord(playerPos);
  TileID tilePlayerType = gameState.hexGrid.PointToType(playerPos);
  debugData.push_back(
      {"Player",
       {
           TextFormat("X,Y: %.1f,%.1f", playerPos.x, playerPos.y),
           TextFormat("Tile Q,R: %i,%i", playerTile.q, playerTile.r),
           TextFormat("State:  %s", gameState.player.PlayerStateToString()),
           TextFormat("Face Dir: %s", gameState.player.PlayerDirToString()),
           TextFormat("Frame: %i", gameState.player.GetAnimationFrame()),
           TextFormat("Type: %s",
                      gameState.hexGrid.TileToString(tilePlayerType)),
           TextFormat("Speed[1/s]: %.2f",
                      gameState.player.GetSpeedTilesPerSecond()),
       }});

  // --- Tool Bar ---
  debugData.push_back(
      {"Tool Bar",
       {
           TextFormat("Item: %s", gameState.itemHandler.GetSelectedItemType()),
           TextFormat("Slot: %i", gameState.itemHandler.GetSelectionToolBar()),
       }});

  // Draw section
  Vector2 playerScreenPos = GetWorldToScreen2D(playerPos, gameState.camera);
  DrawCircleV(
      GetWorldToScreen2D(gameState.hexGrid.HexCoordToPoint(HexCoord(0, 0)),
                         gameState.camera),
      3.0f, RED);
  DrawCircleV(playerScreenPos, 3.0f, RED);

  // Draw text
  for (const DebugData &data : debugData) {
    fontHandler.DrawTextHackRegular(data.section.c_str(),
                                    {sectionPosX, currentY}, sectionColor);
    currentY += sectionGapY;
    currentY += subSectionGapY;

    // Draw sub-section text
    for (const std::string &subSection : data.subSection) {
      fontHandler.DrawTextHackRegular(
          subSection.c_str(), {subSectionPosX, currentY}, subSectionColor);
      currentY += subSectionGapY;
    }
    currentY += sectionGapY;
  }
};

void Game::Unload() {
  // Stop logic thread
  if (isRunning) {
    isRunning = false;
    mainToLogicCV.notify_one();
    if (logicThread.joinable()) {
      logicThread.join();
    }
  }

  gameState.hexGrid.Shutdown();
  gfxManager.UnloadAssets();
  fontHandler.UnloadFonts();
  UnloadFileData(hackFontRegular);
}

// --- Deinitialization ---
Game::~Game() {
  // Unload is safe to call multiple times because we check isRunning
  // However, UnloadAssets/Shutdown might not be.
  // Ideally, we should have a flag 'isUnloaded'.
  // For now, let's just ensure the thread is joined.
  if (logicThread.joinable()) {
    isRunning = false;
    mainToLogicCV.notify_one();
    logicThread.join();
  }
}
