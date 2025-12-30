#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "texture_atlas.h"
#include <algorithm>
#include <iostream>

GFX_Manager::GFX_Manager() {
  GFX_Data_Buffers.resize(2);
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

void GFX_Manager::LoadTextureToBackbuffer(drawMask::id layerID,
                                          tex_atlas::Coords texAtlas,
                                          Vector2 dst, gfx::Opts opts) {

  Rectangle srcRec = GetSrcRec(texAtlas.x, texAtlas.y);
  Rectangle dstRec = {dst.x, dst.y, srcRec.width, srcRec.height};

  if (opts.srcWidth != 0.0f)
    srcRec.width = opts.srcWidth;
  if (opts.srcHeight != 0.0f)
    srcRec.height = opts.srcHeight;

  if (opts.dstWidth != 0.0f)
    dstRec.width = opts.dstWidth;
  if (opts.dstHeight != 0.0f) {
    dstRec.height = opts.dstHeight;
  }

  GFX_Data_Buffers[backBufferIndex][static_cast<int>(layerID)].push_back(
      {dst.y + opts.sortingOffsetY, textureAtlas, srcRec, dstRec, opts.color,
       opts.useHitShader});
}

void GFX_Manager::LoadTextureToBackbuffer_Ex(drawMask::id layerID,
                                             tex_atlas::Coords texAtlas,
                                             Rectangle dstRec, gfx::Opts opts) {
  Rectangle srcRec = GetSrcRec(texAtlas.x, texAtlas.y);
  if (opts.srcWidth > 0)
    srcRec.width = opts.srcWidth;
  if (opts.srcHeight > 0)
    srcRec.height = opts.srcHeight;

  // Write to Back Buffer
  GFX_Data_Buffers[backBufferIndex][static_cast<int>(layerID)].push_back(
      {dstRec.y + opts.sortingOffsetY, textureAtlas, srcRec, dstRec, opts.color,
       opts.useHitShader});
}

void GFX_Manager::LoadTextureToBackbuffer_Raw(drawMask::id layerID,
                                              Texture2D texture,
                                              Rectangle srcRec,
                                              Rectangle dstRec,
                                              gfx::Opts opts) {
  // Write to Back Buffer
  GFX_Data_Buffers[backBufferIndex][static_cast<int>(layerID)].push_back(
      {dstRec.y + opts.sortingOffsetY, texture, srcRec, dstRec, opts.color,
       opts.useHitShader});
}

void GFX_Manager::RenderLayer(drawMask::id maskID) {
  // Read from Front Buffer (1 - backBufferIndex)
  int frontIndex = 1 - backBufferIndex;
  auto &layer = GFX_Data_Buffers[frontIndex][static_cast<int>(maskID)];

  // Sort objects by Y position (Painter's Algorithm)
  std::sort(layer.begin(), layer.end(),
            [](const gfx::Object &a, const gfx::Object &b) {
              return a.sortY < b.sortY;
            });

  for (auto &item : layer) {
    // GFX_Props &props = item.props;

    if (item.useHitShader)
      BeginShaderMode(hitShader);

    DrawTexturePro(item.texture, item.srcRec, item.dstRec, Vector2{0, 0}, 0.0f,
                   item.color);

    if (item.useHitShader)
      EndShaderMode();
  }
  // Do NOT clear here. We clear the *new* back buffer in SwapBuffers.
  // This keeps the capacity for the next frame.
}

void GFX_Manager::SwapBuffers() {
  // Safety clear for the NEW back buffer (which was the old front buffer)
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
