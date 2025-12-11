#include "game.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include <string>

// --- Initialization ---
Game::Game() {
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
             "Dream of HexCoords - Interactive");
  // SetTargetFPS(120);
  textureHandler.LoadAssets("assets/texture_atlas.png");

  hexGrid.InitGrid(12.0f);
  hexGrid.GetTextureHandler(&textureHandler);

  player.GetTextureHandler(&textureHandler);

  camera.target = Config::SCREEN_CENTER;
  camera.offset = Config::SCREEN_CENTER;
  camera.zoom = Config::CAMERA_ZOOM;
  camera.rotation = 0.0f;
  cameraRect = {0, 0, 0, 0};
  cameraTopLeft = {0, 0};

  MousePos = (Vector2){0, 0};
}

// --- Main Loop ---
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // --- Update ---
    this->MousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    this->relativeCenter = GetScreenToWorld2D(Config::SCREEN_CENTER, camera);
    this->cameraTopLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
    this->cameraRect = {cameraTopLeft.x, cameraTopLeft.y, Config::CAMERA_WIDTH,
                        Config::CAMERA_HEIGTH};

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

    EndMode2D();
    DrawDebugOverlay(Config::DEBUG_FLAG);
    EndDrawing();
  }
}

// --- Deinitialization ---
Game::~Game() {

  textureHandler.UnloadAssets();
  CloseWindow(); // Close window and OpenGL context
}

// --- Debug overlay ---
void Game::DrawDebugOverlay(bool is_enabled) {
  if (!is_enabled)
    return;

  int sectionPosX = Config::DEBUG_OVERLAY_SECTION_X_POS;
  int sectionPosY = Config::DEBUG_OVERLAY_SECTION_Y_POS;
  int sectionGapY = Config::DEBUG_OVERLAY_SECTION_Y_GAP;
  int sectionFontSize = Config::DEBUG_OVERLAY_SECTION_FONT_SIZE;
  Color sectionColor = RED;

  int subSectionPosX = Config::DEBUG_OVERLAY_SUBSECTION_X_POS;
  int subSectionGapY = Config::DEBUG_OVERLAY_SUBSECTION_Y_GAP;
  int subSectionFontSize = Config::DEBUG_OVERLAY_SUBSECTION_FONT_SIZE;
  Color subSectionColor = RED;

  int currentY = sectionPosY;

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
           TextFormat("Hoverd Tile Q,R: %i,%i", mapTile.q, mapTile.r),
       }});

  // --- Player ---
  Vector2 playerPos = player.GetPosition();
  HexCoord playerTile = hexGrid.PointToHexCoord(playerPos);
  const char *playerState = player.PlayerStateToString();
  const char *playerDir = player.PlayerDirToString();
  debugData.push_back(
      {"Player",
       {
           TextFormat("X,Y: %.1f,%.1f", playerPos.x, playerPos.y),
           TextFormat("Tile Q,R: %i,%i", playerTile.q, playerTile.r),
           TextFormat("Player State = %s", playerState),
           TextFormat("Player Face Dir = %s", playerDir),
       }});

  // Draw section
  Vector2 playerScreenPos = GetWorldToScreen2D(playerPos, camera);
  DrawCircle(playerScreenPos.x, playerScreenPos.y, 3.0f, RED);
  for (const auto &data : debugData) {
    DrawText(data.section.c_str(), sectionPosX, currentY, sectionFontSize,
             sectionColor);
    currentY += sectionGapY;
    currentY += subSectionGapY;

    // Draw sub-section
    for (const std::string &subSection : data.subSection) {
      DrawText(subSection.c_str(), subSectionPosX, currentY, subSectionFontSize,
               subSectionColor);
      currentY += subSectionGapY;
    }
    currentY += sectionGapY;
  }
};
