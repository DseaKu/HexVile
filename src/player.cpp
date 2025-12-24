#include "player.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "raylib.h"
#include "raymath.h"
#include "texture_atlas.h"
#include "ui_handler.h"

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
void Player::UpdatePlayerState(const FrameContext *frameContext) {

  if (moveDir.x == 0 && moveDir.y == 0) {
    Idle();
  } else {
    Walk(moveDir, frameContext->deltaTime);
  }

  // Interact with enviorment
  if (frameContext->mouseMask == mouseMask::GROUND) {
    if (frameContext->inputs.mousePress.left) {
      int selToolBarSlot = uiHandler->GetSelToolBarSlot();
      HexCoord clickedTile =
          hexGrid->PointToHexCoord(frameContext->mouseWorldPos);

      ItemStack *selectedItem =
          itemHandler->GetToolBarItemPointer(selToolBarSlot);

      // Process tool interaction
      if (selectedItem->itemID == item::AXE) {
        Vector2 clickedPos = hexGrid->HexCoordToPoint(clickedTile);

        if (Vector2Distance(this->position, clickedPos) <
            conf::INTERACT_DISTANCE) {
          Chop(clickedTile);
        }
        // Process tile setter
      } else {
        tile::id tileToPlace =
            itemHandler->ConvertItemToTileID(selectedItem->itemID);
        if (tileToPlace != tile::NULL_ID &&
            hexGrid->SetTile(clickedTile, tileToPlace)) {
          itemHandler->TakeItemFromToolBar(selectedItem, 1);
        }
      }
    }
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

void Player::Update(const FrameContext *frameContext) {
  animationDelta += frameContext->deltaTime;

  playerTile = hexGrid->PointToHexCoord(position);

  Vector2 dir = {0, 0};
  moveDir.x =
      -frameContext->inputs.keyPress.Left + frameContext->inputs.keyPress.Right;
  moveDir.y =
      -frameContext->inputs.keyPress.Up + frameContext->inputs.keyPress.Down;

  UpdatePlayerFaceDir();

  UpdatePlayerState(frameContext);

  // Calculate move Speed
  float distance = Vector2Distance(position, previousPosition);
  if (frameContext->deltaTime > 0) {
    moveSpeed = distance / frameContext->deltaTime / conf::TILE_RESOLUTION;
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

  if (hexGrid->RemoveResource(target, rsrc::TREE)) {
    itemHandler->AddItem(item::WOOD, 1);
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
      animationData[i][j] = {.frameCount = tex_atlas::PLAYER_X_MAX,
                             .speed = tex_atlas::PLAYER_ANIMATION_SPEED,
                             .loop = true};
    }
  }

  // Walk Specifics
  for (int i = 0; i < faceDir::SIZE; i++) {
    animationData[playerState::WALK][i].frameCount = tex_atlas::PLAYER_WALK_MAX;
  }

  // Idle Specifics
  for (int i = 0; i < faceDir::SIZE; i++) {
    animationData[playerState::IDLE][i].speed =
        tex_atlas::PLAYER_ANIMATION_SPEED_IDLE;
  }

  // Chop Specifics
  for (int i = 0; i < faceDir::SIZE; i++) {
    animationData[playerState::CHOP][i].frameCount =
        tex_atlas::PLAYER_WALK_MAX; // Use walk frames for now
    animationData[playerState::CHOP][i].speed =
        tex_atlas::PLAYER_ANIMATION_SPEED * 1.5f;
    animationData[playerState::CHOP][i].loop = false;
  }
}

// --- Setters ---
void Player::SetGFX_Manager(GFX_Manager *graphicsManager) {
  this->graphicsManager = graphicsManager;
}

void Player::SetHexGrid(HexGrid *grid) { this->hexGrid = grid; }

void Player::SetUI_Handler(UI_Handler *uiHandler) {
  this->uiHandler = uiHandler;
}

void Player::SetItemHandler(ItemHandler *itemHandler) {
  this->itemHandler = itemHandler;
}
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

  TexData texData = tex_lut::playerLUT.at(this->stateID);

  // Calculate animation frame
  float animSpeed = texData.speed;
  int frameCount = texData.frameCount;
  int animationProgress = this->animationDelta * animSpeed;
  this->animationFrame = animationProgress % frameCount;

  // Get texture atlas position
  int taX = texData.x + this->animationFrame;
  int taY = texData.y + this->faceDirID;

  // Get destination position
  Vector2 drawPos = position;
  drawPos.x -= tex_atlas::RES16;
  drawPos.y -= tex_atlas::RES16 -
               conf::PLAYER_SPRITE_Y_OFFSET; // Player position shouldn't be
                                             // below the sprite
  Rectangle dstRect = {drawPos.x, drawPos.y, tex_atlas::RES32_F,
                       tex_atlas::RES32_F};

  // Load texture to renderer
  graphicsManager->LoadGFX_Data(drawMask::ON_GROUND, taX, taY, dstRect, WHITE);
}
