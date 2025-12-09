#include "player.h"
#include "defines.h"
#include "raylib.h"

Player::Player() { position = Config::SCREEN_CENTER; }

void Player::Init(Vector2 pos) { position = pos; }
void Player::Update() {

  float speed = 10;
  float delta = GetFrameTime();
  int x_movement = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  int y_movement = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);
  position.x += x_movement * delta * speed;
  position.y += y_movement * delta * speed;
}
void Player::Draw() { DrawCircle(position.x, position.y, 4.0f, GREEN); }

Vector2 Player::GetPosition() { return position; }
