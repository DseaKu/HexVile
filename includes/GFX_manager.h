#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "raylib.h"

#include "enums.h"
#include <map>
#include <vector>

struct GFX_Props {
  Texture2D texture;
  Rectangle srcRec;
  Rectangle dstRec;
  Color color;
};

class GFX_Manager {
private:
  int TA_Width;
  int TA_Height;
  Texture2D textureAtlas;
  std::vector<std::vector<Rectangle>> textureRecData;

  // Double buffering for thread safety
  // 0: Front (Render), 1: Back (Logic)
  std::vector<std::multimap<float, GFX_Props>> GFX_Data_Buffers[2];
  int backBufferIndex = 1;

  void InitTextureRec();
  Rectangle GetSrcRec(int x, int y);

public:
  GFX_Manager();
  int LoadAssets(const char *);
  void UnloadAssets();
  void LoadGFX_Data(drawMask::id layerID, int TA_X, int TA_Y, Rectangle dstRec,
                    Color col);

  void LoadGFX_Data(drawMask::id layerID, int TA_X, int TA_Y, Color col);

  void LoadGFX_Data(drawMask::id layerID, Texture2D texture, Rectangle srcRec,
                    Rectangle dstRec, Color col);

  // Combine 2 assets rectangle and load as one render object
  void LoadGFX_Data_32x64(drawMask::id layerID, int TA_X, int TA_Y,
                          Rectangle dstRec, Color col);
  void RenderLayer(drawMask::id layer);

  Rectangle GetTileRec(tile::id id, int frame);

  void SwapBuffers();
};

#endif // !GRAPHICS_MANAGER_H
