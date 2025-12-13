#include "io_handler.h"
#include "raylib.h"
#include "ui_handler.h"

IO_Handler::IO_Handler() {}

void IO_Handler::InitIOHandler(Vector2 *mousePointer,
                               UI_Handler *ui_handler_pointer) {
  ui_handler = ui_handler_pointer;
  mousePos = mousePointer;
}
