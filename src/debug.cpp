#include "debug.h"
#include "defines.h"
#include "raylib.h"
Debugger::Debugger(bool enable_flag) {
  if (enable_flag) {
    is_enabled = true;
  } else {
    is_enabled = false;
  }
}

Debugger::~Debugger() {}

void Debugger::DrawDebugInformation() {
  float screen_width = GetScreenWidth();
  float screen_height = GetScreenHeight();
  Vector2 mouse_position = GetMousePosition();

  // Calculate mouse position relative to the center of the screen
  Vector2 game_mouse_position = {mouse_position.x - (screen_width / 2.0f),
                                 mouse_position.y - (screen_height / 2.0f)};

  DrawText(TextFormat("Real Mouse Position:\n  x:%.2f\n  y:%.2f",
                      mouse_position.x, mouse_position.y),
           100, 100, 10, RED);
  DrawText(TextFormat("Scaled Mouse Position:\n  x:%.2f\n  y:%.2f",
                      game_mouse_position.x * Config::SCALE,
                      game_mouse_position.y * Config::SCALE),
           100, 150, 10, RED);
  DrawText(TextFormat("Selected Hex-Tile:\n  x:%i\n  y:%i",
                      game_mouse_position.x * Config::SCALE,
                      game_mouse_position.y * Config::SCALE),
           100, 200, 10, RED);
  DrawCircleV((Vector2){screen_width / 2.0f, screen_height / 2.0f}, 5.0f, RED);
}
