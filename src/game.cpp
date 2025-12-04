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
  Camera.target = Config::SCREEN_CENTER;
  Camera.offset = Config::SCREEN_CENTER;
  Camera.zoom = Config::ZOOM;
  TileMap.SetTileMap("assets/images/Tileset1.png");
}
void Game::GameLoop() {
  while (!WindowShouldClose()) {
    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    Debugger.SetDebugger(Config::DEBUGGER_FLAG);
    selected_tile = TileMap.MouseToHexCoords();

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(Camera);
    TileMap.DrawTile(IDK, 0, -2);
    TileMap.DrawTile(MISC, -1, -1);
    TileMap.DrawTile(MISC, 0, -1);
    TileMap.DrawTile(MISC, 1, -1);
    TileMap.DrawTile(DIRT, 0, 0);
    TileMap.DrawTile(WATER, 1, 0);
    TileMap.DrawTile(VOID, 2, 0);
    TileMap.DrawTile(ICE, -3, 0);
    TileMap.DrawTile(ICE, 3, 0);
    TileMap.DrawTile(MISC, 1, 1);
    TileMap.DrawTile(IDK, 0, 2);
    TileMap.DrawTile(GRASS, 1, 2);
    TileMap.DrawTile(GRASS, 0, 3);
    TileMap.DrawTile(GRASS, 1, 3);
    TileMap.DrawTile(GRASS, 0, 4);
    TileMap.DrawTile(GRASS, 1, 4);
    EndMode2D();

    Debugger.DrawDebugInformation(selected_tile);
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
