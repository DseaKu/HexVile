#ifndef PLAYER_H
#define PLAYER_H

#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "graphics_manager.h"

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
  GraphicsManager *graphicsManager;
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
  void SetGraphicsManager(GraphicsManager *graphicsManager);
  int GetAnimationFrame();
  HexCoord GetTile();
  const char *PlayerStateToString();
  const char *PlayerDirToString();
  float GetSpeedTilesPerSecond();
};
#endif // !PLAYER_H
