#include "player.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "raymath.h"
#include "texture_atlas.h"

Player::Player() {
  position = Conf::SCREEN_CENTER;
  previousPosition = position;
  speedTilesPerSecond = 0.0f;
  faceDir = S;
  state = PLAYER_STATE_IDLE;
  animationFrame = 0;
  animationDelta = 0.0f;
  playerTile = {0, 0};

  InitAnimations();
}

void Player::Update(const KeyboardInput *keyboardInput, float deltaTime) {
  animationDelta += deltaTime;
  playerTile = hexGrid->PointToHexCoord(position);

  Vector2 dir = {0, 0};
  dir.x = -keyboardInput->Left + keyboardInput->Right;
  dir.y = -keyboardInput->Up + keyboardInput->Down;

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
  } else {
    Walk(dir, deltaTime);
  }

  // Calculate Speed
  float distance = Vector2Distance(position, previousPosition);
  if (deltaTime > 0) {
    speedTilesPerSecond = distance / deltaTime / Conf::TILE_RESOLUTION;
  } else {
    speedTilesPerSecond = 0;
  }
  previousPosition = position;

  // Calculate animation frame
  float currentSpeed = animationData[state][faceDir].speed;
  float frameCount = (float)animationData[state][faceDir].frameCount;

  float animationProgress = animationDelta * currentSpeed;
  animationFrame = (int)animationProgress % (int)frameCount;

  GenerateDrawData();
}

void Player::Idle() {
  if (state != PlayerStateID::PLAYER_STATE_IDLE) {
    animationFrame = 0;
    state = PlayerStateID::PLAYER_STATE_IDLE;
  }
}

void Player::Walk(Vector2 dir, float deltaTime) {
  float speed = Conf::PLAYER_MOVE_SPEED;

  if (state != PLAYER_STATE_WALK) {
    animationFrame = 0;
    state = PLAYER_STATE_WALK;
  }

  // Normalize diagonal movement
  if (Vector2Length(dir) > 0) {
    dir = Vector2Normalize(dir);
  }

  Vector2 nextPos = {position.x + dir.x * deltaTime * speed,
                     position.y + dir.y * deltaTime * speed};

  float offsetX = dir.x * Conf::PLAYER_COLLISION_CHECK_OFFSET;
  float offsetY = dir.y * Conf::PLAYER_COLLISION_CHECK_OFFSET;
  Vector2 checkPos = {nextPos.x + offsetX, nextPos.y + offsetY};

  // Check if destination is walkable
  if (hexGrid->IsWalkable(hexGrid->PointToHexCoord(checkPos))) {
    position = nextPos;
  } else {
    // Idle if blocked
    Idle();
  }
}

void Player::InitAnimations() {
  // Default init
  for (int i = 0; i < PLAYER_STATE_ID_SIZE; i++) {
    for (int j = 0; j < DIR_LABELS_SIZE; j++) {
      animationData[i][j] = {.frameCount = TA::PLAYER_X_MAX,
                             .speed = TA::PLAYER_ANIMATION_SPEED,
                             .loop = true};
    }
  }

  // Walk Specifics
  for (int i = 0; i < DIR_LABELS_SIZE; i++) {
    animationData[PLAYER_STATE_WALK][i].frameCount = TA::PLAYER_WALK_MAX;
  }

  // Idle Specifics
  for (int i = 0; i < DIR_LABELS_SIZE; i++) {
    animationData[PLAYER_STATE_IDLE][i].speed = TA::PLAYER_ANIMATION_SPEED_IDLE;
  }
}

// --- Setters ---
void Player::SetGFX_Manager(GFX_Manager *graphicsManager) {
  this->graphicsManager = graphicsManager;
}

void Player::SetHexGrid(HexGrid *grid) { this->hexGrid = grid; }

// --- Getters ---
Vector2 Player::GetPosition() const { return position; }

HexCoord Player::GetTile() const { return playerTile; }

int Player::GetAnimationFrame() const { return animationFrame; }

float Player::GetSpeedTilesPerSecond() const { return speedTilesPerSecond; }

const char *Player::PlayerStateToString() const {
  switch (state) {
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

const char *Player::PlayerDirToString() const {
  switch (faceDir) {
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

// --- Rendering ---
void Player::GenerateDrawData() {
  Vector2 drawPos = position;
  drawPos.x -= TA::RES16;
  drawPos.y -= TA::RES16 - Conf::PLAYER_SPRITE_Y_OFFSET;

  float resolution = TA::RES;

  int TA_X = animationFrame + TA::PLAYER_X;
  int TA_Y = (state - 1) * (DIR_LABELS_SIZE - 1) + faceDir;

  Rectangle dstRect = {drawPos.x, drawPos.y, resolution, resolution};

  graphicsManager->LoadGFX_Data(DRAW_MASK_ON_GROUND, drawPos.y, TA_X, TA_Y,
                                dstRect, WHITE);
}
