#ifndef CAMERA_H
#define CAMERA_H
#include "raylib.h"
class GameCamera {
private:
  Camera2D camera;
  Vector2 *targetPosition;

public:
  GameCamera();
  ~GameCamera();
  void FollowPosition(Vector2 *position);
  void Update();
};
#endif // !CAMERA_H
