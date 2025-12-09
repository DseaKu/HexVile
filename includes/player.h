#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
class Player {
private:
  Vector2 position;

public:
  Player();
  void Init(Vector2 pos);
  void Update();
  void Draw();
  Vector2 GetPosition();
};
#endif // !PLAYER_H
