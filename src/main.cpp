#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "tile_map.h"

int main(void) {
  //----------------------------------------------------------------------------------
  // Initialization
  //--------------------------------------------------------------------------------------
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
             "Dream of Hexagons - Interactive");

  SetTargetFPS(120);
  TileMap tileMap("assets/images/Tileset1.png");

  // Main game loop
  while (!WindowShouldClose()) {
    //----------------------------------------------------------------------------------
    // Update
    //----------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);
    tileMap.DrawTile(DIRT);

    EndDrawing();
  }

  //----------------------------------------------------------------------------------
  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
