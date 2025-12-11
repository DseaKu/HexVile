#ifndef FONT_HANDLER_H
#define FONT_HANDLER_H
#include "raylib.h"

class FontHandler {
private:
  Font fontHackRegular;
  int fontSizeDefault;

public:
  FontHandler();
  void InitFonts();
  void DeInitFonts();
  Font getFontHackRegular();
  int getFontSizeDefault();
};

#endif // !FONT_HANDLER_H
