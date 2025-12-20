#include "input_handler.h"
#include "defines.h"
#include "enums.h"

InputHandler::InputHandler() {
  currentInput = nullptr;
  scaledMousePos = {0.0f, 0.0f};
}

void InputHandler::Init() {}

Vector2 InputHandler::GetMouseWorldPos() { return currentInput->mouseWorldPos; }

// --- Conversion ---

// --- Get ---

int InputHandler::GetToolBarSelction(int curToolBarSel) {

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

MouseMask InputHandler::GetMouseMask() { return mouseMask; }

// --- Set ---
void InputHandler::SetMouseMask(MouseMask mask) { mouseMask = mask; }

void InputHandler::SetInputState(InputState *inputState) {
  this->currentInput = inputState;
}
