#ifndef FONT_HANDLER_H
#define FONT_HANDLER_H
#include "GFX_manager.h"
#include "enums.h"
#include "raylib.h"

class FontHandler {
private:
  // --- Members ---
  Font fontHackRegular;
  int fontSizeDefault;

public:
  // --- Constructors ---
  FontHandler();

  // --- Core Lifecycle ---
  void LoadFonts();
  void UnloadFonts();

  // --- Graphics / Backbuffer ---
  void DrawTextHackRegular(const char *text, Vector2 position, Color color);
  void QueueText(GFX_Manager *gfx, const char *text, Vector2 pos, Color col,
                 drawMask::id layer = drawMask::DEBUG_OVERLAY);

  // --- Getters ---
  Font GetFontHackRegular();
  int GetFontSizeDefault();
};
#endif // !FONT_HANDLER_H
