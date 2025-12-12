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
  this->textureHandler.LoadAssets(Conf::TEXTURE_ATLAS_PATH);
  int fileSize = 0;
  this->hackFontRegular = LoadFileData(Conf::FONT_HACK_REGULAR_PATH, &fileSize);

  this->hexGrid.InitGrid(Conf::MAP_SIZE);
  this->hexGrid.SetTextureHandler(&textureHandler);

  this->player.SetHexGrid(&hexGrid);
  this->player.SetTextureHandler(&textureHandler);

  this->camera.target = Conf::SCREEN_CENTER;
  this->camera.offset = Conf::SCREEN_CENTER;
  this->camera.zoom = Conf::CAMERA_ZOOM;
  this->camera.rotation = 0.0f;
  this->cameraRect = {0, 0, 0, 0};
  this->cameraTopLeft = {0, 0};

  this->MousePos = (Vector2){0, 0};

  this->fontHandler.LoadFonts();
}

// --- Main Loop ---
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // --- Update ---
    this->MousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    this->relativeCenter = GetScreenToWorld2D(Conf::SCREEN_CENTER, camera);
    this->cameraTopLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
    this->cameraRect = {cameraTopLeft.x, cameraTopLeft.y, Conf::CAMERA_WIDTH,
                        Conf::CAMERA_HEIGTH};

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      HexCoord clickedHex = hexGrid.PointToHexCoord(this->MousePos);
      hexGrid.SetTile(clickedHex, TILE_WATER);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      HexCoord clickedHex = hexGrid.PointToHexCoord(this->MousePos);
      hexGrid.SetTile(clickedHex, TILE_VOID);
    }
    player.Update();
    camera.target = player.GetPosition();

    // --- Draw ---
    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(WHITE);

    // --- Camera View ---
    hexGrid.Draw(camera);
    player.Draw();

    // --- Test Font ---

    // --- Camera View ---

    EndMode2D();
    DrawDebugOverlay(Conf::DEBUG_FLAG);
    EndDrawing();
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
  debugData.push_back({"Resources",
                       {
                           TextFormat("FPS: %i", GetFPS()),
                       }});

  HexCoord mapTile = hexGrid.PointToHexCoord(this->MousePos);
  debugData.push_back(
      {"Mouse",
       {
           TextFormat("X,Y: %.1f,%.1f", this->MousePos.x, this->MousePos.y),
           TextFormat("Tile Q,R: %i,%i", mapTile.q, mapTile.r),
       }});

  // --- Player ---
  Vector2 playerPos = player.GetPosition();
  HexCoord playerTile = hexGrid.PointToHexCoord(playerPos);
  debugData.push_back(
      {"Player",
       {
           TextFormat("X,Y: %.1f,%.1f", playerPos.x, playerPos.y),
           TextFormat("Tile Q,R: %i,%i", playerTile.q, playerTile.r),
           TextFormat("State:  %s", player.PlayerStateToString()),
           TextFormat("Face Dir: %s", player.PlayerDirToString()),
           TextFormat("Frame: %i", player.GetAnimationFrame()),
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
