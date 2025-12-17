#ifndef PLAYER_H
#define PLAYER_H

#include "GFX_manager.h"
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
  GFX_Manger *graphicsManager;
  PlayerStateID state;
  PlayerAnimationData animationData[PLAYER_STATE_ID_SIZE][DIR_LABELS_SIZE];
  HexGrid *hexGrid;
  HexCoord playerTile;

  void Idle();
  void Walk(Vector2 dir);
  void InitAnimations();
  void GenerateDrawData();

public:
  Player();
  void Update();
  void SetHexGrid(HexGrid *grid);
  Vector2 GetPosition();
  void SetGFX_Manger(GFX_Manger *graphicsManager);
  int GetAnimationFrame();
  HexCoord GetTile();
  const char *PlayerStateToString();
  const char *PlayerDirToString();
  float GetSpeedTilesPerSecond();
};
#endif // !PLAYER_H
