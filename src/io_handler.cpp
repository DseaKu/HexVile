#include "io_handler.h"
#include "defines.h"
#include "enums.h"

IO_Handler::IO_Handler() {
  currentInput = nullptr;
  scaledMousePos = {0.0f, 0.0f};
}

void IO_Handler::Init() {}

Vector2 IO_Handler::GetMouseWorldPos() { return currentInput->mouseWorldPos; }

// --- Conversion ---

// --- Get ---

int IO_Handler::GetToolBarSelction(int curToolBarSel) {

  int toolBarSel = -1;

  if (currentInput->keyboardInput.keyOne) {
    toolBarSel = 0;
  } else if (currentInput->keyboardInput.keyTwo) {
    toolBarSel = 1;
  } else if (currentInput->keyboardInput.keyThree) {
    toolBarSel = 2;
  } else if (currentInput->keyboardInput.keyFour) {
    toolBarSel = 3;
  } else if (currentInput->keyboardInput.keyFive) {
    toolBarSel = 4;
  } else if (currentInput->keyboardInput.keySix) {
    toolBarSel = 5;
  } else if (currentInput->keyboardInput.keySeven) {
    toolBarSel = 6;
  } else if (currentInput->keyboardInput.keyEight) {
    toolBarSel = 7;
  } else if (currentInput->keyboardInput.keyNine) {
    toolBarSel = 8;
  } else if (currentInput->keyboardInput.keyZero) {
    toolBarSel = 9;
  }

  if (toolBarSel != -1)
    return toolBarSel;
  return curToolBarSel;
}

MouseMask IO_Handler::GetMouseMask() { return mouseMask; }

// --- Set ---
void IO_Handler::SetMouseMask(MouseMask mask) { mouseMask = mask; }

void IO_Handler::SetInputState(InputState *inputState) {
  this->currentInput = inputState;
}
