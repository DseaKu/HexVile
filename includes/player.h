#ifndef PLAYER_H
#define PLAYER_H

#include "enums.h"
#include "raylib.h"
#include "texture_handler.h"

class Player {
private:
  Vector2 position;
  float animationFrame;
  bool isFacingRight;
  TextureHandler *textureHandler;
  PlayerStateID state;

  void Idle();
  void Walk(Vector2 direction);

public:
  Player();
  void Update();
  void Draw();
  Vector2 GetPosition();
  void GetTextureHandler(TextureHandler *textureHandler);
  const char *PlayerStateToString();
};
#endif // !PLAYER_H
