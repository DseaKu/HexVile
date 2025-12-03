#include "debug.h"
#include "defines.h"
#include "raylib.h"

void Debugger::SetDebugger(bool enable_flag) {
  if (enable_flag) {
    is_enabled = true;
  } else {
    is_enabled = false;
  }
};

void Debugger::DrawDebugInformation(HexCoords selected_tile) {

  if (!is_enabled)
    return;

  Vector2 mouse_position = GetMousePosition();

  DrawFPS(100, 50);

  DrawText(TextFormat("Real Mouse Position:\n  x:%.2f\n  y:%.2f",
                      mouse_position.x, mouse_position.y),
           100, 100, 10, RED);

  // Calculate mouse position relative to the center of the screen
  Vector2 scaled_mouse_position = {
      (mouse_position.x - (Config::SCREEN_WIDTH / 2.0f)) / Config::ZOOM,
      (mouse_position.y - (Config::SCREEN_HEIGHT / 2.0f)) / Config::ZOOM};

  DrawText(TextFormat("Scaled Mouse Position:\n  x:%.2f\n  y:%.2f",
                      scaled_mouse_position.x, scaled_mouse_position.y),
           100, 150, 10, RED);

  DrawText(TextFormat("Selected Tile:\n  q:%i\n  r:%i", selected_tile.q,
                      selected_tile.r),
           100, 200, 10, RED);

  DrawCircleV(Config::SCREEN_CENTER, 5.0f, RED);
}
