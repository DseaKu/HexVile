#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "texture_handler.h"

class Player {
private:
  Vector2 position;
  float frame;
  bool isFacingRight;
  TextureHandler *textureHandler;

public:
  Player();
  void Update();
  void Draw();
  Vector2 GetPosition();
  void GetTextureHandler(TextureHandler *textureHandler);
};
#endif // !PLAYER_H
