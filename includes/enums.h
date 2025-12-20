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

// --- Detail Indices  ---
enum DetailIndex {
  DETAIL_IDX_0 = 0,
  DETAIL_IDX_1 = 1,
  DETAIL_IDX_2 = 2,
  DETAIL_IDX_3 = 3,
  DETAIL_IDX_4 = 4,
  DETAIL_IDX_5 = 5,
  DETAIL_IDX_6 = 6,
  DETAIL_IDX_7 = 7,
  DETAIL_IDX_8 = 8,
  DETAIL_IDX_9 = 9,
  DETAIL_IDX_10 = 10,
  DETAIL_IDX_11 = 11
};

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
  DRAW_MASK_GROUND_0, // Tiles,
  DRAW_MASK_GROUND_1, // Highlighted Tiles,
  DRAW_MASK_SHADOW,
  DRAW_MASK_ON_GROUND, // Player, Terrain Details,
  DRAW_MASK_UI_0,      // Toolbar, Item BG,
  DRAW_MASK_UI_1,      // Items, ItemsNum,
  DRAW_MASK_SIZE,
};

enum UI_ID {
  UI_ID_NULL = 0,
  UI_ID_TILE_H,
  UI_ID_ITEM_BAR_BG,
  UI_ID_ITEM_BAR_BG_H,
  UI_ID_SIZE,
};

#endif // !ENUMS_H
