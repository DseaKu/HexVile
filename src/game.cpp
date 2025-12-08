#include "game.h"
#include "defines.h"
#include "hex_tile_grid.h"
#include "raylib.h"

// Initialization
//--------------------------------------------------------------------------------------
Game::Game()
    : hexGrid(40.0f, Config::MAP_SIZE,
              {Config::SCREEN_CENTER.x, Config::SCREEN_CENTER.y}) {

  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
             "Dream of Hexagons - Interactive");
  SetTargetFPS(120);

  camera.target = Config::SCREEN_CENTER;
  camera.offset = Config::SCREEN_CENTER;
  camera.zoom = Config::ZOOM;
  camera.rotation = 0.0f;

  MousePos = (Vector2){0, 0};
}

// Main Loop
//--------------------------------------------------------------------------------------
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // Update
    //----------------------------------------------------------------------------------
    MousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Hexagon clickedHex = hexGrid.PointToHexagon(MousePos);
      hexGrid.ToggleTile(clickedHex);
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(WHITE);

    hexGrid.Draw();

    BeginMode2D(camera);

    EndMode2D();

    DrawDebugOverlay(Config::DEBUGGER_FLAG);

    EndDrawing();
  }
}

// De-Initialization
//--------------------------------------------------------------------------------------
Game::~Game() {

  CloseWindow(); // Close window and OpenGL context
}

// Draw debug Information
//--------------------------------------------------------------------------------------
void Game::DrawDebugOverlay(bool is_enabled) {
  if (!is_enabled)
    return;
  DrawFPS(100, 50);
  DrawCircleV(Config::SCREEN_CENTER, 5.0f, RED);

  // Draw mouse position
  Vector2 scaled_mouse_position = {
      (this->MousePos.x - (Config::SCREEN_WIDTH / 2.0f)) / Config::ZOOM,
      (this->MousePos.y - (Config::SCREEN_HEIGHT / 2.0f)) / Config::ZOOM};

  DrawText(TextFormat("Real Mouse Position:\n  x:%.2f\n  y:%.2f",
                      this->MousePos.x, this->MousePos.y),
           100, 100, 10, RED);

  DrawText(TextFormat("Scaled Mouse Position:\n  x:%.2f\n  y:%.2f",
                      scaled_mouse_position.x, scaled_mouse_position.y),
           100, 150, 10, RED);

  Vector2 tile00 = hexGrid.HexagonToPoint((Hexagon){0, 0});
  DrawText(TextFormat("Pos of qr:0,0=%.2f %.2f", tile00.x, tile00.y), 100, 200,
           10, RED);
};
