#include "player.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "raymath.h"
#include "texture_atlas.h"

Player::Player() {
  position = conf::SCREEN_CENTER;
  previousPosition = position;
  speedTilesPerSecond = 0.0f;
  dirID = dir::S;
  stateID = playerState::IDLE;
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
      dirID = dir::N;
    } else if (dir.x == 1 && dir.y == -1) {
      dirID = dir::NE;
    } else if (dir.x == 1 && dir.y == 0) {
      dirID = dir::E;
    } else if (dir.x == 1 && dir.y == 1) {
      dirID = dir::SE;
    } else if (dir.x == 0 && dir.y == 1) {
      dirID = dir::S;
    } else if (dir.x == -1 && dir.y == 1) {
      dirID = dir::SW;
    } else if (dir.x == -1 && dir.y == 0) {
      dirID = dir::W;
    } else if (dir.x == -1 && dir.y == -1) {
      dirID = dir::NW;
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
    speedTilesPerSecond = distance / deltaTime / conf::TILE_RESOLUTION;
  } else {
    speedTilesPerSecond = 0;
  }
  previousPosition = position;

  // Calculate animation frame
  float currentSpeed = animationData[stateID][dirID].speed;
  float frameCount = (float)animationData[stateID][dirID].frameCount;

  float animationProgress = animationDelta * currentSpeed;
  animationFrame = (int)animationProgress % (int)frameCount;

  GenerateDrawData();
}

void Player::Idle() {
  if (stateID != playerState::IDLE) {
    animationFrame = 0;
    stateID = playerState::IDLE;
  }
}

void Player::Walk(Vector2 dir, float deltaTime) {
  float speed = conf::PLAYER_MOVE_SPEED;

  if (stateID != playerState::WALK) {
    animationFrame = 0;
    stateID = playerState::WALK;
  }

  // Normalize diagonal movement
  if (Vector2Length(dir) > 0) {
    dir = Vector2Normalize(dir);
  }

  Vector2 nextPos = {position.x + dir.x * deltaTime * speed,
                     position.y + dir.y * deltaTime * speed};

  float offsetX = dir.x * conf::PLAYER_COLLISION_CHECK_OFFSET;
  float offsetY = dir.y * conf::PLAYER_COLLISION_CHECK_OFFSET;
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
  for (int i = 0; i < playerState::SIZE; i++) {
    for (int j = 0; j < dir::SIZE; j++) {
      animationData[i][j] = {.frameCount = ta::PLAYER_X_MAX,
                             .speed = ta::PLAYER_ANIMATION_SPEED,
                             .loop = true};
    }
  }

  // Walk Specifics
  for (int i = 0; i < dir::SIZE; i++) {
    animationData[playerState::WALK][i].frameCount = ta::PLAYER_WALK_MAX;
  }

  // Idle Specifics
  for (int i = 0; i < dir::SIZE; i++) {
    animationData[playerState::IDLE][i].speed = ta::PLAYER_ANIMATION_SPEED_IDLE;
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
  switch (stateID) {
  case playerState::NULL_ID:
    return "NULL";
  case playerState::IDLE:
    return "IDLE";
  case playerState::WALK:
    return "WALK";
  default:
    return "Unknown State";
  }
}

const char *Player::PlayerDirToString() const {
  switch (dirID) {
  case dir::NULL_ID:
    return "NULL";
  case dir::NW:
    return "NW";
  case dir::W:
    return "W";
  case dir::SW:
    return "SW";
  case dir::S:
    return "S";
  case dir::SE:
    return "SE";
  case dir::E:
    return "E";
  case dir::NE:
    return "NE";
  case dir::N:
    return "N";
  default:
    return "Unknown Direction";
  }
}

// --- Rendering ---
void Player::GenerateDrawData() {
  Vector2 drawPos = position;
  drawPos.x -= ta::RES16;
  drawPos.y -= ta::RES16 - conf::PLAYER_SPRITE_Y_OFFSET;

  float resolution = ta::RES32;

  int TA_X = animationFrame + ta::PLAYER_X;
  int TA_Y = (stateID - 1) * (dir::SIZE - 1) + dirID;

  Rectangle dstRect = {drawPos.x, drawPos.y, resolution, resolution};

  graphicsManager->LoadGFX_Data(drawMask::ON_GROUND, drawPos.y, TA_X, TA_Y,
                                dstRect, WHITE);
}
