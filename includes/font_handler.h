#ifndef FONT_HANDLER_H
#define FONT_HANDLER_H
#include "raylib.h"

class FontHandler {
private:
  Font fontDefault;
  int fontSizeDefault;
  Font fontSDF;
  Shader shader;

public:
  FontHandler();
  void InitFonts();
  void DeInitFonts();
  Font getFontDefault();
  int getFontSizeDefault();
};

#endif // !FONT_HANDLER_H
