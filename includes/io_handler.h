#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "raylib.h"
#include "ui_handler.h"

class IO_Handler {
private:
  UI_Handler *ui_handler;
  Vector2 *mousePos;

public:
  IO_Handler();
  void InitIOHandler(Vector2 *mousePointer, UI_Handler *ui_handler_pointer);
};

#endif // !IO_HANDLER_H
