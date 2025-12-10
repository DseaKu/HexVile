#ifndef ENUMS_H
#define ENUMS_H

enum class TileID {
  TILE_NULL = 0,
  TILE_VOID,
  TILE_GRASS,
  TILE_WATER,
};

enum class PlayerStateID {
  PLAYER_STATE_NULL = 0,
  PLAYER_STATE_IDLE,
  PLAYER_STATE_WALK,

};
#endif // !ENUMS_H
