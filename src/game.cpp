#include "game.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include <string>

// --- Initialization ---
Game::Game()
    : isRunning(true), logicUpdateReady(false), logicUpdateDone(true),
      logicExecutionTime(0.0), renderExecutionTime(0.0), debugUpdateTimer(0.0f),
      displayRenderTime(0.0), displayLogicTime(0.0), displayVisTime(0.0) {
  GFX_Manager.LoadAssets(Conf::TEXTURE_ATLAS_PATH);

  gameState.timer = 0.0f;
  gameState.updateGridTreshold = Conf::GRID_UPDATE_PLAYER_MOVE_THRESHOLD;
  int fileSize = 0;
  hackFontRegular = LoadFileData(Conf::FONT_HACK_REGULAR_PATH, &fileSize);

  gameState.hexGrid.InitGrid(Conf::MAP_RADIUS);
  gameState.hexGrid.SetGFX_Manager(&GFX_Manager);
  gameState.hexGrid.SetCamRectPointer(&gameState.cameraRect);

  gameState.player.SetHexGrid(&gameState.hexGrid);
  gameState.player.SetGFX_Manager(&GFX_Manager);

  gameState.camera.target = Conf::SCREEN_CENTER;
  gameState.camera.offset = Conf::SCREEN_CENTER;
  gameState.camera.zoom = Conf::INITIAL_CAMERA_ZOOM;
  gameState.camera.rotation = 0.0f;
  gameState.cameraRect = {0, 0, 0, 0};
  gameState.cameraTopLeft = {0, 0};

  fontHandler.LoadFonts();

  uiHandler.SetGFX_Manager(&GFX_Manager);
  uiHandler.SetItemHandler(&gameState.itemHandler);
  uiHandler.SetFontHandler(&fontHandler);
  uiHandler.SetIO_Handler(&ioHandler);
  uiHandler.SetHexGrid(&gameState.hexGrid);
  uiHandler.SetToolBarActive(true);

  // Start the logic thread
  logicThread = std::thread(&Game::LogicLoop, this);
}

