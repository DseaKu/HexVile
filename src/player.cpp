#include "player.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "raymath.h"

Player::Player() {
  this->position = Conf::SCREEN_CENTER;
  this->faceDir = S;
  this->state = PLAYER_STATE_IDLE;
  this->animationFrame = 0.0f;
  this->animationDelta = 0;
  this->position = Conf::SCREEN_CENTER;
  this->playerTile = {0, 0};
  InitAnimations();
}

void Player::Update() {
  this->animationDelta += GetFrameTime();
  this->playerTile = this->hexGrid->PointToHexCoord(this->position);

  Vector2 dir;
  dir.x = -IsKeyDown(KEY_A) + IsKeyDown(KEY_D);
  dir.y = -IsKeyDown(KEY_W) + IsKeyDown(KEY_S);

  // Determine player face direction
  if (dir.x != 0 || dir.y != 0) {
    if (dir.x == 0 && dir.y == -1) {
      faceDir = N;
    } else if (dir.x == 1 && dir.y == -1) {
      faceDir = NE;
    } else if (dir.x == 1 && dir.y == 0) {
      faceDir = E;
    } else if (dir.x == 1 && dir.y == 1) {
      faceDir = SE;
    } else if (dir.x == 0 && dir.y == 1) {
      faceDir = S;
    } else if (dir.x == -1 && dir.y == 1) {
      faceDir = SW;
    } else if (dir.x == -1 && dir.y == 0) {
      faceDir = W;
    } else if (dir.x == -1 && dir.y == -1) {
      faceDir = NW;
    }
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

HexCoord Player::GetTile() { return this->playerTile; }

void Player::Draw() {
  Vector2 playerPosition = this->position;
  playerPosition.x -= Conf::ASSEST_RESOLUTION_HALF;
  playerPosition.y -= Conf::ASSEST_RESOLUTION_HALF - Conf::PLAYER_Y_OFFSET;
  float resolution = Conf::ASSEST_RESOLUTION;
  float xFrameOffset = resolution * this->animationFrame +
                       Conf::TEXTURE_ATLAS_PLAYER_ANIMATION_X_OFFSET;
  float yFrameOffset =
      resolution *
      ((this->state - 1) * (DIR_LABELS_LENGTH - 1) + (this->faceDir));

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
    return "NW";
  case W:
    return "W";
  case SW:
    return "SW";
  case S:
    return "S";
  case SE:
    return "SE";
  case E:
    return "E";
  case NE:
    return "NE";
  case N:
    return "N";
  default:
    return "Unknown Direction";
  }
}

int Player::GetAnimationFrame() { return this->animationFrame; }

void Player::SetTextureHandler(TextureHandler *textureHandler) {

  this->textureHandler = textureHandler;
}

void Player::Idle() {
  if (this->state != PlayerStateID::PLAYER_STATE_IDLE) {
    this->animationFrame = 0.0f;
    this->state = PlayerStateID::PLAYER_STATE_IDLE;
  }
}

void Player::Walk(Vector2 dir) {

  float speed = Conf::PLAYER_SPEED;
  float delta = GetFrameTime();

  if (this->state != PLAYER_STATE_WALK) {
    animationFrame = 0.0f;
    this->state = PLAYER_STATE_WALK;
  }
  // Normalize diagonal movement
  if (Vector2Length(dir) > 0) {
    dir = Vector2Normalize(dir);
  }
  Vector2 nextPos = {this->position.x + dir.x * delta * speed,
                     this->position.y + dir.y * delta * speed};

  this->position = nextPos;
}

void Player::InitAnimations() {
  for (int i = 0; i < PLAYER_STATE_ID_LENGTH; i++) {
    for (int j = 0; j < DIR_LABELS_LENGTH; j++) {
      this->animationData[i][j] = {
          .frameCount = Conf::TEXTURE_ATLAS_PLAYER_ANIMATION_FRAME_COUNT_MAX,
          .speed = Conf::PLAYER_ANIMATION_SPEED,
          .loop = true};
    }
  }
  for (int i = 0; i < DIR_LABELS_LENGTH; i++) {
    this->animationData[PLAYER_STATE_WALK][i].frameCount =
        Conf::TEXTURE_ATLAS_PLAYER_ANIMATION_FRAME_COUNT_WALK;
  }
  for (int i = 0; i < DIR_LABELS_LENGTH; i++) {
    this->animationData[PLAYER_STATE_IDLE][i].speed =
        Conf::TEXTURE_ATLAS_PLAYER_ANIMATION_SPEED_IDLE;
  }
}
void Player::SetHexGrid(HexGrid *grid) { this->hexGrid = grid; }
