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
  Vector2 origin;
  Color color;
  bool useHitShader;
};

} // namespace gfx

class GFX_Manager {
private:
  // --- Members ---
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

  // --- Private Methods ---
  void InitTextureRec();
  Rectangle GetSrcRec(int x, int y);

public:
  // --- Constructors ---
  GFX_Manager();

  // --- Core Lifecycle ---
  int LoadAssets(const char *);
  void UnloadAssets();

  // --- Graphics / Backbuffer ---
  void LoadTextureToBackbuffer(drawMask::id layerID, tex::atlas::Coords texAtlas,
                               Vector2 dst, tex::Opts opts = {});
  void LoadTextureToBackbuffer_Raw(drawMask::id layerID, Texture2D texture,
                                   Rectangle srcRec, Rectangle dstRec,
                                   tex::Opts opts = {});
  void RenderLayer(drawMask::id layer);
  void SwapBuffers();

  // --- Getters ---
  Rectangle GetTileRec(tile::id id, int frame);
};

#endif // !GRAPHICS_MANAGER_H
