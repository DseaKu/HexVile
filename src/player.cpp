#include "player.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "raymath.h"

Player::Player() {
  position = Config::SCREEN_CENTER;
  isFacingRight = true;
  animationFrame = 0.0f;
  animationDelta = 0;
  position = Config::SCREEN_CENTER;

  for (int i = 0; i < PLAYER_STATE_ID_LENGTH; i++) {
    this->animationData[i] = {
        .frameCount = 0, .speed = Config::PLAYER_ANIMATION_SPEED, .loop = true};
  }
  this->animationData[PLAYER_STATE_IDLE].frameCount = 9;
  this->animationData[PLAYER_STATE_IDLE].speed -= 5;
  this->animationData[PLAYER_STATE_WALK].frameCount = 8;
}

void Player::Update() {
  animationDelta += GetFrameTime();
  Vector2 direction;
  direction.x = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  direction.y = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);

  // Player Idle
  if (direction.x == 0 && direction.y == 0) {
    Player::Idle();

    // Player Walk
  } else if (direction.x != 0 || direction.y != 0) {
    Player::Walk(direction);

  } else {
    this->state = PLAYER_STATE_NULL;
  }

  // Calculate animation frame
  float animationSpeed =
      this->animationDelta * animationData[this->state].speed;
  this->animationFrame =
      (int)animationSpeed % animationData[this->state].frameCount;
}

void Player::Draw() {
  Vector2 playerPosition = this->position;
  float resolution = Config::ASSEST_RESOLUTION;
  playerPosition.x -= Config::ASSEST_RESOLUTION_HALF;
  playerPosition.y -= Config::ASSEST_RESOLUTION_HALF;

  Rectangle assestRect = {this->animationFrame * resolution,
                          resolution * (float)this->state, (float)resolution,
                          resolution};

  Rectangle destRect = {playerPosition.x, playerPosition.y, resolution,
                        resolution};

  Vector2 origin = {0.0f, 0.0f};

  textureHandler->Draw(assestRect, destRect, origin, 0.0f, WHITE,
                       this->isFacingRight);
}

Vector2 Player::GetPosition() { return position; }

const char *Player::PlayerStateToString() {
  switch (this->state) {
  case PLAYER_STATE_NULL:
    return "NULL";
  case PLAYER_STATE_IDLE:
    return "IDLE";
  case PLAYER_STATE_WALK:
    return "WALK";
  default:
    return "Unknown State";
  }
}
void Player::GetTextureHandler(TextureHandler *textureHandler) {

  this->textureHandler = textureHandler;
}
void Player::Idle() {
  if (this->state != PlayerStateID::PLAYER_STATE_IDLE) {
    this->animationFrame = 0.0f;
    this->state = PlayerStateID::PLAYER_STATE_IDLE;
  }
}

void Player::Walk(Vector2 direction) {

  float speed = Config::PLAYER_SPEED;
  float delta = GetFrameTime();

  if (this->state != PLAYER_STATE_WALK) {
    animationFrame = 0.0f;
    this->state = PLAYER_STATE_WALK;
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
