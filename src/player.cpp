#include "player.h"
#include "defines.h"
#include "raylib.h"
#include "raymath.h"

Player::Player() {
  position = Config::SCREEN_CENTER;
  isFacingRight = true;
  frame = 0.0f;
  position = Config::SCREEN_CENTER;
}

void Player::GetTextureHandler(TextureHandler *textureHandler) {

  this->textureHandler = textureHandler;
}

void Player::Update() {

  float speed = Config::PLAYER_SPEED;
  float delta = GetFrameTime();

  Vector2 direction;
  direction.x = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  direction.y = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);

  if (direction.x < 0) {
    this->isFacingRight = true;
  } else if (direction.x > 0) {
    this->isFacingRight = false;
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
  float reso = Config::ASSEST_RESOLUTION;
  pos.x -= Config::ASSEST_RESOLUTION_HALF;
  pos.y -= Config::ASSEST_RESOLUTION_HALF;

  Rectangle tile_rect = {(float)((int)this->frame % 8) * reso, 64, (float)reso,
                         reso};

  Rectangle dest_rect = {pos.x, pos.y, reso, reso};

  Vector2 origin = {0.0f, 0.0f};

  textureHandler->Draw(tile_rect, dest_rect, origin, 0.0f, WHITE,
                       this->isFacingRight);
}

Vector2 Player::GetPosition() { return position; }
