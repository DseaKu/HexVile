#ifndef PLAYER_H
#define PLAYER_H

#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "raylib.h"
#include "ui_handler.h"

struct PlayerAnimationData {
  int frameCount;
  float speed;
  bool loop;
};

class Player {
private:
  // --- Dependencies ---
  GFX_Manager *graphicsManager = nullptr;
  HexGrid *hexGrid = nullptr;
  UI_Handler *uiHandler = nullptr;
  ItemHandler *itemHandler = nullptr;

  // --- State ---
  Vector2 position;
  Vector2 previousPosition;
  HexCoord playerTile;
  playerState::id stateID;
  faceDir::id faceDirID;
  Vector2 moveDir;
  float moveSpeed;
  float speedTilesPerSecond;

  // --- Logic ---
  void UpdatePlayerState(const FrameContext *frameContext);
  void UpdatePlayerFaceDir();

  // --- Animation ---
  int animationFrame;
  float animationDelta;
  PlayerAnimationData animationData[playerState::SIZE][faceDir::SIZE];

  // --- Helpers ---
  void Idle();
  void Walk(Vector2 dir, float deltaTime);
  void InitAnimations();
  void GenerateDrawData();

public:
  Player();

  // --- Logic ---
  void Update(const FrameContext *frameContext);
  void Chop(HexCoord target);

  // --- Setters ---
  void SetHexGrid(HexGrid *grid);
  void SetGFX_Manager(GFX_Manager *graphicsManager);
  void SetUI_Handler(UI_Handler *ui_handler);
  void SetItemHandler(ItemHandler *itemHandler);

  // --- Getters ---
  Vector2 GetPosition() const;
  HexCoord GetTile() const;
  int GetAnimationFrame() const;
  float GetSpeedTilesPerSecond() const;
  const char *PlayerStateToString() const;
  const char *PlayerDirToString() const;
};
#endif // !PLAYER_H
