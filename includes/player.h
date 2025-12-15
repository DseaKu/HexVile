#ifndef PLAYER_H
#define PLAYER_H

#include "enums.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "texture_handler.h"

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
  TextureHandler *textureHandler;
  PlayerStateID state;
  PlayerAnimationData animationData[PLAYER_STATE_ID_SIZE][DIR_LABELS_SIZE];
  HexGrid *hexGrid;
  HexCoord playerTile;

  void Idle();
  void Walk(Vector2 dir);
  void InitAnimations();

public:
  Player();
  void Update();
  void Draw();
  void SetHexGrid(HexGrid *grid);
  Vector2 GetPosition();
  void SetTextureHandler(TextureHandler *textureHandler);
  int GetAnimationFrame();
  HexCoord GetTile();
  const char *PlayerStateToString();
  const char *PlayerDirToString();
  float GetSpeedTilesPerSecond();
};
#endif // !PLAYER_H
