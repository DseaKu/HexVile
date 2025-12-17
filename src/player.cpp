#include "player.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "raymath.h"

Player::Player() {
  this->position = Conf::SCREEN_CENTER;
  this->previousPosition = this->position;
  this->speedTilesPerSecond = 0.0f;
  this->faceDir = S;
  this->state = PLAYER_STATE_IDLE;
  this->animationFrame = 0.0f;
  this->animationDelta = 0;
  this->position = Conf::SCREEN_CENTER;
  this->playerTile = {0, 0};
  InitAnimations();
}

void Player::Update(PlayerInputState input, float deltaTime) {
  this->animationDelta += deltaTime;
  this->playerTile = this->hexGrid->PointToHexCoord(this->position);

  Vector2 dir;
  dir.x = -input.moveLeft + input.moveRight;
  dir.y = -input.moveUp + input.moveDown;

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
    Idle();
  } else if (dir.x != 0 || dir.y != 0) {
    Walk(dir, deltaTime);
  } else {
    this->state = PLAYER_STATE_NULL;
  }

  // Calculate Speed
  float distance = Vector2Distance(this->position, this->previousPosition);
  if (deltaTime > 0) {
    this->speedTilesPerSecond = distance / deltaTime / Conf::TILE_SIZE;
  } else {
    this->speedTilesPerSecond = 0;
  }
  this->previousPosition = this->position;

  // Calculate animation frame
  float animationSpeed =
      this->animationDelta * animationData[this->state][faceDir].speed;
  this->animationFrame =
      (int)animationSpeed % animationData[this->state][faceDir].frameCount;

  GenerateDrawData();
}

HexCoord Player::GetTile() { return this->playerTile; }

void Player::GenerateDrawData() {
  Vector2 playerPosition = this->position;
  playerPosition.x -= TA::ASSEST_RESOLUTION_HALF;
  playerPosition.y -= TA::ASSEST_RESOLUTION_HALF - Conf::PLAYER_Y_OFFSET;
  float resolution = TA::ASSEST_RESOLUTION;
  float xFrameOffset =
      resolution * this->animationFrame + TA::PLAYER_X_OFFSET_TILE;
  float yFrameOffset = resolution * ((this->state - 1) * (DIR_LABELS_SIZE - 1) +
                                     (this->faceDir));

  Rectangle srcRect = {xFrameOffset, yFrameOffset, resolution, resolution};

  Rectangle dstRect = {playerPosition.x, playerPosition.y, resolution,
                       resolution};

  graphicsManager->LoadGFX_Data(DRAW_MASK_ON_GROUND, playerPosition.y, srcRect,
                                dstRect, WHITE);
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

float Player::GetSpeedTilesPerSecond() { return this->speedTilesPerSecond; }

void Player::SetGFX_Manager(GFX_Manager *graphicsManager) {

  this->graphicsManager = graphicsManager;
}

void Player::Idle() {
  if (this->state != PlayerStateID::PLAYER_STATE_IDLE) {
    this->animationFrame = 0.0f;
    this->state = PlayerStateID::PLAYER_STATE_IDLE;
  }
}

void Player::Walk(Vector2 dir, float deltaTime) {

  float speed = Conf::PLAYER_SPEED;

  if (this->state != PLAYER_STATE_WALK) {
    animationFrame = 0.0f;
    this->state = PLAYER_STATE_WALK;
  }
  // Normalize diagonal movement
  if (Vector2Length(dir) > 0) {
    dir = Vector2Normalize(dir);
  }
  Vector2 nextPos = {this->position.x + dir.x * deltaTime * speed,
                     this->position.y + dir.y * deltaTime * speed};

  float offsetX = dir.x * Conf::OFFSET_TO_OBSTACLE;
  float offsetY = dir.y * Conf::OFFSET_TO_OBSTACLE;
  Vector2 a = {nextPos.x + offsetX, nextPos.y + offsetY};
  // Check if destination is walkable
  if (this->hexGrid->IsWalkable(this->hexGrid->PointToHexCoord(a))) {
    this->position = nextPos;
  } else {
    Idle();
  }
}

void Player::InitAnimations() {
  for (int i = 0; i < PLAYER_STATE_ID_SIZE; i++) {
    for (int j = 0; j < DIR_LABELS_SIZE; j++) {
      this->animationData[i][j] = {.frameCount = TA::PLAYER_X_MAX,
                                   .speed = TA::PLAYER_ANIMATION_SPEED,
                                   .loop = true};
    }
  }
  for (int i = 0; i < DIR_LABELS_SIZE; i++) {
    this->animationData[PLAYER_STATE_WALK][i].frameCount =
        TA::PLAYER_X_WALK_MAX;
  }
  for (int i = 0; i < DIR_LABELS_SIZE; i++) {
    this->animationData[PLAYER_STATE_IDLE][i].speed =
        TA::PLAYER_ANIMATION_SPEED_IDLE;
  }
}
void Player::SetHexGrid(HexGrid *grid) { this->hexGrid = grid; }
