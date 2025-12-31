#ifndef PLAYER_H
#define PLAYER_H

#include "GFX_manager.h"
#include "enums.h"
#include "frame_context.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "raylib.h"
#include "ui_handler.h"

class Player {
private:
  // --- Dependencies ---
  GFX_Manager *graphicsManager = nullptr;
  HexGrid *hexGrid = nullptr;
  UI_Handler *uiHandler = nullptr;
  ItemHandler *itemHandler = nullptr;
  const frame::Context *frameContext = nullptr;

  // --- State ---
  Vector2 position;
  Vector2 previousPosition;
  HexCoord playerTile;
  playerState::id stateID;
  faceDir::id faceDirID;
  Vector2 moveDir;
  float moveSpeed;
  float speedTilesPerSecond;

  // --- Animation ---
  int animationFrame;
  float animationDelta;
  float damageAccumulator;

  // --- Private Methods ---
  void UpdatePlayerState();
  void UpdatePlayerFaceDir();
  void FaceToPoint(Vector2 point);
  void Idle();
  void Walk(Vector2 dir, float deltaTime);

public:
  // --- Constructors ---
  Player();

  // --- Core Lifecycle ---
  void Update();
  void Chop(HexCoord target);

  // --- Graphics / Backbuffer ---
  void LoadBackBuffer();

  // --- Setters ---
  void SetHexGrid(HexGrid *grid);
  void SetGFX_Manager(GFX_Manager *graphicsManager);
  void SetUI_Handler(UI_Handler *ui_handler);
  void SetItemHandler(ItemHandler *itemHandler);
  void SetFrameContext(const frame::Context *frameContext);

  // --- Getters ---
  Vector2 GetPosition() const;
  HexCoord GetTile() const;
  int GetAnimationFrame() const;
  float GetSpeedTilesPerSecond() const;
  const char *PlayerStateToString() const;
  const char *PlayerDirToString() const;
};
#endif // !PLAYER_H
