#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "enums.h"
#include "raylib.h"
struct KeyboardInputState {
  bool keyOne;
  bool keyTwo;
  bool keyThree;
  bool keyFour;
  bool keyFive;
  bool keySix;
  bool keySeven;
  bool keyEight;
  bool keyNine;
  bool keyZero;
};

class IO_Handler {
private:
  MouseMask mouseMask;
  Vector2 scaledMousePos;

public:
  IO_Handler();
  void Init();
  void UpdateMousePos(Vector2 pos);
  Vector2 GetScaledMousePos();

  // --- Conversion ---
  const char *MouseMaskToString(MouseMask m);

  // --- Get ---
  int GetToolBarSelction(int curToolBarSel, KeyboardInputState keyboardState);
  MouseMask GetMouseMask();

  // --- Set ---
  void SetMouseMask(MouseMask mask);
};

#endif // !IO_HANDLER_H
