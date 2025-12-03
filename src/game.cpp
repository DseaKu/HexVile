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
  camera.target = Config::SCREEN_CENTER;
  camera.offset = Config::SCREEN_CENTER;
  camera.zoom = Config::ZOOM;
  tile_map.SetTileMap("assets/images/Tileset1.png");
}
void Game::GameLoop() {
  while (!WindowShouldClose()) {
    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------
    debugger.SetDebugger(Config::DEBUGGER_FLAG);
    selected_tile = tile_map.MouseToHexCoords();

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    tile_map.DrawTile(IDK, 0, -2);
    tile_map.DrawTile(MISC, -1, -1);
    tile_map.DrawTile(MISC, 0, -1);
    tile_map.DrawTile(MISC, 1, -1);
    tile_map.DrawTile(DIRT, 0, 0);
    tile_map.DrawTile(WATER, 1, 0);
    tile_map.DrawTile(VOID, 2, 0);
    tile_map.DrawTile(ICE, -3, 0);
    tile_map.DrawTile(ICE, 3, 0);
    tile_map.DrawTile(MISC, 1, 1);
    tile_map.DrawTile(IDK, 0, 2);
    tile_map.DrawTile(GRASS, 1, 2);
    tile_map.DrawTile(GRASS, 0, 3);
    tile_map.DrawTile(GRASS, 1, 3);
    tile_map.DrawTile(GRASS, 0, 4);
    tile_map.DrawTile(GRASS, 1, 4);
    EndMode2D();

    debugger.DrawDebugInformation(selected_tile);
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
