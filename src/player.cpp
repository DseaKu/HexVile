#include "player.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "raymath.h"

Player::Player() {
  position = Config::SCREEN_CENTER;
  isFacingRight = true;
  animationFrame = 0.0f;
  position = Config::SCREEN_CENTER;
}

void Player::GetTextureHandler(TextureHandler *textureHandler) {

  this->textureHandler = textureHandler;
}
void Player::Idle() {
  if (this->state != PlayerStateID::PLAYER_STATE_IDLE) {
    this->animationFrame = 0.0f;
    this->state = PlayerStateID::PLAYER_STATE_IDLE;
  } else {
    this->animationFrame += GetFrameTime();
  }
}

void Player::Walk(Vector2 direction) {

  float speed = Config::PLAYER_SPEED;
  float delta = GetFrameTime();

  if (this->state != PlayerStateID::PLAYER_STATE_WALK) {
    animationFrame = 0.0f;
    this->state = PlayerStateID::PLAYER_STATE_WALK;
  } else {
    this->animationFrame += delta;
  }

  if (direction.x < 0) {
    this->isFacingRight = true;
  } else if (direction.x > 0) {
    this->isFacingRight = false;
  }

  // Normalize diagonal movement
  if (Vector2Length(direction) > 0) {
    direction = Vector2Normalize(direction);
  }

  this->position.x += direction.x * delta * speed;
  this->position.y += direction.y * delta * speed;
}

void Player::Update() {

  Vector2 direction;
  direction.x = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  direction.y = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);

  // Player Idle
  if (direction.x == 0 && direction.y == 0) {
    Player::Idle();
  }

  // Player Walk
  if (direction.x != 0 || direction.y != 0) {
    Player::Walk(direction);
  }
}

void Player::Draw() {
  Vector2 playerPosition = this->position;
  float resolution = Config::ASSEST_RESOLUTION;
  playerPosition.x -= Config::ASSEST_RESOLUTION_HALF;
  playerPosition.y -= Config::ASSEST_RESOLUTION_HALF;

  if (this->animationFrame > 8) {
    this->animationFrame = 0;
  }
  int xOffset = (int)animationFrame % 8;

  Rectangle assestRect = {xOffset * resolution, resolution * (float)this->state,
                          (float)resolution, resolution};

  Rectangle destRect = {playerPosition.x, playerPosition.y, resolution,
                        resolution};

  Vector2 origin = {0.0f, 0.0f};

  textureHandler->Draw(assestRect, destRect, origin, 0.0f, WHITE,
                       this->isFacingRight);
}

Vector2 Player::GetPosition() { return position; }

const char *Player::PlayerStateToString() {
  switch (this->state) {
  case PlayerStateID::PLAYER_STATE_NULL:
    return "NULL";
  case PlayerStateID::PLAYER_STATE_IDLE:
    return "IDLE";
  case PlayerStateID::PLAYER_STATE_WALK:
    return "WALK";
  default:
    return "Unknown State";
  }
}
