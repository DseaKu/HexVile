#include "ui_handler.h"
#include "defines.h"
#include "raylib.h"
UIHandler::UIHandler() { scale = Conf::UI_SCALE; }

void UIHandler::Init() {}
void UIHandler::Draw() {
  float width = Conf::UI_X_OFFSET * 2 * Conf::UI_WIDTH_SCALE;
  float posX = Conf::SCREEN_CENTER.x - (width / 2.0f);
  float posY = Conf::SCREEN_HEIGHT - Conf::UI_Y_OFFSET;
  DrawRectangle(posX, posY, width, 600, GRAY);
}
