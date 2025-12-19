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

int IO_Handler::GetToolBarSelction(int curToolBarSel, KeyboardInputState keyboardState) {

  int toolBarSel = -1;

  if (keyboardState.keyOne) {
    toolBarSel = 0;
  } else if (keyboardState.keyTwo) {
    toolBarSel = 1;
  } else if (keyboardState.keyThree) {
    toolBarSel = 2;
  } else if (keyboardState.keyFour) {
    toolBarSel = 3;
  } else if (keyboardState.keyFive) {
    toolBarSel = 4;
  } else if (keyboardState.keySix) {
    toolBarSel = 5;
  } else if (keyboardState.keySeven) {
    toolBarSel = 6;
  } else if (keyboardState.keyEight) {
    toolBarSel = 7;
  } else if (keyboardState.keyNine) {
    toolBarSel = 8;
  } else if (keyboardState.keyZero) {
    toolBarSel = 9;
  }

  if (toolBarSel != -1)
    return toolBarSel;
  return curToolBarSel;
}

MouseMask IO_Handler::GetMouseMask() { return mouseMask; }

// --- Set ---
void IO_Handler::SetMouseMask(MouseMask mask) { mouseMask = mask; }
void IO_Handler::SetScaledMousePos(Vector2 pos) { scaledMousePos = pos; }
void IO_Handler::SetRealMousePos(Vector2 pos) { realMousePos = pos; }
