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
  Font GetFontHackRegular();
  int GetFontSizeDefault();
  void DrawTextHackRegular(const char *text, Vector2 position, Color color);
};

#endif // !FONT_HANDLER_H
