#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "raylib.h"

#include "enums.h"
#include "texture.h"
#include <vector>

namespace gfx {

struct Object {
  float sortY;
  Texture2D texture;
  Rectangle srcRec;
  Rectangle dstRec;
  Color color;
  bool useHitShader;
};

struct Opts {
  Color color = WHITE;
  bool useHitShader = false;

  float srcWidth = 0.0f;
  float srcHeight = 0.0f;
  float dstWidth = 0.0f;
  float dstHeight = 0.0f;
  float dstRecY = 0.0f;

  float sortingOffsetY = 0.0f;
};
constexpr Opts TextureOpts32x64 = {.srcHeight = tex::size::TILE * 2.0f,
                                   .dstHeight = tex::size::TILE * 2.0f,
                                   .dstRecY = -tex::size::TILE};
constexpr Opts TextureOptsInventoryBG = {
    .srcWidth = tex::atlas::INVENTORY.width * tex::size::TILE,
    .srcHeight = tex::atlas::INVENTORY.height * tex::size::TILE,
    // .dstWidth = tex::atlas::INVENTORY_WIDTH,
    // .dstHeight = tex::atlas::RSRC_TREE_HEIGHT};
};
} // namespace gfx

class GFX_Manager {
private:
  int TA_Width;
  int TA_Height;
  Texture2D textureAtlas;
  Shader hitShader;
  std::vector<std::vector<Rectangle>> textureRecData;

  // 0: Front (Render), 1: Back (Logic)
  std::vector<std::vector<std::vector<gfx::Object>>>
      GFX_Data_Buffers; // [BufferIndex][LayerID][Object]
  //
  int backBufferIndex = 1;

  void InitTextureRec();
  Rectangle GetSrcRec(int x, int y);

public:
  GFX_Manager();
  int LoadAssets(const char *);
  void UnloadAssets();

  void LoadTextureToBackbuffer(drawMask::id layerID,
                               tex::atlas::Coords texAtlas, Vector2 dst,
                               gfx::Opts opts = {});

  void LoadTextureToBackbuffer_Ex(drawMask::id layerID,
                                  tex::atlas::Coords texAtlas, Rectangle dstRec,
                                  gfx::Opts opts = {});

  void LoadTextureToBackbuffer_Raw(drawMask::id layerID, Texture2D texture,
                                   Rectangle srcRec, Rectangle dstRec,
                                   gfx::Opts opts = {});

  void RenderLayer(drawMask::id layer);

  Rectangle GetTileRec(tile::id id, int frame);

  void SwapBuffers();
};

#endif // !GRAPHICS_MANAGER_H
