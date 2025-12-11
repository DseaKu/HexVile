#include "player.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "raymath.h"

Player::Player() {
  position = Config::SCREEN_CENTER;
  faceDir = DIR_NULL;
  animationFrame = 0.0f;
  animationDelta = 0;
  position = Config::SCREEN_CENTER;
  InitAnimations();
}

void Player::Update() {
  animationDelta += GetFrameTime();
  Vector2 dir;
  dir.x = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  dir.y = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);

  // Determine player face direction
  if (dir.x == -1 && dir.y == -1) {
    faceDir = NW;
  } else if (dir.x == -1 && dir.y == 0) {
    faceDir = W;
  } else if (dir.x == -1 && dir.y == 1) {
    faceDir = SW;
  } else if (dir.x == 0 && dir.y == 1) {
    faceDir = S;
  } else if (dir.x == 1 && dir.y == 1) {
    faceDir = SE;
  } else if (dir.x == 1 && dir.y == 0) {
    faceDir = E;
  } else if (dir.x == -1 && dir.y == 0) {
    faceDir = NE;
  } else if (dir.x == 0 && dir.y == -1) {
    faceDir = N;
  } else {
    faceDir = DIR_NULL;
  }
  // Determine player state
  if (dir.x == 0 && dir.y == 0) {
    Player::Idle();
  } else if (dir.x != 0 || dir.y != 0) {
    Player::Walk(dir);
  } else {
    this->state = PLAYER_STATE_NULL;
  }

  // Calculate animation frame
  float animationSpeed =
      this->animationDelta * animationData[this->state][faceDir].speed;
  this->animationFrame =
      (int)animationSpeed % animationData[this->state][faceDir].frameCount;
}

void Player::Draw() {
  Vector2 playerPosition = this->position;
  playerPosition.x -= Config::ASSEST_RESOLUTION_HALF;
  playerPosition.y -= Config::ASSEST_RESOLUTION_HALF;
  float resolution = Config::ASSEST_RESOLUTION;
  float xFrameOffset = resolution * this->animationFrame;
  float yFrameOffset = resolution * this->state + resolution * this->faceDir;

  Rectangle assestRect = {xFrameOffset, yFrameOffset, resolution, resolution};

  Rectangle destRect = {playerPosition.x, playerPosition.y, resolution,
                        resolution};

  textureHandler->Draw(assestRect, destRect, {0.0f, 0.0f}, 0.0f, WHITE);
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

const char *Player::PlayerDirToString() {
  switch (this->faceDir) {
  case DIR_NULL:
    return "NULL";
  case NW:
    return "North West";
  case W:
    return "      West";
  case SW:
    return "South West";
  case S:
    return "South     ";
  case SE:
    return "South East";
  case E:
    return "      East";
  case NE:
    return "North East";
  case N:
    return "North     ";
  default:
    return "Unknown Direction";
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
  // Normalize diagonal movement
  if (Vector2Length(direction) > 0) {
    direction = Vector2Normalize(direction);
  }
  this->position.x += direction.x * delta * speed;
  this->position.y += direction.y * delta * speed;
}

void Player::InitAnimations() {
  for (int i = 0; i < PLAYER_STATE_ID_LENGTH; i++) {
    for (int j = 0; j < DIR_LABELS_LENGTH; j++) {
      this->animationData[i][j] = {.frameCount = 8,
                                   .speed = Config::PLAYER_ANIMATION_SPEED,
                                   .loop = true};
    }
  }
}
