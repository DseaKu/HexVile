#include "font_handler.h"
#include "defines.h"
#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#endif

FontHandler::FontHandler() {

  this->fontSizeDefault = Config::FONT_SIZE_DEFAULT;
  this->fontHackRegular = {0};
}

void FontHandler::LoadFonts() {
  int fileSize = 0;

  // --- Hack Font Regular ---
  unsigned char *fileData =
      LoadFileData(Config::FONT_HACK_REGULAR_PATH, &fileSize);
  this->fontHackRegular.baseSize = this->fontSizeDefault;
  this->fontHackRegular.glyphCount = Config::HACK_REGULAR_GLYPH_COUNT;
  this->fontHackRegular.glyphs =
      LoadFontData(fileData, fileSize, Config::FONT_SIZE_DEFAULT, 0,
                   Config::HACK_REGULAR_GLYPH_COUNT, FONT_DEFAULT,
                   &fontHackRegular.glyphCount);
  Image atlas = GenImageFontAtlas(fontHackRegular.glyphs, &fontHackRegular.recs,
                                  Config::HACK_REGULAR_GLYPH_COUNT,
                                  this->fontHackRegular.baseSize, 0, 0);
  this->fontHackRegular.texture = LoadTextureFromImage(atlas);
  UnloadImage(atlas);
}

void FontHandler::UnloadFonts() { UnloadFont(fontHackRegular); }
Font FontHandler::GetFontHackRegular() { return this->fontHackRegular; }
int FontHandler::GetFontSizeDefault() { return this->fontSizeDefault; }
void FontHandler::DrawTextHackRegular(const char *text, Vector2 pos,
                                      Color color) {
  DrawTextEx(this->fontHackRegular, text, pos, Config::FONT_SIZE_DEFAULT,
             Config::FONT_SPACING_DEFAULT, color);
}
