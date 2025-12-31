#include "font_handler.h"
#include "defines.h"
#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#endif

// --- Constructors ---
FontHandler::FontHandler() {

  this->fontSizeDefault = conf::DEFAULT_FONT_SIZE;
  this->fontHackRegular = {0};
}

// --- Core Lifecycle ---
void FontHandler::LoadFonts() {
  int fileSize = 0;

  // --- Hack Font Regular ---
  unsigned char *fileData =
      LoadFileData(conf::FONT_HACK_REGULAR_PATH, &fileSize);
  this->fontHackRegular.baseSize = this->fontSizeDefault;
  this->fontHackRegular.glyphCount = conf::HACK_REGULAR_GLYPH_COUNT;
  this->fontHackRegular.glyphs =
      LoadFontData(fileData, fileSize, conf::DEFAULT_FONT_SIZE, 0,
                   conf::HACK_REGULAR_GLYPH_COUNT, FONT_DEFAULT,
                   &fontHackRegular.glyphCount);
  Image atlas = GenImageFontAtlas(fontHackRegular.glyphs, &fontHackRegular.recs,
                                  conf::HACK_REGULAR_GLYPH_COUNT,
                                  this->fontHackRegular.baseSize, 0, 0);
  this->fontHackRegular.texture = LoadTextureFromImage(atlas);
  UnloadImage(atlas);
}

void FontHandler::UnloadFonts() { UnloadFont(fontHackRegular); }

// --- Graphics / Backbuffer ---
void FontHandler::DrawTextHackRegular(const char *text, Vector2 pos,
                                      Color color) {
  DrawTextEx(this->fontHackRegular, text, pos, conf::DEFAULT_FONT_SIZE,
             conf::DEFAULT_FONT_SPACING, color);
}

void FontHandler::QueueText(GFX_Manager *gfx, const char *text, Vector2 pos,
                            Color col, drawMask::id layer) {
  if (text == nullptr || gfx == nullptr)
    return;

  Vector2 currentPos = pos;
  float spacing = (float)conf::DEFAULT_FONT_SPACING;

  for (int i = 0; text[i] != '\0'; i++) {
    int codepoint = (unsigned char)text[i]; // Simple ASCII assumption

    if (codepoint == '\n') {
      currentPos.y += fontHackRegular.baseSize; // Move down
      currentPos.x = pos.x;                     // Reset X
      continue;
    }

    int index = GetGlyphIndex(fontHackRegular, codepoint);

    if (index != -1) {
      Rectangle srcRec = fontHackRegular.recs[index];
      GlyphInfo glyph = fontHackRegular.glyphs[index];

      Rectangle dstRec = {currentPos.x + glyph.offsetX,
                          currentPos.y + glyph.offsetY, srcRec.width,
                          srcRec.height};

      gfx->LoadTextureToBackbuffer_Raw(layer, fontHackRegular.texture, srcRec,
                                       dstRec,
                                       {.color = col, .origin = {0.0f, 0.0f}});

      if (glyph.advanceX == 0)
        currentPos.x += (float)srcRec.width + spacing;
      else
        currentPos.x += (float)glyph.advanceX + spacing;
    }
  }
}

// --- Getters ---
Font FontHandler::GetFontHackRegular() { return this->fontHackRegular; }
int FontHandler::GetFontSizeDefault() { return this->fontSizeDefault; }