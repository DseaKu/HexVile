#include "game.h"
#include "debug.h"
#include "defines.h"
#include "enums.h"

// Initialization
//--------------------------------------------------------------------------------------
Game::Game() {
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

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(Camera);
    TileMap.DrawTile(DIRT, 0, 0);
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

    Debugger.DrawDebugInformation(HighlightedTile);
    EndDrawing();
  }
}

// De-Initialization
//--------------------------------------------------------------------------------------
Game::~Game() {

  CloseWindow(); // Close window and OpenGL context
}
