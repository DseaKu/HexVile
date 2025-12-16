#include "texture_handler.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include <iostream>

TextureHandler::TextureHandler() {
  DrawData.resize(DRAW_MASK_SIZE);
  textureAtlas = {0, 0, 0, 0, 0};
}

int TextureHandler::LoadAssets(const char *pathToAssest) {
  this->textureAtlas = LoadTexture(pathToAssest);
  // Catch error
  if (this->textureAtlas.id == 0) {
    std::cout << "Error loading texture atlas" << std::endl;
    return 1;
  }
  this->width = this->textureAtlas.width / TA::ASSEST_RESOLUTION;
  this->height = this->textureAtlas.height / TA::ASSEST_RESOLUTION;
  return 0;
}

void TextureHandler::UnloadAssets() { UnloadTexture(this->textureAtlas); }

void TextureHandler::Draw(Rectangle assetsRect, Rectangle destRect,
                          Vector2 origin, float rotation, Color color) {
  DrawTexturePro(this->textureAtlas, assetsRect, destRect, origin, rotation,
                 color);
}

void TextureHandler::LoadDrawData(DrawMaskID maskID, float y, Rectangle srcRec,
                                  Rectangle dstRect, Color col) {

  DrawData[static_cast<int>(maskID)].emplace(
      y, DrawProperties{srcRec, dstRect, col});
}

void TextureHandler::RenderDrawData() {
  for (auto &layer : DrawData) {
    for (auto &item : layer) {
      DrawProperties &props = item.second;
      DrawTexturePro(textureAtlas, props.srcRec, props.dstRect, Vector2{0, 0},
                     0.0f, props.color);
    }
    layer.clear();
  }
}