// --- Main Loop (Rendering Thread) ---
void Game::GameLoop() {
  while (!WindowShouldClose()) {
    // 1. Gather Input (Main Thread)
    UpdateInputs();

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
      GFX_Manager.RenderLayer(DRAW_MASK_GROUND_0);
      GFX_Manager.RenderLayer(DRAW_MASK_GROUND_1);
      GFX_Manager.RenderLayer(DRAW_MASK_SHADOW);
      GFX_Manager.RenderLayer(DRAW_MASK_ON_GROUND);
      EndMode2D();

      GFX_Manager.RenderLayer(DRAW_MASK_UI_0);
      GFX_Manager.RenderLayer(DRAW_MASK_UI_1);

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

    GFX_Manager.SwapBuffers();
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
  if (IsKeyPressed(KEY_O)) {
    ToggleFullscreen();
  }

  // Populate currentInput struct
  currentInput.frameTime = GetFrameTime();
  currentInput.mouseScreenPos = GetMousePosition();

  // Calculate World Position here (Main Thread)
  currentInput.mouseWorldPos =
      GetScreenToWorld2D(currentInput.mouseScreenPos, gameState.camera);

  currentInput.leftMouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  currentInput.rightMouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

  currentInput.keyOne = IsKeyPressed(KEY_ONE);
  currentInput.keyTwo = IsKeyPressed(KEY_TWO);
  currentInput.keyThree = IsKeyPressed(KEY_THREE);
  currentInput.keyFour = IsKeyPressed(KEY_FOUR);
  currentInput.keyFive = IsKeyPressed(KEY_FIVE);
  currentInput.keySix = IsKeyPressed(KEY_SIX);
  currentInput.keySeven = IsKeyPressed(KEY_SEVEN);
  currentInput.keyEight = IsKeyPressed(KEY_EIGHT);
  currentInput.keyNine = IsKeyPressed(KEY_NINE);
  currentInput.keyZero = IsKeyPressed(KEY_ZERO);

  currentInput.playerInput.moveLeft = IsKeyDown(KEY_A);
  currentInput.playerInput.moveRight = IsKeyDown(KEY_D);
  currentInput.playerInput.moveUp = IsKeyDown(KEY_W);
  currentInput.playerInput.moveDown = IsKeyDown(KEY_S);

  // Calculate Camera Rects for Logic (Must be done in Main Thread)
  gameState.cameraTopLeft = GetScreenToWorld2D(Vector2{0, 0}, gameState.camera);
  gameState.cameraRect = {gameState.cameraTopLeft.x, gameState.cameraTopLeft.y,
                          Conf::CAMERA_WIDTH, Conf::CAMERA_HEIGHT};
}

void Game::RunLogic() {
  // Use currentInput and gameState
  gameState.timer += currentInput.frameTime;

  // IO Handler Update (Logic side)
  ioHandler.SetScaledMousePos(currentInput.mouseWorldPos);
  ioHandler.SetRealMousePos(currentInput.mouseScreenPos);

  // Update Grid
  gameState.hexGrid.Update(
      gameState.camera,
      currentInput.frameTime); // Note: UpdateTileVisibility might need camera
  uiHandler.Update();

  // --- Logic equivalent of UpdateInputs ---
  // We use currentInput instead of calling IsKeyDown etc.

  // NOTE: ioHandler.GetScaledMousePos() logic is duplicated or skipped?
  // ioHandler scales mouse based on zoom.
  // Vector2 scaledMouse = currentInput.mouseWorldPos; // Close enough?
  // Let's assume WorldPos is what we want for "RealMousePos".
  // For "Scaled", IO_Handler applies some offset/zoom logic.
  // Let's use currentInput.mouseWorldPos for interactions.

  int toolBarSel = gameState.itemHandler.GetSelectionToolBar();

  if (currentInput.leftMouseClicked) {
    // Clicked on item bar
    if (uiHandler.GetToolBarAvailability() &&
        CheckCollisionPointRec(
            currentInput.mouseScreenPos, // ToolBar is Screen Space usually?
            uiHandler.GetToolBarRect())) {
      // ioHandler.SetMouseMask(MOUSE_MASK_ITEM_BAR);
      currentInput.mouseMask =
          MOUSE_MASK_ITEM_BAR; // Update local state if needed
      toolBarSel = uiHandler.GetItemSlotAt(currentInput.mouseScreenPos);

    } else {
      // ioHandler.SetMouseMask(MOUSE_MASK_PLAY_GROUND);
      currentInput.mouseMask = MOUSE_MASK_PLAY_GROUND;

      HexCoord clickedHex =
          gameState.hexGrid.PointToHexCoord(currentInput.mouseWorldPos);
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

  if (currentInput.rightMouseClicked) {
    HexCoord clickedHex =
        gameState.hexGrid.PointToHexCoord(currentInput.mouseWorldPos);
    gameState.hexGrid.SetTile(clickedHex, TILE_NULL);
  }

  // Keyboard selection
  KeyboardInputState keyboardState;
  keyboardState.keyOne = currentInput.keyOne;
  keyboardState.keyTwo = currentInput.keyTwo;
  keyboardState.keyThree = currentInput.keyThree;
  keyboardState.keyFour = currentInput.keyFour;
  keyboardState.keyFive = currentInput.keyFive;
  keyboardState.keySix = currentInput.keySix;
  keyboardState.keySeven = currentInput.keySeven;
  keyboardState.keyEight = currentInput.keyEight;
  keyboardState.keyNine = currentInput.keyNine;
  keyboardState.keyZero = currentInput.keyZero;

  toolBarSel = ioHandler.GetToolBarSelction(toolBarSel, keyboardState);

  uiHandler.SetSelectedItem(toolBarSel);
  gameState.itemHandler.SetItemSelection(toolBarSel);

  // Player Update
  gameState.player.Update(currentInput.playerInput, currentInput.frameTime);

  // Update Camera Target (Logic)
  gameState.camera.target = gameState.player.GetPosition();

  // Calculate Camera Rects for Next Frame (Logic)
  // Warning: GetScreenToWorld2D is Raylib (Main Thread).
  // We can't update relativeCenter/cameraTopLeft accurately here without
  // Raylib. However, these are used for Culling (HexGrid). Strategy: Calculate
  // them in Main Thread (UpdateInputs) and pass them in InputState? OR
  // calculate them in Main Thread AFTER Logic is done? Let's do it in Main
  // Thread inside GameLoop (Render phase) or before Logic. Logic needs them for
  // HexGrid.Update? HexGrid.Update uses camera to calculate visibility. It uses
  // `GetWorldToScreen2D` internally? If so, HexGrid.Update MUST be on Main
  // Thread. Let's check HexGrid.Update.
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
    debugUpdateTimer = 0.0f;
  }

  debugData.clear();
  debugData.push_back(
      {"Resources",
       {
           TextFormat("FPS: %i", GetFPS()),
           TextFormat("Tiles Total: %i", gameState.hexGrid.GetTilesInTotal()),
           TextFormat("Tiles Used: %i", gameState.hexGrid.GetTilesInUse()),
           TextFormat("Tiles Visible: %i", gameState.hexGrid.GetTilesVisible()),
           TextFormat("Map radius: %i", gameState.hexGrid.GetMapRadius()),
           TextFormat("Render Time: %.3f ms", displayRenderTime),
           TextFormat("Logic Time: %.3f ms", displayLogicTime),
           TextFormat("Culling Time: %.3f ms", displayVisTime),
       }});

  // Use currentInput for debug display
  HexCoord mapTile =
      gameState.hexGrid.PointToHexCoord(currentInput.mouseWorldPos);
  TileID tileMouseType =
      gameState.hexGrid.PointToType(currentInput.mouseWorldPos);

  debugData.push_back(
      {"Mouse",
       {
           TextFormat("X,Y: %.1f,%.1f", currentInput.mouseWorldPos.x,
                      currentInput.mouseWorldPos.y),
           TextFormat("Tile Q,R: %i,%i", mapTile.q, mapTile.r),
           TextFormat("Type: %s",
                      gameState.hexGrid.TileToString(tileMouseType)),
           TextFormat("Clicked on: %s",
                      this->MouseMaskToString(currentInput.mouseMask)),
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
  GFX_Manager.UnloadAssets();
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
