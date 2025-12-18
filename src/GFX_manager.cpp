#include "GFX_manager.h"
#include "enums.h"
#include "raylib.h"
#include "texture_atlas.h"
#include <iostream>

GFX_Manager::GFX_Manager() {
  GFX_Data.resize(DRAW_MASK_SIZE);
  textureAtlas = {0, 0, 0, 0, 0};
}

void GFX_Manager::InitTextureRec() {

  float reso = static_cast<float>(TA::RES);
  textureRecData.resize(TA_Height);

  for (int y = 0; y < TA_Height; y++) {
    textureRecData[y].resize(TA_Width);
    for (int x = 0; x < TA_Width; x++) {

      float xOffset = static_cast<float>(x * reso);
      float yOffset = static_cast<float>(y * reso);
      textureRecData[y][x] =
          Rectangle{.x = xOffset, .y = yOffset, .width = reso, .height = reso};
    }
  }
}

int GFX_Manager::LoadAssets(const char *pathToAssest) {
  this->textureAtlas = LoadTexture(pathToAssest);
  // Catch error
  if (this->textureAtlas.id == 0) {
    std::cout << "Error loading texture atlas" << std::endl;
    return 1;
  }
  this->TA_Width = this->textureAtlas.width / TA::RES;
  this->TA_Height = this->textureAtlas.height / TA::RES;
  InitTextureRec();
  return 0;
}

Rectangle GFX_Manager::GetSrcRec(int x, int y) { return textureRecData[y][x]; }

void GFX_Manager::UnloadAssets() { UnloadTexture(this->textureAtlas); }

void GFX_Manager::LoadGFX_Data(DrawMaskID maskID, float y, int TA_X, int TA_Y,
                               Rectangle dstRec, Color col) {
  Rectangle srcRec = GetSrcRec(TA_X, TA_Y);
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

Rectangle GFX_Manager::GetTileRec(TileID tileID, int frame) {
  int x_idx = (TA::TILE_X / TA::RES) + frame;
  int y_idx = static_cast<int>(tileID);
  return textureRecData[y_idx][x_idx];
}
