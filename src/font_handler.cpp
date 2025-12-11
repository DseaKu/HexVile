#include "font_handler.h"
#include "defines.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

FontHandler::FontHandler() {

  this->fontSizeDefault = Config::FONT_SIZE_DEFAULT;
  this->fontHackRegular = {0};
}

void FontHandler::InitFonts() {
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
  // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 4
  // px, pack method: 0 (default)
  Image atlas = GenImageFontAtlas(fontHackRegular.glyphs, &fontHackRegular.recs,
                                  Config::HACK_REGULAR_GLYPH_COUNT, 16, 4, 0);
  this->fontHackRegular.texture = LoadTextureFromImage(atlas);
  UnloadImage(atlas);
}

void FontHandler::DeInitFonts() { UnloadFont(fontHackRegular); }
Font FontHandler::getFontHackRegular() { return this->fontHackRegular; }
int FontHandler::getFontSizeDefault() { return this->fontSizeDefault; }
