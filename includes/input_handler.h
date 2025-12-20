#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "defines.h"
#include "enums.h"
#include "raylib.h"

class InputHandler {
private:
  MouseMask mouseMask;
  Vector2 scaledMousePos;
  InputState *currentInput;

public:
  InputHandler();
  void Init();
  Vector2 GetMouseWorldPos();

  // --- Conversion ---
  const char *MouseMaskToString(MouseMask m);

  // --- Get ---
  int GetToolBarSelction(int curToolBarSel);
  MouseMask GetMouseMask();

  // --- Set ---
  void SetMouseMask(MouseMask mask);
  void SetInputState(InputState *InputState);
};

#endif // !INPUT_HANDLER_H
