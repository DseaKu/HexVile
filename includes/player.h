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
  int animationFrame;
  float animationDelta;
  DirLabels faceDir;
  Vector2 position;
  Vector2 previousPosition;
  float speedTilesPerSecond;
  GFX_Manager *graphicsManager;
  PlayerStateID state;
  PlayerAnimationData animationData[PLAYER_STATE_ID_SIZE][DIR_LABELS_SIZE];
  HexGrid *hexGrid;
  HexCoord playerTile;

  void Idle();
  void Walk(Vector2 dir, float deltaTime);
  void InitAnimations();
  void GenerateDrawData();

public:
  Player();
  void Update(const KeyboardInput *keyboardInput, float deltaTime);
  void SetHexGrid(HexGrid *grid);
  Vector2 GetPosition();
  void SetGFX_Manager(GFX_Manager *graphicsManager);
  int GetAnimationFrame();
  HexCoord GetTile();
  const char *PlayerStateToString();
  const char *PlayerDirToString();
  float GetSpeedTilesPerSecond();
};
#endif // !PLAYER_H
