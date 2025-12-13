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
  textureHandler.LoadAssets(Conf::TEXTURE_ATLAS_PATH);
  int fileSize = 0;
  hackFontRegular = LoadFileData(Conf::FONT_HACK_REGULAR_PATH, &fileSize);

  hexGrid.InitGrid(Conf::MAP_SIZE);
  hexGrid.SetTextureHandler(&textureHandler);

  player.SetHexGrid(&hexGrid);
  player.SetTextureHandler(&textureHandler);

  uiHandler.Init();
  uiHandler.SetTextureHandler(&textureHandler);
  uiHandler.SetItemBarStatus(true);

  camera.target = Conf::SCREEN_CENTER;
  camera.offset = Conf::SCREEN_CENTER;
  camera.zoom = Conf::CAMERA_ZOOM;
  camera.rotation = 0.0f;
  cameraRect = {0, 0, 0, 0};
  cameraTopLeft = {0, 0};

  mousePos = (Vector2){0, 0};
  mouseMask = MOUSE_MASK_NULL;

  fontHandler.LoadFonts();
}

// --- Main Loop ---
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // --- Update ---
    ProcessInputs();
    relativeCenter = GetScreenToWorld2D(Conf::SCREEN_CENTER, camera);
    cameraTopLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
    cameraRect = {cameraTopLeft.x, cameraTopLeft.y, Conf::CAMERA_WIDTH,
                  Conf::CAMERA_HEIGTH};

    player.Update();
    camera.target = player.GetPosition();

    // --- Draw ---
    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(WHITE);

    // --- Camera View ---
    hexGrid.Draw(camera);
    player.Draw();

    // --- End Camera View ---
    EndMode2D();

    uiHandler.DrawItemBar();
    DrawDebugOverlay(Conf::DEBUG_FLAG);

    EndDrawing();
  }
}

void Game::ProcessInputs() {

  mousePos = GetScreenToWorld2D(GetMousePosition(), this->camera);

  // --- Mouse ---
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (uiHandler.GetItemBarStatus() &&
        CheckCollisionPointRec(GetMousePosition(),
                               uiHandler.GetItemBarRect())) {
      mouseMask = MOUSE_MASK_ITEM_BAR;
    } else {
      mouseMask = MOUSE_MASK_PLAY_GROUND;
      HexCoord clickedHex = hexGrid.PointToHexCoord(this->mousePos);
      hexGrid.SetTile(clickedHex, TILE_WATER);
    }
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    HexCoord clickedHex = hexGrid.PointToHexCoord(this->mousePos);
    hexGrid.SetTile(clickedHex, TILE_VOID);
  }

  // --- Keyboard ---
  // Toolbar selection
  if (IsKeyPressed(KEY_ONE))
    uiHandler.SetSelectedItem(0);
  if (IsKeyPressed(KEY_TWO))
    uiHandler.SetSelectedItem(1);
  if (IsKeyPressed(KEY_THREE))
    uiHandler.SetSelectedItem(2);
  if (IsKeyPressed(KEY_FOUR))
    uiHandler.SetSelectedItem(3);
  if (IsKeyPressed(KEY_FIVE))
    uiHandler.SetSelectedItem(4);
  if (IsKeyPressed(KEY_SIX))
    uiHandler.SetSelectedItem(5);
  if (IsKeyPressed(KEY_SEVEN))
    uiHandler.SetSelectedItem(6);
  if (IsKeyPressed(KEY_EIGHT))
    uiHandler.SetSelectedItem(7);
  if (IsKeyPressed(KEY_NINE))
    uiHandler.SetSelectedItem(8);
  if (IsKeyPressed(KEY_ZERO))
    uiHandler.SetSelectedItem(9);
}

const char *Game::MouseMaskToString(MouseMask m) {
  switch (m) {
  case MOUSE_MASK_NULL:
    return "Null";
  case MOUSE_MASK_PLAY_GROUND:
    return "Play Ground";
  case MOUSE_MASK_ITEM_BAR:
    return "Item Bar";
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
           TextFormat("Used Tiles: %i", hexGrid.getTilesInUse()),
           TextFormat("Map radius: %i", hexGrid.getMapRadius()),
       }});

  HexCoord mapTile = hexGrid.PointToHexCoord(this->mousePos);
  TileID tileMouseType = hexGrid.PointToType(this->mousePos);
  debugData.push_back(
      {"Mouse",
       {
           TextFormat("X,Y: %.1f,%.1f", this->mousePos.x, this->mousePos.y),
           TextFormat("Tile Q,R: %i,%i", mapTile.q, mapTile.r),
           TextFormat("Type: %s", hexGrid.TileToString(tileMouseType)),
           TextFormat("Clicked on: %s",
                      this->MouseMaskToString(this->mouseMask)),
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
       }});

  // Draw section
  Vector2 playerScreenPos = GetWorldToScreen2D(playerPos, camera);
  DrawCircle(playerScreenPos.x, playerScreenPos.y, 3.0f, RED);
  for (const DebugData &data : debugData) {
    fontHandler.DrawTextHackRegular(data.section.c_str(),
                                    {sectionPosX, currentY}, sectionColor);
    currentY += sectionGapY;
    currentY += subSectionGapY;

    // Draw sub-section
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

  textureHandler.UnloadAssets();
  fontHandler.UnloadFonts();
  CloseWindow(); // Close window and OpenGL context
}
