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
  faceDirID = faceDir::S;
  stateID = playerState::IDLE;
  animationFrame = 0;
  animationDelta = 0.0f;
  playerTile = {0, 0};

  InitAnimations();
}

// --- Logic ---
void Player::UpdatePlayerState(const float deltaTime) {

  if (moveDir.x == 0 && moveDir.y == 0) {
    Idle();
  } else {
    Walk(moveDir, deltaTime);
  }
}

void Player::UpdatePlayerFaceDir() {

  // Determine player face direction
  if (moveDir.x != 0 || moveDir.y != 0) {
    if (moveDir.x == 0 && moveDir.y == -1) {
      faceDirID = faceDir::N;
    } else if (moveDir.x == 1 && moveDir.y == -1) {
      faceDirID = faceDir::NE;
    } else if (moveDir.x == 1 && moveDir.y == 0) {
      faceDirID = faceDir::E;
    } else if (moveDir.x == 1 && moveDir.y == 1) {
      faceDirID = faceDir::SE;
    } else if (moveDir.x == 0 && moveDir.y == 1) {
      faceDirID = faceDir::S;
    } else if (moveDir.x == -1 && moveDir.y == 1) {
      faceDirID = faceDir::SW;
    } else if (moveDir.x == -1 && moveDir.y == 0) {
      faceDirID = faceDir::W;
    } else if (moveDir.x == -1 && moveDir.y == -1) {
      faceDirID = faceDir::NW;
    }
  }
}

void Player::Update(const KeyboardInput *keyboardInput, const float deltaTime) {
  animationDelta += deltaTime;

  playerTile = hexGrid->PointToHexCoord(position);

  Vector2 dir = {0, 0};
  moveDir.x = -keyboardInput->Left + keyboardInput->Right;
  moveDir.y = -keyboardInput->Up + keyboardInput->Down;

  UpdatePlayerFaceDir();

  UpdatePlayerState(deltaTime);

  // Calculate move Speed
  float distance = Vector2Distance(position, previousPosition);
  if (deltaTime > 0) {
    moveSpeed = distance / deltaTime / conf::TILE_RESOLUTION;
  } else {
    moveSpeed = 0;
  }
  previousPosition = position;

  GenerateDrawData();
}

void Player::Chop(HexCoord target) {
  if (stateID == playerState::CHOP) {
    return;
  }

  stateID = playerState::CHOP;
  animationFrame = 0;
  animationDelta = 0.0f;

  // Face the target
  Vector2 targetPos = hexGrid->HexCoordToPoint(target);
  Vector2 diff = Vector2Subtract(targetPos, position);
  float angle = atan2(diff.y, diff.x) * RAD2DEG;
  if (angle < 0)
    angle += 360;

  // Map angle to 8 directions (approximate)
  // E: 0, SE: 45, S: 90, SW: 135, W: 180, NW: 225, N: 270, NE: 315
  // Offset by 22.5 to center sectors
  int sector = (int)((angle + 22.5f) / 45.0f) % 8;

  switch (sector) {
  case 0:
    faceDirID = faceDir::E;
    break;
  case 1:
    faceDirID = faceDir::SE;
    break;
  case 2:
    faceDirID = faceDir::S;
    break;
  case 3:
    faceDirID = faceDir::SW;
    break;
  case 4:
    faceDirID = faceDir::W;
    break;
  case 5:
    faceDirID = faceDir::NW;
    break;
  case 6:
    faceDirID = faceDir::N;
    break;
  case 7:
    faceDirID = faceDir::NE;
    break;
  }
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

  // Check if destination tile is walkable
  if (hexGrid->IsWalkable(hexGrid->PointToHexCoord(checkPos))) {
    // Check for collision with obstacles (e.g., trees)
    if (!hexGrid->CheckObstacleCollision(checkPos,
                                         conf::PLAYER_COLLISION_CHECK_OFFSET)) {
      position = nextPos;
    } else {
      Idle();
    }
  } else {
    // Idle if blocked
    Idle();
  }
}

void Player::InitAnimations() {
  // Default init
  for (int i = 0; i < playerState::SIZE; i++) {
    for (int j = 0; j < faceDir::SIZE; j++) {
      animationData[i][j] = {.frameCount = ta::PLAYER_X_MAX,
                             .speed = ta::PLAYER_ANIMATION_SPEED,
                             .loop = true};
    }
  }

  // Walk Specifics
  for (int i = 0; i < faceDir::SIZE; i++) {
    animationData[playerState::WALK][i].frameCount = ta::PLAYER_WALK_MAX;
  }

  // Idle Specifics
  for (int i = 0; i < faceDir::SIZE; i++) {
    animationData[playerState::IDLE][i].speed = ta::PLAYER_ANIMATION_SPEED_IDLE;
  }

  // Chop Specifics
  for (int i = 0; i < faceDir::SIZE; i++) {
    animationData[playerState::CHOP][i].frameCount =
        ta::PLAYER_WALK_MAX; // Use walk frames for now
    animationData[playerState::CHOP][i].speed =
        ta::PLAYER_ANIMATION_SPEED * 1.5f;
    animationData[playerState::CHOP][i].loop = false;
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

float Player::GetSpeedTilesPerSecond() const { return moveSpeed; }

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
  switch (faceDirID) {
  case faceDir::UNDEFINED:
    return "UNDEFINED";
  case faceDir::NW:
    return "NW";
  case faceDir::W:
    return "W";
  case faceDir::SW:
    return "SW";
  case faceDir::S:
    return "S";
  case faceDir::SE:
    return "SE";
  case faceDir::E:
    return "E";
  case faceDir::NE:
    return "NE";
  case faceDir::N:
    return "N";
  default:
    return "Unknown Direction";
  }
}

// --- Rendering ---
void Player::GenerateDrawData() {

  animationProperties aD = ad::playerLUT.at(this->stateID);

  // Calculate animation frame
  float currentSpeed = aD.speed;

  float frameCount = (float)animationData[stateID][faceDirID].frameCount;
  float frameCount = (float)animationData[stateID][faceDirID].frameCount;

  float animationProgress = animationDelta * currentSpeed;

  if (animationData[stateID][faceDirID].loop) {
    animationFrame = (int)animationProgress % (int)frameCount;
  } else {
    animationFrame = (int)animationProgress;
    if (animationFrame >= frameCount) {
      animationFrame = (int)frameCount - 1;
      if (stateID == playerState::CHOP) {
        Idle();
      }
    }
  }

  // Get texture atlas position
  int taX = aD.x + this->animationFrame;
  int taY = aD.y + this->faceDirID;

  // Get destination position
  Vector2 drawPos = position;
  drawPos.x -= ta::RES16;
  drawPos.y -=
      ta::RES16 - conf::PLAYER_SPRITE_Y_OFFSET; // Player position shouldn't be
                                                // below the sprite
  Rectangle dstRect = {drawPos.x, drawPos.y, ta::RES32_F, ta::RES32_F};

  // Load texture to renderer
  graphicsManager->LoadGFX_Data(drawMask::ON_GROUND, taX, taY, dstRect, WHITE);
}
