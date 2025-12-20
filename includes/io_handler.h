#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "defines.h"
#include "enums.h"
#include "raylib.h"

class IO_Handler {
private:
  MouseMask mouseMask;
  Vector2 scaledMousePos;
  InputState *currentInput;

public:
  IO_Handler();
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

#endif // !IO_HANDLER_H
