#include "game.h"
#include "debug.h"
#include "defines.h"
#include "enums.h"

Game::Game() {
  //----------------------------------------------------------------------------------
  // Initialization
  //--------------------------------------------------------------------------------------
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
             "Dream of Hexagons - Interactive");

  SetTargetFPS(120);
  HexTiles tiles("assets/images/Tileset1.png");
  camera.target = Config::SCREEN_CENTER;
  camera.offset = Config::SCREEN_CENTER;
  camera.zoom = Config::ZOOM;
}
void Game::GameLoop() {
  // Main game loop
  while (!WindowShouldClose()) {
    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    debugger.SetDebugger(Config::DEBUGGER_FLAG);

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    tiles.DrawTile(IDK, 0, -2);
    tiles.DrawTile(MISC, -1, -1);
    tiles.DrawTile(MISC, 0, -1);
    tiles.DrawTile(MISC, 1, -1);
    tiles.DrawTile(DIRT, 0, 0);
    tiles.DrawTile(WATER, 1, 0);
    tiles.DrawTile(VOID, 2, 0);
    tiles.DrawTile(ICE, -3, 0);
    tiles.DrawTile(ICE, 3, 0);
    tiles.DrawTile(MISC, 1, 1);
    tiles.DrawTile(IDK, 0, 2);
    tiles.DrawTile(GRASS, 1, 2);
    tiles.DrawTile(GRASS, 0, 3);
    tiles.DrawTile(GRASS, 1, 3);
    tiles.DrawTile(GRASS, 0, 4);
    tiles.DrawTile(GRASS, 1, 4);
    EndMode2D();

    debugger.DrawDebugInformation();
    EndDrawing();
  }
}

Game::~Game() {

  //----------------------------------------------------------------------------------
  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------
}
