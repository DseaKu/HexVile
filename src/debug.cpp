#include "debug.h"
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
  DrawText(TextFormat("Mouse Position:\n  x:%i\n  y:%i", mouse_position.x,
                      mouse_position.y),
           100, 100, 10, RED);
}
