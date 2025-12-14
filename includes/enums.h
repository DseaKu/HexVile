#ifndef ENUMS_H
#define ENUMS_H

typedef enum TileID {
  TILE_NULL = 0,
  TILE_GRASS,
  TILE_WATER,
  TILE_DIRT,
  TILE_ID_SIZE,
} TileID;

typedef enum PlayerStateID {
  PLAYER_STATE_NULL = 0,
  PLAYER_STATE_WALK,
  PLAYER_STATE_IDLE,
  PLAYER_STATE_ID_SIZE,
} PlayerStateID;

typedef enum DirLabels {
  DIR_NULL = 0,
  NW,
  W,
  SW,
  S,
  SE,
  E,
  NE,
  N,
  DIR_LABELS_SIZE,
} DirLabels;

typedef enum ItemID {
  ITEM_NULL = 0,
  ITEM_SET_GRASS,
  ITEM_SET_WATER,
  ITEM_SET_DIRT,
  ITEM_ID_SIZE,
} ItemID;

typedef enum ItemPosition {
  ITEM_POSITION_NULL = 0,
  ITEM_POSTIION_TOOL_BAR,
  ITEM_POSTIION_INVENTORY,
  ITEM_POSITION_CHEST,
  ITEM_POSITION_SIZE,
} ItemPosition;

typedef enum MouseMask {
  MOUSE_MASK_NULL = 0,
  MOUSE_MASK_PLAY_GROUND,
  MOUSE_MASK_ITEM_BAR,
  MOUSE_MASK_SIZE,
} MouseMask;

#endif // !ENUMS_H
