#include "game.h"
#include "debug.h"
#include "defines.h"
#include "raylib.h"

// Initialization
//--------------------------------------------------------------------------------------
Game::Game()
    : hexGrid(35.0f, 5, {Config::SCREEN_CENTER.x, Config::SCREEN_CENTER.y}) {

  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
             "Dream of Hexagons - Interactive");
  SetTargetFPS(120);

  Camera.target = Config::SCREEN_CENTER;
  Camera.offset = Config::SCREEN_CENTER;
  Camera.zoom = Config::ZOOM;
  Camera.rotation = 0.0f;

  TileMap.LoadTileMapAsset("assets/images/Tileset1.png");
  TileMap.SetHighlightedTilePointer(&HighlightedTile);
}

// Main Loop
//--------------------------------------------------------------------------------------
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // Update
    //----------------------------------------------------------------------------------
    Debugger.SetDebugger(Config::DEBUGGER_FLAG);
    HighlightedTile = TileMap.PosToHexCoords(GetMousePosition());

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      HexTile clickedHex = hexGrid.PixelToHexTile(mousePos);
      hexGrid.ToggleTile(clickedHex);
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(BLACK);

    hexGrid.Draw();
    hexGrid.DrawDebugOverlay(mousePos);

    BeginMode2D(Camera);

    EndMode2D();

    Debugger.DrawDebugInformation(HighlightedTile);
    EndDrawing();
  }
}

// De-Initialization
//--------------------------------------------------------------------------------------
Game::~Game() {

  CloseWindow(); // Close window and OpenGL context
  TileMap.UnloadTileMapAsset();
}
