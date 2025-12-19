#include "io_handler.h"
#include "enums.h"

IO_Handler::IO_Handler() { scaledMousePos = {0.0f, 0.0f}; }

void IO_Handler::Init() {}

void IO_Handler::UpdateMousePos(Vector2 pos) { scaledMousePos = pos; }

Vector2 IO_Handler::GetScaledMousePos() { return scaledMousePos; }

// --- Conversion ---

// --- Get ---

int IO_Handler::GetToolBarSelction(int curToolBarSel,
                                   KeyboardInputState keyboardState) {

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
