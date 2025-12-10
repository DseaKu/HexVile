#include "game.h"
#include "defines.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include <string>

// --- Initialization ---
Game::Game() {
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
             "Dream of HexCoords - Interactive");
  // SetTargetFPS(120);
  textureHandler.LoadAssets("assets/images/Tileset4.png");

  hexGrid.InitGrid(12.0f);
  hexGrid.GetTextureHandler(&textureHandler);

  player.GetTextureHandler(&textureHandler);

  camera.target = Config::SCREEN_CENTER;
  camera.offset = Config::SCREEN_CENTER;
  camera.zoom = Config::CAMERA_ZOOM;
  camera.rotation = 0.0f;

  MousePos = (Vector2){0, 0};
}

// --- Main Loop ---
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // --- Update ---
    this->MousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    this->relativeCenter = GetScreenToWorld2D(Config::SCREEN_CENTER, camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      HexCoord clickedHex = hexGrid.PointToHexCoord(this->MousePos);
      hexGrid.ToggleTile(clickedHex);
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
  int subSectionPosY = Config::DEBUG_OVERLAY_SUBSECTION_Y_POS;
  int subSectionGapY = Config::DEBUG_OVERLAY_SUBSECTION_Y_GAP;
  int subSectionFontSize = Config::DEBUG_OVERLAY_SUBSECTION_FONT_SIZE;
  Color subSectionColor = RED;

  debugData.push_back({"Resources",
                       {
                           "FPS: " + std::to_string(GetFPS()),
                       }});

  debugData.push_back(
      {"Mouse",
       {
           "Real Mouse Position: x" + std::to_string(this->MousePos.x),
       }});

  // Draw section
  for (const auto &data : debugData) {
    DrawText(data.section.c_str(), sectionPosX, sectionPosY, sectionFontSize,
             sectionColor);
    sectionPosY += sectionGapY;
    ;

    // Draw sub-section
    for (const std::string &subSection : data.subSection) {
      DrawText(subSection.c_str(), subSectionPosX, subSectionPosY,
               subSectionFontSize, subSectionColor);
    }
  }
  // // --- Mouse ---
  // Vector2 scaled_mouse_position = {
  //     (this->MousePos.x - (Config::SCREEN_WIDTH / 2.0f)) /
  //     Config::CAMERA_ZOOM, (this->MousePos.y - (Config::SCREEN_HEIGHT
  //     / 2.0f)) /
  //         Config::CAMERA_ZOOM};
  //
  // DrawText(TextFormat("Real Mouse Position:\n  x:%.2f\n  y:%.2f",
  //                     this->MousePos.x, this->MousePos.y),
  //          xOffset, 100, 10, RED);
  //
  // DrawText(TextFormat("Scaled Mouse Position:\n  x:%.2f\n  y:%.2f",
  //                     scaled_mouse_position.x, scaled_mouse_position.y),
  //          xOffset, 140, 10, RED);
  //
  // HexCoord selected_tile = hexGrid.PointToHexCoord(this->MousePos);
  // DrawText(TextFormat("Hex Coordinate :\n  x:%i\n  y:%i", selected_tile.q,
  //                     selected_tile.r),
  //          xOffset, 180, 10, RED);
  //
  // Vector2 tile00 = hexGrid.HexCoordToPoint((HexCoord){0, 0});
  // DrawText(TextFormat("Map Tile{0,0} = %.2f %.2f", tile00.x, tile00.y),
  // xOffset,
  //          250, 10, RED);
  //
  // // --- Player ---
  // Vector2 player_pos = player.GetPosition();
  // DrawText(
  //     TextFormat("Player Position = %.2f %.2f", player_pos.x, player_pos.y),
  //     xOffset, 300, 10, RED);
  //
  // const char *playerState = player.PlayerStateToString();
  // DrawText(TextFormat("Player State = %s", playerState), xOffset, 320, 10,
  // RED);
};
