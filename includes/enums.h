#ifndef ENUMS_H
#define ENUMS_H
#include <map>

// --- Tile ---
namespace tile {
enum id {
  NULL_ID = 0,
  GRASS,
  WATER,
  DIRT,
  SIZE,
};
}

// --- Detail Indices ---
namespace detail {
enum id {
  IDX_0 = 0,
  IDX_1 = 1,
  IDX_2 = 2,
  IDX_3 = 3,
  IDX_4 = 4,
  IDX_5 = 5,
  IDX_6 = 6,
  IDX_7 = 7,
  IDX_8 = 8,
  IDX_9 = 9,
  IDX_10 = 10,
  IDX_11 = 11
};
}

// --- Player State ---
namespace playerState {
enum id {
  NULL_ID = 0,
  WALK,
  IDLE,
  CHOP,
  SIZE,
};
}

// --- Directions ---
namespace faceDir {
enum id : int {
  UNDEFINED = -1,
  NW = 0,
  W,
  SW,
  S,
  SE,
  E,
  NE,
  N,
  SIZE,
};
}

// --- Items ---
namespace item {
enum id {
  NULL_ID = 0,
  SET_GRASS,
  SET_WATER,
  SET_DIRT,
  AXE,
  WOOD,
  SIZE,
};
}

// --- Item Positions ---
namespace itemPos {
enum id {
  NULL_ID = 0,
  TOOL_BAR,
  INVENTORY,
  CHEST,
  SIZE,
};
}

// --- Mouse ---
namespace mouseMask {
enum id {
  NULL_ID = 0,
  GROUND,
  TOOL_BAR,
  SIZE,
};
}

// --- Draw Masks ---
namespace drawMask {
enum id {
  NULL_ID = 0,
  GROUND0, // Tiles
  GROUND1, // Highlighted Tiles
  SHADOW,
  ON_GROUND, // Player, Terrain Details, Resource
  UI_0,      // Toolbar, Item BG
  UI_1,      // Items, ItemsNum
  DEBUG_OVERLAY,
  SIZE,
};
}

// --- UI IDs ---
namespace ui {
enum id {
  NULL_ID = 0,
  TILE_H,
  ITEM_BAR_BG,
  ITEM_BAR_BG_H,
  SIZE,
};
}

// --- Mappings ---
static const std::map<item::id, tile::id> item_to_tile_map = {
    {item::SET_GRASS, tile::GRASS},
    {item::SET_WATER, tile::WATER},
    {item::SET_DIRT, tile::DIRT},
};

#endif // !ENUMS_H
