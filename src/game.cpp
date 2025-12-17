#include "game.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include <string>

// --- Initialization ---
Game::Game() {
  InitWindow(Conf::SCREEN_WIDTH, Conf::SCREEN_HEIGHT, Conf::WINDOW_TITLE);
  SetTargetFPS(Conf::FPS_MAX);
  graphicsManager.LoadAssets(Conf::TA_PATH);

  timer = 0.0f;
  updateGridTreshold = Conf::TRIGGER_UPDATE_GRID;
  int fileSize = 0;
  hackFontRegular = LoadFileData(Conf::FONT_HACK_REGULAR_PATH, &fileSize);

  hexGrid.InitGrid(Conf::MAP_SIZE);
  hexGrid.SetGFX_Manger(&graphicsManager);
  hexGrid.SetCamRectPointer(&this->cameraRect);

  player.SetHexGrid(&hexGrid);
  player.SetGFX_Manger(&graphicsManager);

  camera.target = Conf::SCREEN_CENTER;
  camera.offset = Conf::SCREEN_CENTER;
  camera.zoom = Conf::CAMERA_ZOOM;
  camera.rotation = 0.0f;
  cameraRect = {0, 0, 0, 0};
  cameraTopLeft = {0, 0};

  fontHandler.LoadFonts();

  uiHandler.SetGFX_Manger(&graphicsManager);
  uiHandler.SetItemHandler(&itemHandler);
  uiHandler.SetFontHandler(&fontHandler);
  uiHandler.SetIO_Handler(&ioHandler);
  uiHandler.SetHexGrid(&hexGrid);
  uiHandler.SetToolBarActive(true);
}

// --- Main Loop ---
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // --- Update ---
    timer += GetTime();

    ioHandler.UpdateMousePos(camera);
    ProcessInputs();
    relativeCenter = GetScreenToWorld2D(Conf::SCREEN_CENTER, camera);
    cameraTopLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
    cameraRect = {cameraTopLeft.x, cameraTopLeft.y, Conf::CAMERA_WIDTH,
                  Conf::CAMERA_HEIGTH};

    camera.target = player.GetPosition();

    player.Update();
    hexGrid.Update(camera);
    uiHandler.Update();

    // --- Draw ---
    BeginDrawing();
    ClearBackground(WHITE);

    // --- Camera View ---
    BeginMode2D(camera);

    graphicsManager.RenderDrawData(DRAW_MASK_GROUND_0);
    graphicsManager.RenderDrawData(DRAW_MASK_GROUND_1);
    graphicsManager.RenderDrawData(DRAW_MASK_SHADOW);
    graphicsManager.RenderDrawData(DRAW_MASK_ON_GROUND);

    // --- End Camera View ---
    EndMode2D();
    graphicsManager.RenderDrawData(DRAW_MASK_UI_0);
    graphicsManager.RenderDrawData(DRAW_MASK_UI_1);

    DrawDebugOverlay(Conf::DEBUG_FLAG);

    EndDrawing();
  }
}

