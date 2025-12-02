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
  Vector2 mouse_position = GetMousePosition();
  DrawText(TextFormat("Real Mouse Position:\n  x:%.2f\n  y:%.2f",
                      mouse_position.x, mouse_position.y),
           100, 100, 10, RED);
  DrawText(TextFormat("Scaled Mouse Position:\n  x:%.2f\n  y:%.2f",
                      mouse_position.x * Config::SCALE,
                      mouse_position.y * Config::SCALE),
           100, 150, 10, RED);
  DrawCircleV((Vector2){0.0f, 0.0f}, 5.0f, RED);
}
