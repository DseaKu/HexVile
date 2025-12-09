#include "player.h"
#include "defines.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

Player::Player() { position = Config::SCREEN_CENTER; }

void Player::Init(Vector2 pos) { position = pos; }
void Player::Update() {

  float speed = Config::PLAYER_SPEED;
  float delta = GetFrameTime();

  Vector2 direction;
  direction.x = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  direction.y = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);

  if (Vector2Length(direction) > 0) {
    direction = Vector2Normalize(direction);
  }

  position.x += direction.x * delta * speed;
  position.y += direction.y * delta * speed;
}
void Player::Draw() { DrawCircle(position.x, position.y, 4.0f, GREEN); }

Vector2 Player::GetPosition() { return position; }
