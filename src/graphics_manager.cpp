#include "graphics_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include <iostream>

GraphicsManager::GraphicsManager() {
  DrawData.resize(DRAW_MASK_SIZE);
  textureAtlas = {0, 0, 0, 0, 0};
}

int GraphicsManager::LoadAssets(const char *pathToAssest) {
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

void GraphicsManager::UnloadAssets() { UnloadTexture(this->textureAtlas); }

void GraphicsManager::Draw(Rectangle assetsRect, Rectangle destRect,
                          Vector2 origin, float rotation, Color color) {
  DrawTexturePro(this->textureAtlas, assetsRect, destRect, origin, rotation,
                 color);
}

void GraphicsManager::LoadDrawData(DrawMaskID maskID, float y, Rectangle srcRec,
                                  Rectangle dstRec, Color col) {

  DrawData[static_cast<int>(maskID)].emplace(
      y, DrawProperties{srcRec, dstRec, col});
}

void GraphicsManager::RenderDrawData(DrawMaskID maskID) {
  auto &layer = DrawData[static_cast<int>(maskID)];
  for (auto &item : layer) {
    DrawProperties &props = item.second;
    DrawTexturePro(textureAtlas, props.srcRec, props.dstRec, Vector2{0, 0},
                   0.0f, props.color);
  }
  layer.clear();
}
