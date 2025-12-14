#include "io_handler.h"
#include "enums.h"
#include "raylib.h"

IO_Handler::IO_Handler() {}

void IO_Handler::Init() {}

void IO_Handler::UpdateMousePos(Camera2D cam) {
  realMousePos = GetMousePosition();
  scaledMousePos = GetScreenToWorld2D(realMousePos, cam);
}
// --- Conversion ---

// --- Get ---
Vector2 IO_Handler::GetRealMousePos() { return realMousePos; }

Vector2 IO_Handler::GetScaledMousePos() { return scaledMousePos; }

int IO_Handler::GetToolBarSelction(int curToolBarSel) {

  int toolBarSel = -1;

  if (IsKeyPressed(KEY_ONE)) {
    toolBarSel = 0;
  } else if (IsKeyPressed(KEY_TWO)) {
    toolBarSel = 1;
  } else if (IsKeyPressed(KEY_THREE)) {
    toolBarSel = 2;
  } else if (IsKeyPressed(KEY_FOUR)) {
    toolBarSel = 3;
  } else if (IsKeyPressed(KEY_FIVE)) {
    toolBarSel = 4;
  } else if (IsKeyPressed(KEY_SIX)) {
    toolBarSel = 5;
  } else if (IsKeyPressed(KEY_SEVEN)) {
    toolBarSel = 6;
  } else if (IsKeyPressed(KEY_EIGHT)) {
    toolBarSel = 7;
  } else if (IsKeyPressed(KEY_NINE)) {
    toolBarSel = 8;
  } else if (IsKeyPressed(KEY_ZERO)) {
    toolBarSel = 9;
  }

  if (toolBarSel != -1)
    return toolBarSel;
  return curToolBarSel;
}

MouseMask *IO_Handler::GetMouseMaskPointer() { return &mouseMask; }

// --- Set ---
