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

class GFX_Manger {
private:
  Texture2D textureAtlas;
  int width;
  int height;
  std::vector<std::multimap<float, GFX_Props>> DrawData;

public:
  GFX_Manger();
  int LoadAssets(const char *);
  void UnloadAssets();
  void Draw(Rectangle assetsRec, Rectangle destRec, Vector2 origin,
            float rotation, Color color);

  void LoadDrawData(DrawMaskID maskID, float y, Rectangle srcRec,
                    Rectangle dstRec, Color Color);
  void RenderDrawData(DrawMaskID maskID);
};

#endif // !GRAPHICS_MANAGER_H
