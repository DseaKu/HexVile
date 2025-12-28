#include "player.h"
#include "animation.h"
#include "defines.h"
#include "enums.h"
#include "frame_context.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "raylib.h"
#include "raymath.h"
#include "texture_atlas.h"
#include "ui_handler.h"
#include <pthread.h>

Player::Player() {
  position = conf::SCREEN_CENTER;
  previousPosition = position;
  speedTilesPerSecond = 0.0f;
  faceDirID = faceDir::S;
  stateID = playerState::IDLE;
  animationFrame = 0;
  animationDelta = 0.0f;
  damageAccumulator = 0.0f;
  playerTile = {0, 0};
}

// --- Logic ---
void Player::UpdatePlayerState() {
  bool isInteracting = false;

  // Interact with enviorment
  if (frameContext->mouseMask == mouseMask::GROUND &&
      frameContext->inputs.mouseDown.left) {
    isInteracting = true;
    int selToolBarSlot = frameContext->selToolBarSlot;
    HexCoord hoveredTile =
        hexGrid->PointToHexCoord(frameContext->pos.mouseWorld);
    ItemStack *selectedItem =
        itemHandler->GetToolBarItemPointer(selToolBarSlot);

    // Process tool interaction
    if (selectedItem->itemID == item::AXE) {
      Vector2 clickedPos = hexGrid->HexCoordToPoint(hoveredTile);
      if (Vector2Distance(this->position, clickedPos) <
          conf::INTERACT_DISTANCE_PLAYER) {
        Chop(hoveredTile);
      }

      // Process tile planter
    } else {
      tile::id tileToPlace =
          itemHandler->ConvertItemToTileID(selectedItem->itemID);
      if (tileToPlace != tile::NULL_ID &&
          hexGrid->SetTile(hoveredTile, tileToPlace)) {
        itemHandler->TakeItemFromToolBar(selectedItem, 1);
      }
    }
  }

  if (!isInteracting) {
    if (moveDir.x != 0 || moveDir.y != 0) {
      Walk(moveDir, frameContext->deltaTime);
    } else {
      Idle();
    }
  }
}
void Player::FaceToPoint(Vector2 point) {

  Vector2 diff = Vector2Subtract(point, this->position);
  float angle = atan2(diff.y, diff.x) * RAD2DEG;
  if (angle < 0) {
    angle += 360;
  }
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
void Player::UpdatePlayerFaceDir() {

  if (frameContext->inputs.mouseDown.left ||
      frameContext->inputs.mouseDown.right) {
    Vector2 tarPos = frameContext->pos.mouseWorld;
    this->FaceToPoint(tarPos);
  } else {

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
}

void Player::Update() {
  animationDelta += frameContext->deltaTime;

  playerTile = hexGrid->PointToHexCoord(position);

  Vector2 dir = {0, 0};
  moveDir.x =
      -frameContext->inputs.keyPress.left + frameContext->inputs.keyPress.right;
  moveDir.y =
      -frameContext->inputs.keyPress.up + frameContext->inputs.keyPress.down;

  UpdatePlayerFaceDir();

  UpdatePlayerState();

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
    animationDelta += this->frameContext->deltaTime;
    damageAccumulator += conf::DMG_STONE_AXE * this->frameContext->deltaTime;

    if (damageAccumulator >= 1.0f) {
      int dmg = (int)damageAccumulator;
      damageAccumulator -= dmg;

      if (hexGrid->DamageResource(target, rsrc::ID_TREE, dmg)) {
        itemHandler->AddItem(item::WOOD, 1);
      }
    }
  } else {
    stateID = playerState::CHOP;
    animationFrame = 0;
    animationDelta = 0.0f;
    damageAccumulator = 0.0f;
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

void Player::SetFrameContext(const frame::Context *frameContext) {
  this->frameContext = frameContext;
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

  animation::Object aniData = animation::playerLut.at(this->stateID);

  // Calculate animation frame
  float animSpeed = aniData.speed;
  int frameCount = aniData.frameCount;
  int animationProgress = this->animationDelta * animSpeed;
  this->animationFrame = animationProgress % frameCount;

  faceDir::id dirID = this->faceDirID;

  // The Chop animation in the sprite sheet is offset by 1 position (Starts at N
  // instead of NW)
  int dirIndex = (int)dirID;
  if (this->stateID == playerState::CHOP) {
    dirIndex = (dirIndex + 1) % 8;
  }

  // Get texture atlas position
  int taX = aniData.texAtlas.x + this->animationFrame;
  int taY = aniData.texAtlas.y + dirIndex;

  // Get destination position
  Vector2 drawPos = position;
  drawPos.x -= tex_atlas::RES16;
  drawPos.y -= tex_atlas::RES16 -
               conf::PLAYER_SPRITE_Y_OFFSET; // Move player sprite a little bit
                                             // to the bottom
  Rectangle dstRect = {drawPos.x, drawPos.y, tex_atlas::RES32_F,
                       tex_atlas::RES32_F};

  // Load texture to renderer
  graphicsManager->LoadGFX_Data(drawMask::ON_GROUND, {taX, taY}, dstRect,
                                WHITE);
}
