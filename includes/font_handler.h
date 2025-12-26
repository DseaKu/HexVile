#ifndef FONT_HANDLER_H
#define FONT_HANDLER_H
#include "GFX_manager.h"
#include "enums.h"
#include "raylib.h"

class FontHandler {
private:
  Font fontHackRegular;
  int fontSizeDefault;

public:
  FontHandler();
  void LoadFonts();
  void UnloadFonts();
  Font GetFontHackRegular();
  int GetFontSizeDefault();
  void DrawTextHackRegular(const char *text, Vector2 position, Color color);
  void QueueText(GFX_Manager *gfx, const char *text, Vector2 pos, Color col,
                 drawMask::id layer = drawMask::DEBUG_OVERLAY);
};
#endif // !FONT_HANDLER_H
