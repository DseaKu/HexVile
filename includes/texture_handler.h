#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include "raylib.h"
class TextureHandler {
private:
  Texture2D textureAtlas;
  int width;
  int height;

public:
  TextureHandler();
  int LoadAssets(const char *);
  void UnloadAssets();
  void Draw(Rectangle assetsRect, Rectangle destRect, Vector2 origin,
            float rotation, Color color);
};

#endif // !TEXTURE_HANDLER_H
