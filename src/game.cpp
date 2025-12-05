#include "game.h"
#include "debug.h"
#include "defines.h"
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
      HexTile clickedHex = hexGrid.PixelToHexTile(MousePos);
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
  TileMap.UnloadTileMapAsset();
}

// Draw debug Information
//--------------------------------------------------------------------------------------
void Game::DrawDebugOverlay(bool is_enabled) {
  if (!is_enabled)
    return;
  DrawFPS(100, 50);
  hexGrid.DrawDebugOverlay(MousePos);
};
