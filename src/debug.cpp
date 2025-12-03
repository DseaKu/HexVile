#include "debug.h"
#include "defines.h"
#include "raylib.h"
#include <cmath>

void Debugger::SetDebugger(bool enable_flag) {
  if (enable_flag) {
    is_enabled = true;
  } else {
    is_enabled = false;
  }
};

void Debugger::DrawDebugInformation() {
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

  int hex_row = roundf(scaled_mouse_position.y / Config::TILE_SIZE_HALF);
  float col_unadjusted = scaled_mouse_position.x / Config::TILE_SIZE;
  int hex_col;

  if (hex_row & 1) { // Odd row
    hex_col = roundf(col_unadjusted + 0.5f);
  } else { // Even row
    hex_col = roundf(col_unadjusted);
  }

  DrawText(TextFormat("Selected Hex-Tile:\n  x:%i\n  y:%i", hex_col, hex_row),
           100, 200, 10, RED);

  DrawCircleV(Config::SCREEN_CENTER, 5.0f, RED);
}
