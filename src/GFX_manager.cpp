#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include <iostream>

GFX_Manger::GFX_Manger() {
  DrawData.resize(DRAW_MASK_SIZE);
  textureAtlas = {0, 0, 0, 0, 0};
}

int GFX_Manger::LoadAssets(const char *pathToAssest) {
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

void GFX_Manger::UnloadAssets() { UnloadTexture(this->textureAtlas); }

void GFX_Manger::Draw(Rectangle assetsRect, Rectangle destRect, Vector2 origin,
                      float rotation, Color color) {
  DrawTexturePro(this->textureAtlas, assetsRect, destRect, origin, rotation,
                 color);
}

void GFX_Manger::LoadDrawData(DrawMaskID maskID, float y, Rectangle srcRec,
                              Rectangle dstRec, Color col) {

  DrawData[static_cast<int>(maskID)].emplace(y, GFX_Props{srcRec, dstRec, col});
}

void GFX_Manger::RenderDrawData(DrawMaskID maskID) {
  auto &layer = DrawData[static_cast<int>(maskID)];
  for (auto &item : layer) {
    GFX_Props &props = item.second;
    DrawTexturePro(textureAtlas, props.srcRec, props.dstRec, Vector2{0, 0},
                   0.0f, props.color);
  }
  layer.clear();
}
