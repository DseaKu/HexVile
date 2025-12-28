#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "texture_atlas.h"
#include <iostream>

GFX_Manager::GFX_Manager() {
  GFX_Data_Buffers[0].resize(drawMask::SIZE);
  GFX_Data_Buffers[1].resize(drawMask::SIZE);
  backBufferIndex = 1; // Start writing to 1
  textureAtlas = {0, 0, 0, 0, 0};
  hitShader = {0};
}

void GFX_Manager::InitTextureRec() {

  float reso = static_cast<float>(tex_atlas::RES32);
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
  this->TA_Width = this->textureAtlas.width / tex_atlas::RES32;
  this->TA_Height = this->textureAtlas.height / tex_atlas::RES32;
  InitTextureRec();

  this->hitShader = LoadShader(0, conf::HIT_FLASH_SHADER_PATH);

  return 0;
}

Rectangle GFX_Manager::GetSrcRec(int x, int y) { return textureRecData[y][x]; }

void GFX_Manager::UnloadAssets() {
  UnloadTexture(this->textureAtlas);
  UnloadShader(this->hitShader);
}

void GFX_Manager::LoadGFX_Data(drawMask::id layerID, tex_atlas::Coords texAtlas,
                               Rectangle dstRec, Color col, bool useHitShader) {
  Rectangle srcRec = GetSrcRec(texAtlas.x, texAtlas.y);
  // Write to Back Buffer
  GFX_Data_Buffers[backBufferIndex][static_cast<int>(layerID)].emplace(
      dstRec.y, GFX_Props{textureAtlas, srcRec, dstRec, col, useHitShader});
}

void GFX_Manager::LoadGFX_Data(drawMask::id layerID, tex_atlas::Coords texAtlas,
                               Vector2 dst, Color col, bool useHitShader) {
  Rectangle srcRec = GetSrcRec(texAtlas.x, texAtlas.y);
  // Write to Back Buffer
  Rectangle dstRec = {dst.x, dst.y, tex_atlas::RES32_F, tex_atlas::RES64_F};
  GFX_Data_Buffers[backBufferIndex][static_cast<int>(layerID)].emplace(
      dst.y, GFX_Props{textureAtlas, srcRec, dstRec, col, useHitShader});
}

void GFX_Manager::LoadGFX_Data(drawMask::id layerID, Texture2D texture,
                               Rectangle srcRec, Rectangle dstRec, Color col,
                               bool useHitShader) {
  // Write to Back Buffer
  GFX_Data_Buffers[backBufferIndex][static_cast<int>(layerID)].emplace(
      dstRec.y, GFX_Props{texture, srcRec, dstRec, col, useHitShader});
}

void GFX_Manager::LoadGFX_Data_32x64(drawMask::id layerID,
                                     tex_atlas::Coords texAtlas, Vector2 dst,
                                     Color col, bool useHitShader) {
  Rectangle srcRec = GetSrcRec(texAtlas.x, texAtlas.y);
  srcRec.height += tex_atlas::RES32_F;
  Rectangle dstRec = {dst.x, dst.y, tex_atlas::RES32_F, tex_atlas::RES64_F};

  // Write to Back Buffer
  GFX_Data_Buffers[backBufferIndex][static_cast<int>(layerID)].emplace(
      dst.y + tex_atlas::RES32_F,
      GFX_Props{textureAtlas, srcRec, dstRec, col, useHitShader});
}

void GFX_Manager::RenderLayer(drawMask::id maskID) {
  // Read from Front Buffer (1 - backBufferIndex)
  int frontIndex = 1 - backBufferIndex;
  auto &layer = GFX_Data_Buffers[frontIndex][static_cast<int>(maskID)];
  for (auto &item : layer) {
    GFX_Props &props = item.second;

    if (props.useHitShader)
      BeginShaderMode(hitShader);

    DrawTexturePro(props.texture, props.srcRec, props.dstRec, Vector2{0, 0},
                   0.0f, props.color);

    if (props.useHitShader)
      EndShaderMode();
  }
  layer.clear(); // Consumed
}

void GFX_Manager::SwapBuffers() {
  // Check if the NEW back buffer (old front) is clear?
  // RenderLayer clears it, so it should be fine.
  // Just to be safe, we could clear it here, but it's O(N) redundant if Render
  // cleared it. However, if we missed rendering a layer, it might persist.
  // Safety clear:
  int nextBack = 1 - backBufferIndex;
  for (auto &layer : GFX_Data_Buffers[nextBack]) {
    layer.clear();
  }

  backBufferIndex = nextBack;
}

Rectangle GFX_Manager::GetTileRec(tile::id tileID, int frame) {
  int x_idx = (tex_atlas::TILE_X / tex_atlas::RES32) + frame;
  int y_idx = static_cast<int>(tileID);
  return textureRecData[y_idx][x_idx];
}
