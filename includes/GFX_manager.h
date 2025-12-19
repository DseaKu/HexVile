#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "raylib.h"

#include "enums.h"
#include <map>
#include <vector>

struct GFX_Props {
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
  void LoadGFX_Data(DrawMaskID maskID, float y, int TA_X, int TA_Y,
                    Rectangle dstRec, Color Color);
  void RenderLayer(DrawMaskID maskID);
  Rectangle GetTileRec(TileID tileID, int frame);

  void SwapBuffers();
};

#endif // !GRAPHICS_MANAGER_H
