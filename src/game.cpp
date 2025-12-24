#include "game.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "raymath.h"
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
Game::Game() {
  isRunning = true;
  isFullscreenMode = false;
  logicUpdateReady = false;
  logicUpdateDone = true;
  logicExecutionTime = 0.0;
  renderExecutionTime = 0.0;
  debugUpdateTimer = 0.0f;
  displayRenderTime = 0.0;
  displayLogicTime = 0.0;
  displayVisTime = 0.0;
  displayRamUsage = 0.0;

  gfxManager.LoadAssets(conf::TEXTURE_ATLAS_PATH);

  worldState.timer = 0.0f;
  worldState.updateGridTreshold = conf::GRID_UPDATE_PLAYER_MOVE_THRESHOLD;
  int fileSize = 0;
  hackFontRegular = LoadFileData(conf::FONT_HACK_REGULAR_PATH, &fileSize);

  worldState.hexGrid.InitGrid(conf::MAP_RADIUS);
  worldState.hexGrid.SetGFX_Manager(&gfxManager);
  worldState.hexGrid.SetCamRectPointer(&worldState.cameraRect);

  worldState.player.SetHexGrid(&worldState.hexGrid);
  worldState.player.SetGFX_Manager(&gfxManager);

  worldState.camera.target = conf::SCREEN_CENTER;
  worldState.camera.offset = conf::SCREEN_CENTER;
  worldState.camera.zoom = conf::INITIAL_CAMERA_ZOOM;
  worldState.camera.rotation = 0.0f;
  worldState.cameraRect = {0, 0, 0, 0};
  worldState.cameraTopLeft = {0, 0};

  fontHandler.LoadFonts();

  uiHandler.SetGFX_Manager(&gfxManager);
  uiHandler.SetItemHandler(&worldState.itemHandler);
  uiHandler.SetFontHandler(&fontHandler);
  uiHandler.SetHexGrid(&worldState.hexGrid);
  uiHandler.SetToolBarActive(true);

  SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

  // Initialise logic thread
  logicThread = std::thread(&Game::LogicLoop, this);

  renderStateIndex = 0;
  isUnloaded = false;
}

// --- Main Loop -> Render Thread ---
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

    // 3. Render (Main Thread) - Uses current WorldState
    {
      auto startRender = std::chrono::high_resolution_clock::now();
      BeginDrawing();
      ClearBackground(WHITE);

      BeginMode2D(renderStates[renderStateIndex].camera);
      gfxManager.RenderLayer(drawMask::GROUND_0);
      gfxManager.RenderLayer(drawMask::GROUND_1);
      gfxManager.RenderLayer(drawMask::SHADOW);
      gfxManager.RenderLayer(drawMask::ON_GROUND);
      EndMode2D();

      gfxManager.RenderLayer(drawMask::UI_0);
      gfxManager.RenderLayer(drawMask::UI_1);

      DrawDebugOverlay(conf::IS_DEBUG_OVERLAY_ENABLED);

      EndDrawing();
      auto endRender = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsedRender =
          endRender - startRender;
      renderExecutionTime = elapsedRender.count();
    }

    // 4. Sync: Wait for Logic to finish (Barrier)
    // We wait here to ensure we don't start the next frame's input gathering
    // until the logic is done with the current frame's input.
    // Also, this ensures that the WorldState is consistent before we loop
    // again.
    {
      std::unique_lock<std::mutex> lock(logicMutex);
      logicToMainCV.wait(lock, [this] { return logicUpdateDone; });
    }

    // Swap Render State Buffers
    renderStateIndex = !renderStateIndex;

    gfxManager.SwapBuffers();
  }

  // Signal logic thread to stop
  isRunning = false;
  mainToLogicCV.notify_one();
}

