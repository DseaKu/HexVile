#ifndef ENUMS_H
#define ENUMS_H

enum class TileID {
  TILE_NULL = 0,
  TILE_VOID,
  TILE_GRASS,
  TILE_WATER,
};

typedef enum PlayerStateID {
  PLAYER_STATE_NULL = 0,
  PLAYER_STATE_IDLE,
  PLAYER_STATE_WALK,
  PLAYER_STATE_ID_LENGTH,
} PlayerStateID;

#endif // !ENUMS_H
