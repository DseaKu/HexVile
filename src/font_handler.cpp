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

  // Loading font data from memory data
  // Parameters > font size: 16, no glyphs array provided (0), glyphs count: 95
  // (autogenerate chars array)
  this->fontHackRegular.glyphs =
      LoadFontData(fileData, fileSize, Config::FONT_SIZE_DEFAULT, 0,
                   Config::HACK_REGULAR_GLYPH_COUNT, FONT_DEFAULT,
                   &fontHackRegular.glyphCount);
  // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 4
  // px, pack method: 0 (default)
  Image atlas = GenImageFontAtlas(fontHackRegular.glyphs, &fontHackRegular.recs,
                                  95, 16, 4, 0);
  this->fontHackRegular.texture = LoadTextureFromImage(atlas);
  UnloadImage(atlas);

  // SDF font generation from TTF f
  this->fontSDF = {0};
  fontSDF.baseSize = 16;
  fontSDF.glyphCount = 95;
  // Parameters > font size: 16, no glyphs array provided (0), glyphs count: 0
  // (defaults to 95)
  fontSDF.glyphs =
      LoadFontData(fileData, fileSize, 16, 0, 0, FONT_SDF, &fontSDF.glyphCount);
  // Parameters > glyphs count: 95, font size: 16, glyphs padding in image: 0
  // px, pack method: 1 (Skyline algorythm)
  atlas = GenImageFontAtlas(fontSDF.glyphs, &fontSDF.recs, 95, 16, 0, 1);
  fontSDF.texture = LoadTextureFromImage(atlas);
  UnloadImage(atlas);

  UnloadFileData(fileData); // Free memory from loaded file

  // Load SDF required shader (we use default vertex shader)
  shader = LoadShader(
      0, TextFormat("resources/shaders/glsl%i/sdf.fs", GLSL_VERSION));
  SetTextureFilter(fontSDF.texture,
                   TEXTURE_FILTER_BILINEAR); // Required for SDF font

  Vector2 fontPosition = {40, Config::SCREEN_HEIGHT / 2.0f - 50};
  Vector2 textSize = {0.0f, 0.0f};
  float fontSize = 16.0f;
  int currentFont = 0; // 0 - fontHackRegular, 1 - fontSDF
}

void FontHandler::DeInitFonts() {
  UnloadFont(fontHackRegular); // Default font unloading
  UnloadFont(fontSDF);         // SDF font unloading

  UnloadShader(shader); // Unload SDF shader
}
Font FontHandler::getFontHackRegular() { return this->fontHackRegular; }
int FontHandler::getFontSizeDefault() { return this->fontSizeDefault; }
