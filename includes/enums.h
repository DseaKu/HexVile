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
  PLAYER_STATE_WALK,
  PLAYER_STATE_IDLE,
  PLAYER_STATE_ID_LENGTH,
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
  DIR_LABELS_LENGTH,
} DirLabels;

typedef enum ItemID {
  ITEM_NULL = 0,
  ITEM_SET_TILE_GRASS,
  ITEM_SET_TILE_WATER,
  ItemsIDLength,
} ItemID;

typedef enum ItemPosition {
  ITEM_POSITION_NULL = 0,
  ITEM_POSTIION_TOOL_BAR,
  ITEM_POSTIION_INVENTORY,
  ITEM_POSITION_CHEST,
  ITEM_POSITION_LENGTH,
} ItemPosition;

typedef enum MouseMask {
  MOUSE_MASK_NULL = 0,
  MOUSE_MASK_PLAY_GROUND,
  MOUSE_MASK_ITEM_BAR,
} MouseMask;

#endif // !ENUMS_H
