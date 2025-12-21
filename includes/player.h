#ifndef PLAYER_H
#define PLAYER_H

#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"

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

  // --- State ---
  Vector2 position;
  Vector2 previousPosition;
  HexCoord playerTile;
  playerState::id stateID;
  dir::id dirID;
  float speedTilesPerSecond;

  // --- Animation ---
  int animationFrame;
  float animationDelta;
  PlayerAnimationData animationData[playerState::SIZE][dir::SIZE];

  // --- Helpers ---
  void Idle();
  void Walk(Vector2 dir, float deltaTime);
  void InitAnimations();
  void GenerateDrawData();

public:
  Player();

  // --- Core ---
  void Update(const KeyboardInput *keyboardInput, float deltaTime);
  void Chop(HexCoord target);

  // --- Setters ---
  void SetHexGrid(HexGrid *grid);
  void SetGFX_Manager(GFX_Manager *graphicsManager);

  // --- Getters ---
  Vector2 GetPosition() const;
  HexCoord GetTile() const;
  int GetAnimationFrame() const;
  float GetSpeedTilesPerSecond() const;
  const char *PlayerStateToString() const;
  const char *PlayerDirToString() const;
};
#endif // !PLAYER_H
