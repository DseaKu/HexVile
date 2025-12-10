#include "player.h"
#include "defines.h"
#include "raylib.h"
#include "raymath.h"

Player::Player() {
  position = Config::SCREEN_CENTER;
  faceDir = 1;
}

void Player::Init(Vector2 pos, const char *pathToAssets) {

  assets = LoadTexture(pathToAssets);
  position = pos;
  frame = 0.0f;
}
void Player::Update() {

  float speed = Config::PLAYER_SPEED;
  float delta = GetFrameTime();

  Vector2 direction;
  direction.x = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  direction.y = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);

  if (direction.x != 0) {
    this->faceDir = direction.x;
  }

  if (Vector2Length(direction) > 0) {
    direction = Vector2Normalize(direction);
    this->frame += delta * 10.0f;
    if (this->frame >= 8.0f) {
      this->frame = 0.0f;
    }
  } else {
    this->frame = 0.0f;
  }

  position.x += direction.x * delta * speed;
  position.y += direction.y * delta * speed;
}
void Player::Draw() {
  Vector2 pos = this->position;
  pos.x -= 24;
  pos.y -= 24;

  Rectangle tile_rect = {(float)((int)this->frame % 8) * 48, 0,
                         (float)48 * this->faceDir, 48};
  Rectangle dest_rect = {pos.x, pos.y, 48, 48};

  Vector2 origin = {0.0f, 0.0f};
  DrawTexturePro(this->assets, tile_rect, dest_rect, origin, 0.0f, WHITE);
}

Vector2 Player::GetPosition() { return position; }
