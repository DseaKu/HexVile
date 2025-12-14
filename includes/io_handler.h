#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "enums.h"
class IO_Handler {
private:
  MouseMask mouseMask;

public:
  IO_Handler();
  void Init();
  int GetToolBarSelction(int curToolBarSel);
  const char *MouseMaskToString(MouseMask m);
};

#endif // !IO_HANDLER_H