// --- Logic Loop -> Logic Thread  ---
void Game::LogicLoop() {
  while (isRunning) {
    std::unique_lock<std::mutex> lock(logicMutex);
    mainToLogicCV.wait(lock, [this] { return logicUpdateReady || !isRunning; });

    if (!isRunning)
      break;

    RunLogic();

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

  frameContext.deltaTime = GetFrameTime();

  frameContext.screenWidth = GetScreenWidth();
  frameContext.screenHeight = GetScreenHeight();

  // Update Camera Offset to match new screen size
  worldState.camera.offset = Vector2{(float)frameContext.screenWidth / 2.0f,
                                     (float)frameContext.screenHeight / 2.0f};

  frameContext.mouseScreenPos = GetMousePosition();

  frameContext.mouseWorldPos =
      GetScreenToWorld2D(frameContext.mouseScreenPos, worldState.camera);

  frameContext.inputs.mousePress.left = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  frameContext.inputs.mousePress.right =
      IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

  frameContext.inputs.keyPress.One = IsKeyPressed(KEY_ONE);
  frameContext.inputs.keyPress.Two = IsKeyPressed(KEY_TWO);
  frameContext.inputs.keyPress.Three = IsKeyPressed(KEY_THREE);
  frameContext.inputs.keyPress.Four = IsKeyPressed(KEY_FOUR);
  frameContext.inputs.keyPress.Five = IsKeyPressed(KEY_FIVE);
  frameContext.inputs.keyPress.Six = IsKeyPressed(KEY_SIX);
  frameContext.inputs.keyPress.Seven = IsKeyPressed(KEY_SEVEN);
  frameContext.inputs.keyPress.Eight = IsKeyPressed(KEY_EIGHT);
  frameContext.inputs.keyPress.Nine = IsKeyPressed(KEY_NINE);
  frameContext.inputs.keyPress.Zero = IsKeyPressed(KEY_ZERO);

  frameContext.inputs.keyPress.Left = IsKeyDown(KEY_A);
  frameContext.inputs.keyPress.Right = IsKeyDown(KEY_D);
  frameContext.inputs.keyPress.Up = IsKeyDown(KEY_W);
  frameContext.inputs.keyPress.Down = IsKeyDown(KEY_S);

  worldState.cameraTopLeft =
      GetScreenToWorld2D(Vector2{0, 0}, worldState.camera);

  float camWidth = (float)frameContext.screenWidth / worldState.camera.zoom;
  float camHeight = (float)frameContext.screenHeight / worldState.camera.zoom;

  worldState.cameraRect = {worldState.cameraTopLeft.x,
                           worldState.cameraTopLeft.y, camWidth, camHeight};
}

void Game::RunLogic() {
  auto startLogic = std::chrono::high_resolution_clock::now();

  // Use frameContext and worldState
  worldState.timer += frameContext.deltaTime;

  // Update UI Layout
  uiHandler.UpdateScreenSize(frameContext.screenWidth,
                             frameContext.screenHeight);

  // Update Grid
  worldState.hexGrid.Update(worldState.camera, frameContext.deltaTime);
  uiHandler.Update(frameContext.mouseWorldPos);

  // Get selected tool bar slot
  int selToolBarSlot = worldState.itemHandler.GetSelectionToolBar();

  // Check if player selected new slow by key press
  selToolBarSlot = uiHandler.GetToolBarSelection(frameContext.inputs.keyPress,
                                                 selToolBarSlot);

  // Process left mouse click
  if (frameContext.inputs.mousePress.left) {
    ProccesLeftMouseClick(&selToolBarSlot);
  }

  // Process left right click
  if (frameContext.inputs.mousePress.right) {
    HexCoord clickedHex =
        worldState.hexGrid.PointToHexCoord(frameContext.mouseWorldPos);
    worldState.hexGrid.SetTile(clickedHex, tile::NULL_ID);
  }

  // Set selected tool bar slot
  uiHandler.SetSelectedItem(selToolBarSlot);
  worldState.itemHandler.SetItemSelection(selToolBarSlot);

  // Player Update
  worldState.player.Update(&frameContext.inputs.keyPress,
                           frameContext.deltaTime);

  // Update Camera Target (Logic)
  worldState.camera.target = worldState.player.GetPosition();

  // --- Update Render State Snapshot (Back Buffer) ---
  RenderState &rs = renderStates[!renderStateIndex];
  rs.camera = worldState.camera;

  rs.tilesTotal = worldState.hexGrid.GetTilesInTotal();
  rs.tilesUsed = worldState.hexGrid.GetTilesInUse();
  rs.tilesVisible = worldState.hexGrid.GetTilesVisible();
  rs.mapRadius = worldState.hexGrid.GetMapRadius();
  rs.visCalcTime = worldState.hexGrid.GetVisCalcTime();

  rs.mouseTileCoord =
      worldState.hexGrid.PointToHexCoord(frameContext.mouseWorldPos);
  rs.mouseTileType = worldState.hexGrid.PointToType(frameContext.mouseWorldPos);

  rs.playerPos = worldState.player.GetPosition();
  rs.playerTileCoord = worldState.hexGrid.PointToHexCoord(rs.playerPos);
  rs.playerTileID = worldState.hexGrid.PointToType(rs.playerPos);
  rs.playerStateStr = worldState.player.PlayerStateToString();
  rs.playerDirStr = worldState.player.PlayerDirToString();
  rs.playerFrame = worldState.player.GetAnimationFrame();
  rs.playerSpeed = worldState.player.GetSpeedTilesPerSecond();

  rs.selectedItemType = worldState.itemHandler.GetSelectedItemType();
  rs.selectedToolBarSlot = worldState.itemHandler.GetSelectionToolBar();

  // Count passed time
  auto endLogic = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsedLogic =
      endLogic - startLogic;
  logicExecutionTime = elapsedLogic.count();
}

void Game::ProccesLeftMouseClick(int *toolBarSel) {

  // Clicked on item bar
  if (uiHandler.GetToolBarAvailability() &&
      CheckCollisionPointRec(
          frameContext.mouseScreenPos, // ToolBar is Screen Space usually?
          uiHandler.GetToolBarRect())) {
    frameContext.mouseMask =
        mouseMask::ITEM_BAR; // Update local state if needed
    *toolBarSel = uiHandler.GetItemSlotAt(frameContext.mouseScreenPos);

    // Clicked on ground
  } else {
    frameContext.mouseMask = mouseMask::GROUND;

    HexCoord clickedHex =
        worldState.hexGrid.PointToHexCoord(frameContext.mouseWorldPos);
    ItemStack *selectedItem =
        worldState.itemHandler.GetToolBarItemPointer(*toolBarSel);

    if (selectedItem->itemID == item::AXE) {
      Vector2 playerPos = worldState.player.GetPosition();
      Vector2 clickPos = worldState.hexGrid.HexCoordToPoint(clickedHex);

      if (Vector2Distance(playerPos, clickPos) < conf::TILE_RESOLUTION * 2.5f) {
        if (worldState.hexGrid.RemoveResource(clickedHex, rsrc::TREE)) {
          worldState.player.Chop(clickedHex);
          worldState.itemHandler.AddItem(item::WOOD, 1);
        }
      }
    } else {
      tile::id tileToPlace =
          worldState.itemHandler.ConvertItemToTileID(selectedItem->itemID);

      if (tileToPlace != tile::NULL_ID &&
          worldState.hexGrid.SetTile(clickedHex, tileToPlace)) {
        worldState.itemHandler.TakeItemFromToolBar(selectedItem, 1);
      }
    }
  }
}

const char *Game::MouseMaskToString(mouseMask::id m) {
  switch (m) {
  case mouseMask::NULL_ID:
    return "Null";
  case mouseMask::GROUND:
    return "Ground";
  case mouseMask::ITEM_BAR:
    return "Tool Bar";
  default:
    return "Undefined";
  }
}

// --- Debug overlay ---
void Game::DrawDebugOverlay(bool is_enabled) {
  if (!is_enabled)
    return;

  float sectionPosX = conf::DEBUG_OVERLAY_SECTION_X;
  int sectionPosY = conf::DEBUG_OVERLAY_SECTION_Y;
  int sectionGapY = conf::DEBUG_OVERLAY_SECTION_Y_GAP;
  // int sectionFontSize = conf::DEBUG_OVERLAY_SECTION_FONT_SIZE;
  Color sectionColor = conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float subSectionPosX = conf::DEBUG_OVERLAY_SUBSECTION_X_POS;
  int subSectionGapY = conf::DEBUG_OVERLAY_SUBSECTION_Y_GAP;
  // int subSectionFontSize = conf::DEBUG_OVERLAY_SUBSECTION_FONT_SIZE;
  Color subSectionColor = conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float currentY = sectionPosY;

  debugUpdateTimer += GetFrameTime();
  if (debugUpdateTimer >= 1.0f) {
    displayRenderTime = renderExecutionTime.load();
    displayLogicTime = logicExecutionTime.load();
    displayVisTime = renderStates[renderStateIndex].visCalcTime;
    displayRamUsage = GetRamUsageMB();
    debugUpdateTimer = 0.0f;
  }

  const RenderState &rs = renderStates[renderStateIndex];

  debugData.clear();
  debugData.push_back(
      {"Resources",
       {
           TextFormat("FPS: %i", GetFPS()),
           TextFormat("RAM: %.2f MB", displayRamUsage),
           TextFormat("Screen: %ix%i", GetScreenWidth(), GetScreenHeight()),
           TextFormat("Render: %ix%i", GetRenderWidth(), GetRenderHeight()),
           TextFormat("Tiles Total: %i", rs.tilesTotal),
           TextFormat("Tiles Used: %i", rs.tilesUsed),
           TextFormat("Tiles Visible: %i", rs.tilesVisible),
           TextFormat("Map radius: %i", rs.mapRadius),
           TextFormat("Render Time: %.2f ms", displayRenderTime),
           TextFormat("Logic Time: %.2f ms", displayLogicTime),
           TextFormat("Culling Time: %.2f ms", displayVisTime),
       }});

  // Use currentInput for debug display
  // HexCoord mapTile =
  //     worldState.hexGrid.PointToHexCoord(frameContext.mouseWorldPos);
  // TileID tileMouseType =
  //     worldState.hexGrid.PointToType(frameContext.mouseWorldPos);

  debugData.push_back(
      {"Mouse",
       {
           TextFormat("X,Y: %.1f,%.1f", frameContext.mouseWorldPos.x,
                      frameContext.mouseWorldPos.y),
           TextFormat("Tile Q,R: %i,%i", rs.mouseTileCoord.q,
                      rs.mouseTileCoord.r),
           TextFormat("Type: %s",
                      worldState.hexGrid.TileToString(rs.mouseTileType)),
           TextFormat("Clicked on: %s",
                      this->MouseMaskToString(frameContext.mouseMask)),
       }});

  // --- Player ---
  // Vector2 playerPos = worldState.player.GetPosition();
  // HexCoord playerTile = worldState.hexGrid.PointToHexCoord(playerPos);
  // TileID tilePlayerType = worldState.hexGrid.PointToType(playerPos);
  debugData.push_back(
      {"Player",
       {
           TextFormat("X,Y: %.1f,%.1f", rs.playerPos.x, rs.playerPos.y),
           TextFormat("Tile Q,R: %i,%i", rs.playerTileCoord.q,
                      rs.playerTileCoord.r),
           TextFormat("State:  %s", rs.playerStateStr.c_str()),
           TextFormat("Face Dir: %s", rs.playerDirStr.c_str()),
           TextFormat("Frame: %i", rs.playerFrame),
           TextFormat("Type: %s",
                      worldState.hexGrid.TileToString(rs.playerTileID)),
           TextFormat("Speed[1/s]: %.2f", rs.playerSpeed),
       }});

  // --- Tool Bar ---
  debugData.push_back({"Tool Bar",
                       {
                           TextFormat("Item: %s", rs.selectedItemType.c_str()),
                           TextFormat("Slot: %i", rs.selectedToolBarSlot),
                       }});

  // Draw section
  Vector2 playerScreenPos = GetWorldToScreen2D(rs.playerPos, rs.camera);
  DrawCircleV(
      GetWorldToScreen2D(worldState.hexGrid.HexCoordToPoint(HexCoord(0, 0)),
                         rs.camera),
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

Game::~Game() { Unload(); }

void Game::Unload() {
  if (isUnloaded)
    return;
  isUnloaded = true;

  isRunning = false;
  mainToLogicCV.notify_all();
  if (logicThread.joinable()) {
    logicThread.join();
  }

  gfxManager.UnloadAssets();
  fontHandler.UnloadFonts();

  if (hackFontRegular != nullptr) {
    UnloadFileData(hackFontRegular);
    hackFontRegular = nullptr;
  }
}
