#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include <iostream>

GFX_Manager::GFX_Manager() {
  GFX_Data.resize(DRAW_MASK_SIZE);
  textureAtlas = {0, 0, 0, 0, 0};
}

int GFX_Manager::LoadAssets(const char *pathToAssest) {
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

void GFX_Manager::UnloadAssets() { UnloadTexture(this->textureAtlas); }

void GFX_Manager::LoadGFX_Data(DrawMaskID maskID, float y, Rectangle srcRec,
                               Rectangle dstRec, Color col) {

  GFX_Data[static_cast<int>(maskID)].emplace(y, GFX_Props{srcRec, dstRec, col});
}

void GFX_Manager::RenderLayer(DrawMaskID maskID) {
  auto &layer = GFX_Data[static_cast<int>(maskID)];
  for (auto &item : layer) {
    GFX_Props &props = item.second;
    DrawTexturePro(textureAtlas, props.srcRec, props.dstRec, Vector2{0, 0},
                   0.0f, props.color);
  }
  layer.clear();
}
