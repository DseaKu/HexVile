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
  Texture2D textureAtlas;
  int width;
  int height;
  std::vector<std::multimap<float, GFX_Props>> GFX_Data;

public:
  GFX_Manager();
  int LoadAssets(const char *);
  void UnloadAssets();
  void LoadGFX_Data(DrawMaskID maskID, float y, Rectangle srcRec,
                    Rectangle dstRec, Color Color);
  void RenderLayer(DrawMaskID maskID);
};

#endif // !GRAPHICS_MANAGER_H
