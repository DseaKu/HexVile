#ifndef ENUMS_H
#define ENUMS_H
#include <map>

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

static const std::map<ItemID, TileID> item_to_tile_map = {
    {ITEM_SET_GRASS, TILE_GRASS},
    {ITEM_SET_WATER, TILE_WATER},
    {ITEM_SET_DIRT, TILE_DIRT},
};
enum DrawMaskID {
  DRAW_MASK_NULL = 0,
  DRAW_MASK_GROUND,
  DRAW_MASK_SHADOW,
  DRAW_MASK_ON_GROUND,
  DRAW_MASK_UI,
  DRAW_MASK_SIZE,
};

typedef enum GrassTerainDetailsID {
  GRASS_DETAILS_NULL = 0,
  GRASS_DETAILS_FLOWER,
  GRASS_DETAILS_BLADE0,
  GRASS_DETAILS_BLADE1,
  GRASS_DETAILS_BLADE2,
  GRASS_DETAILS_BLADE3,
  GRASS_DETAILS_SHIT,
  GRASS_DETAILS_SIZE,
} GrassTerainDetailsID;
#endif // !ENUMS_H
