#include "game.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "raylib.h"

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

  frameContext = {};
  frameContext.selToolBarSlot = 0;

  gfxManager.LoadAssets(conf::TEXTURE_ATLAS_PATH);

  worldState.timer = 0.0f;
  worldState.updateGridTreshold = conf::GRID_UPDATE_PLAYER_MOVE_THRESHOLD;
  int fileSize = 0;
  hackFontRegular = LoadFileData(conf::FONT_HACK_REGULAR_PATH, &fileSize);

  worldState.hexGrid.InitGrid(conf::MAP_RADIUS);
  worldState.hexGrid.SetGFX_Manager(&gfxManager);
  worldState.hexGrid.SetCamRectPointer(&worldState.cameraRect);

  worldState.player.SetHexGrid(&worldState.hexGrid);
  worldState.player.SetItemHandler(&worldState.itemHandler);
  worldState.player.SetGFX_Manager(&gfxManager);
  worldState.player.SetUI_Handler(&uiHandler);
  worldState.player.SetFrameContext(&frameContext);

  worldState.camera.target = conf::SCREEN_CENTER;
  worldState.camera.offset = conf::SCREEN_CENTER;
  worldState.camera.zoom = conf::INITIAL_CAMERA_ZOOM;
  worldState.camera.rotation = 0.0f;
  worldState.cameraRect = {0, 0, 0, 0};
  worldState.cameraTopLeft = {0, 0};

  worldState.itemHandler.SetFrameContext(&frameContext);

  fontHandler.LoadFonts();

  uiHandler.SetGFX_Manager(&gfxManager);
  uiHandler.SetItemHandler(&worldState.itemHandler);
  uiHandler.SetFontHandler(&fontHandler);
  uiHandler.SetHexGrid(&worldState.hexGrid);
  uiHandler.SetToolBarActive(true);
  uiHandler.SetFrameContext(&frameContext);

  debugger.SetManagers(&gfxManager, &fontHandler);

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

    UpdateFrameContext();
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
      gfxManager.RenderLayer(drawMask::DEBUG_OVERLAY);

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

void Game::UpdateFrameContext() {
  if (IsKeyPressed(KEY_F)) {
    isFullscreenMode = !isFullscreenMode;
    ToggleBorderlessWindowed();
  }

  frameContext.deltaTime = GetFrameTime();

  frameContext.screenWidth = GetScreenWidth();
  frameContext.screenHeight = GetScreenHeight();

  frameContext.pos.mouseScreen = GetMousePosition();

  frameContext.inputs.mouseClick.left = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  frameContext.inputs.mouseClick.right =
      IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

  frameContext.inputs.keyPress.one = IsKeyPressed(KEY_ONE);
  frameContext.inputs.keyPress.two = IsKeyPressed(KEY_TWO);
  frameContext.inputs.keyPress.three = IsKeyPressed(KEY_THREE);
  frameContext.inputs.keyPress.four = IsKeyPressed(KEY_FOUR);
  frameContext.inputs.keyPress.five = IsKeyPressed(KEY_FIVE);
  frameContext.inputs.keyPress.six = IsKeyPressed(KEY_SIX);
  frameContext.inputs.keyPress.seven = IsKeyPressed(KEY_SEVEN);
  frameContext.inputs.keyPress.eight = IsKeyPressed(KEY_EIGHT);
  frameContext.inputs.keyPress.nine = IsKeyPressed(KEY_NINE);
  frameContext.inputs.keyPress.zero = IsKeyPressed(KEY_ZERO);

  frameContext.inputs.keyPress.left = IsKeyDown(KEY_A);
  frameContext.inputs.keyPress.right = IsKeyDown(KEY_D);
  frameContext.inputs.keyPress.up = IsKeyDown(KEY_W);
  frameContext.inputs.keyPress.down = IsKeyDown(KEY_S);
}

void Game::RunLogic() {
  auto startLogic = std::chrono::high_resolution_clock::now();

  // --- Update delta time ---
  worldState.timer += frameContext.deltaTime;

  // --- Update mouse ---
  frameContext.pos.mouseWorld =
      GetScreenToWorld2D(frameContext.pos.mouseScreen, worldState.camera);
  frameContext.mouseMask = uiHandler.UpdateMouseMask();

  // --- Update Selected Tool Bar Slot ---
  frameContext.selToolBarSlot = uiHandler.GetToolBarSelection();

  // --- Get position of hovered resource ---
  frameContext.pos.hoveredTile =
      worldState.hexGrid.PointToTile(frameContext.pos.mouseScreen);

  // Player Update
  worldState.player.Update();

  // --- Update camera ---
  worldState.camera.offset = Vector2{(float)frameContext.screenWidth / 2.0f,
                                     (float)frameContext.screenHeight / 2.0f};
  worldState.cameraTopLeft =
      GetScreenToWorld2D(Vector2{0, 0}, worldState.camera);
  float camWidth = (float)frameContext.screenWidth / worldState.camera.zoom;
  float camHeight = (float)frameContext.screenHeight / worldState.camera.zoom;
  worldState.cameraRect = {worldState.cameraTopLeft.x,
                           worldState.cameraTopLeft.y, camWidth, camHeight};
  worldState.camera.target = worldState.player.GetPosition();

  // --- Update UI Layout ---
  uiHandler.UpdateScreenSize(frameContext.screenWidth,
                             frameContext.screenHeight);

  // --- Update Grid ---
  worldState.hexGrid.Update(worldState.camera, frameContext.deltaTime);
  uiHandler.Update(frameContext.pos.mouseWorld);

  // --- Process right click ---
  if (frameContext.inputs.mouseClick.right) {
    HexCoord clickedHex =
        worldState.hexGrid.PointToHexCoord(frameContext.pos.mouseWorld);
    worldState.hexGrid.SetTile(clickedHex, tile::NULL_ID);
  }

  // --- Update Render State Snapshot (Back Buffer) ---
  RenderState &rs = renderStates[!renderStateIndex];
  rs.camera = worldState.camera;

  rs.tilesTotal = worldState.hexGrid.GetTilesInTotal();
  rs.tilesUsed = worldState.hexGrid.GetTilesInUse();
  rs.tilesVisible = worldState.hexGrid.GetTilesVisible();
  rs.mapRadius = worldState.hexGrid.GetMapRadius();
  rs.visCalcTime = worldState.hexGrid.GetVisCalcTime();

  rs.mouseTileCoord =
      worldState.hexGrid.PointToHexCoord(frameContext.pos.mouseWorld);
  rs.mouseTileType =
      worldState.hexGrid.PointToType(frameContext.pos.mouseWorld);

  rs.playerPos = worldState.player.GetPosition();
  rs.playerTileCoord = worldState.hexGrid.PointToHexCoord(rs.playerPos);
  rs.playerTileID = worldState.hexGrid.PointToType(rs.playerPos);
  rs.playerStateStr = worldState.player.PlayerStateToString();
  rs.playerDirStr = worldState.player.PlayerDirToString();
  rs.playerFrame = worldState.player.GetAnimationFrame();
  rs.playerSpeed = worldState.player.GetSpeedTilesPerSecond();

  rs.selectedItemType = worldState.itemHandler.GetSelectedItemType();
  rs.selectedToolBarSlot = frameContext.selToolBarSlot;

  debugger.Update(rs, frameContext.deltaTime, logicExecutionTime.load(),
                  renderExecutionTime.load());

  // Count passed time
  auto endLogic = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsedLogic =
      endLogic - startLogic;
  logicExecutionTime = elapsedLogic.count();
}

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
