#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "raylib.h"

#include "enums.h"
#include "texture_atlas.h"
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

  float sortingOffsetY = 0.0f;
};

} // namespace gfx

class GFX_Manager {
private:
  int TA_Width;
  int TA_Height;
  Texture2D textureAtlas;
  Shader hitShader;
  std::vector<std::vector<Rectangle>> textureRecData;

  // Double buffering for thread safety
  // 0: Front (Render), 1: Back (Logic)
  // [BufferIndex][LayerID][Object]
  std::vector<std::vector<std::vector<gfx::Object>>> GFX_Data_Buffers;
  int backBufferIndex = 1;

  void InitTextureRec();
  Rectangle GetSrcRec(int x, int y);

public:
  GFX_Manager();
  int LoadAssets(const char *);
  void UnloadAssets();

  void LoadGFX_Data(drawMask::id layerID, tex_atlas::Coords texAtlas,
                    Vector2 dst, gfx::Opts opts = {});

  void LoadGFX_DataEx(drawMask::id layerID, tex_atlas::Coords texAtlas,
                      Rectangle dstRec, gfx::Opts opts = {});

  void LoadGFX_DataRaw(drawMask::id layerID, Texture2D texture,
                       Rectangle srcRec, Rectangle dstRec, gfx::Opts opts = {});

  void RenderLayer(drawMask::id layer);

  Rectangle GetTileRec(tile::id id, int frame);

  void SwapBuffers();
};

#endif // !GRAPHICS_MANAGER_H
