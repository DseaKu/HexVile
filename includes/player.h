#ifndef PLAYER_H
#define PLAYER_H

#include "enums.h"
#include "raylib.h"
#include "texture_handler.h"

struct PlayerAnimationData {
  int frameCount;
  float speed;
  bool loop;
};

class Player {
private:
  int animationFrame;
  float animationDelta;
  DirLabels faceDir;
  Vector2 position;
  TextureHandler *textureHandler;
  PlayerStateID state;
  PlayerAnimationData animationData[PLAYER_STATE_ID_LENGTH][DIR_LABELS_LENGTH];

  void Idle();
  void Walk(Vector2 direction);
  void InitAnimations();

public:
  Player();
  void Update();
  void Draw();
  Vector2 GetPosition();
  void GetTextureHandler(TextureHandler *textureHandler);
  const char *PlayerStateToString();
  const char *PlayerDirToString();
};
#endif // !PLAYER_H
