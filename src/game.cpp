#include "game.h"
#include "defines.h"
#include "hex_tile_grid.h"
#include "raylib.h"

// Initialization
//--------------------------------------------------------------------------------------
Game::Game() {
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
             "Dream of HexCoords - Interactive");
  // SetTargetFPS(120);

  hexGrid.InitGrid(12.0f);
  hexGrid.LoadAssets("assets/images/Tileset3.png");

  player.Init(Config::SCREEN_CENTER, "assets/images/player/PlayerWalk.png");

  camera.target = Config::SCREEN_CENTER;
  camera.offset = Config::SCREEN_CENTER;
  camera.zoom = Config::CAMERA_ZOOM;
  camera.rotation = 0.0f;

  MousePos = (Vector2){0, 0};
}

// Main Loop
//--------------------------------------------------------------------------------------
void Game::GameLoop() {
  while (!WindowShouldClose()) {

    // Update
    //----------------------------------------------------------------------------------
    this->MousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    this->relativeCenter = GetScreenToWorld2D(Config::SCREEN_CENTER, camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      HexCoord clickedHex = hexGrid.PointToHexCoord(this->MousePos);
      hexGrid.ToggleTile(clickedHex);
    }
    player.Update();
    camera.target = player.GetPosition();

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(WHITE);

    hexGrid.Draw(camera);
    player.Draw();

    EndMode2D();
    DrawDebugOverlay(Config::DEBUGGER_FLAG);
    EndDrawing();
  }
}

// De-Initialization
//--------------------------------------------------------------------------------------
Game::~Game() {

  hexGrid.UnloadAssets();
  CloseWindow(); // Close window and OpenGL context
}

// Draw debug Information
//--------------------------------------------------------------------------------------
void Game::DrawDebugOverlay(bool is_enabled) {
  if (!is_enabled)
    return;
  DrawFPS(100, 50);
  // DrawCircleV(Config::SCREEN_CENTER, 5.0f, RED);

  // Draw mouse position
  Vector2 scaled_mouse_position = {
      (this->MousePos.x - (Config::SCREEN_WIDTH / 2.0f)) / Config::CAMERA_ZOOM,
      (this->MousePos.y - (Config::SCREEN_HEIGHT / 2.0f)) /
          Config::CAMERA_ZOOM};

  DrawText(TextFormat("Real Mouse Position:\n  x:%.2f\n  y:%.2f",
                      this->MousePos.x, this->MousePos.y),
           100, 100, 10, RED);

  DrawText(TextFormat("Scaled Mouse Position:\n  x:%.2f\n  y:%.2f",
                      scaled_mouse_position.x, scaled_mouse_position.y),
           100, 150, 10, RED);

  HexCoord selected_tile = hexGrid.PointToHexCoord(this->MousePos);
  DrawText(TextFormat("Hex Coordinate :\n  x:%i\n  y:%i", selected_tile.q,
                      selected_tile.r),
           100, 200, 10, RED);

  Vector2 tile00 = hexGrid.HexCoordToPoint((HexCoord){0, 0});
  DrawText(TextFormat("Map Tile{0,0} = %.2f %.2f", tile00.x, tile00.y), 100,
           250, 10, RED);

  Vector2 player_pos = player.GetPosition();
  DrawText(
      TextFormat("Player Position = %.2f %.2f", player_pos.x, player_pos.y),
      100, 300, 10, RED);
};
