#ifndef ENUMS_H
#define ENUMS_H

typedef enum TileID {
  TILE_NULL = 0,
  TILE_VOID,
  TILE_GRASS,
  TILE_WATER,
} TileID;

typedef enum PlayerStateID {
  PLAYER_STATE_NULL = 0,
  PLAYER_STATE_IDLE,
  PLAYER_STATE_WALK,
  PLAYER_STATE_ID_LENGTH,
} PlayerStateID;

typedef enum DirLabels {
  NW = 0,
  W,
  SW,
  S,
  SE,
  E,
  NE,
  N,
  DIR_LABELS_LENGTH,
} DirLabels;

#endif // !ENUMS_H