void Game::ProcessInputs() {

  ioHandler.GetScaledMousePos();
  int toolBarSel = itemHandler.GetSelectionToolBar();

  // --- Mouse ---
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

    // Clicked on item bar
    if (uiHandler.GetToolBarAvailability() &&
        CheckCollisionPointRec(ioHandler.GetRealMousePos(),
                               uiHandler.GetToolBarRect())) {
      ioHandler.SetMouseMask(MOUSE_MASK_ITEM_BAR);
      toolBarSel = uiHandler.GetItemSlotAt(GetMousePosition());

      // Clicked on ground
    } else {
      ioHandler.SetMouseMask(MOUSE_MASK_PLAY_GROUND);
      HexCoord clickedHex =
          hexGrid.PointToHexCoord(ioHandler.GetScaledMousePos());
      Item *selectedItem = itemHandler.GetToolBarItemPointer(toolBarSel);
      TileID tileToPlace = itemHandler.ConvertItemToTileID(selectedItem->id);

      if (selectedItem->id == ITEM_NULL) {
      } else if (hexGrid.SetTile(clickedHex, tileToPlace)) {
        itemHandler.TakeItemFromToolBar(selectedItem, 1);
      };
    }
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    HexCoord clickedHex =
        hexGrid.PointToHexCoord(ioHandler.GetScaledMousePos());
    hexGrid.SetTile(clickedHex, TILE_NULL);
  }

  toolBarSel = ioHandler.GetToolBarSelction(toolBarSel);
  // --- Keyboard ---

  uiHandler.SetSelectedItem(toolBarSel);
  itemHandler.SetItemSelection(toolBarSel);
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

  float sectionPosX = Conf::DEBUG_OVERLAY_SECTION_X_POS;
  int sectionPosY = Conf::DEBUG_OVERLAY_SECTION_Y_POS;
  int sectionGapY = Conf::DEBUG_OVERLAY_SECTION_Y_GAP;
  int sectionFontSize = Conf::DEBUG_OVERLAY_SECTION_FONT_SIZE;
  Color sectionColor = Conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float subSectionPosX = Conf::DEBUG_OVERLAY_SUBSECTION_X_POS;
  int subSectionGapY = Conf::DEBUG_OVERLAY_SUBSECTION_Y_GAP;
  int subSectionFontSize = Conf::DEBUG_OVERLAY_SUBSECTION_FONT_SIZE;
  Color subSectionColor = Conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float currentY = sectionPosY;

  debugData.clear();
  debugData.push_back(
      {"Resources",
       {
           TextFormat("FPS: %i", GetFPS()),
           TextFormat("Render Rect Timer: %0.2f", hexGrid.GetRenderRectTimer()),
           TextFormat("Tiles Total: %i", hexGrid.GetTilesInTotal()),
           TextFormat("Tiles Used: %i", hexGrid.GetTilesInUse()),
           TextFormat("Tiles Visible: %i", hexGrid.GetTilesVisible()),
           TextFormat("Map radius: %i", hexGrid.GetMapRadius()),
       }});

  HexCoord mapTile = hexGrid.PointToHexCoord(ioHandler.GetScaledMousePos());
  TileID tileMouseType = hexGrid.PointToType(ioHandler.GetScaledMousePos());
  debugData.push_back(
      {"Mouse",
       {
           TextFormat("X,Y: %.1f,%.1f", ioHandler.GetScaledMousePos().x,
                      ioHandler.GetScaledMousePos().y),
           TextFormat("Tile Q,R: %i,%i", mapTile.q, mapTile.r),
           TextFormat("Type: %s", hexGrid.TileToString(tileMouseType)),
           TextFormat("Clicked on: %s",
                      this->MouseMaskToString(ioHandler.GetMouseMask())),
       }});

  // --- Player ---
  Vector2 playerPos = player.GetPosition();
  HexCoord playerTile = hexGrid.PointToHexCoord(playerPos);
  TileID tilePlayerType = hexGrid.PointToType(playerPos);
  debugData.push_back(
      {"Player",
       {
           TextFormat("X,Y: %.1f,%.1f", playerPos.x, playerPos.y),
           TextFormat("Tile Q,R: %i,%i", playerTile.q, playerTile.r),
           TextFormat("State:  %s", player.PlayerStateToString()),
           TextFormat("Face Dir: %s", player.PlayerDirToString()),
           TextFormat("Frame: %i", player.GetAnimationFrame()),
           TextFormat("Type: %s", hexGrid.TileToString(tilePlayerType)),
           TextFormat("Speed[1/s]: %.2f", player.GetSpeedTilesPerSecond()),
       }});

  // --- Tool Bar ---
  debugData.push_back(
      {"Tool Bar",
       {
           TextFormat("Item: %s", itemHandler.GetSelectedItemType()),
           TextFormat("Slot: %i", itemHandler.GetSelectionToolBar()),
       }});

  // Draw section
  Vector2 playerScreenPos = GetWorldToScreen2D(playerPos, camera);
  DrawCircleV(
      GetWorldToScreen2D(hexGrid.HexCoordToPoint(HexCoord(0, 0)), camera), 3.0f,
      RED);
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

// --- Deinitialization ---
Game::~Game() {

  graphicsManager.UnloadAssets();
  fontHandler.UnloadFonts();
  CloseWindow(); // Close window and OpenGL context
}
