#ifndef TEXTURE_H
#define TEXTURE_H

#include "enums.h"
#include "raylib.h"
#include <map>

namespace tex {

struct Opts {
  Color color = WHITE;
  bool useHitShader = false;

  float srcWidth = 0.0f;
  float srcHeight = 0.0f;
  float dstWidth = 0.0f;
  float dstHeight = 0.0f;
  float dstRecY = 0.0f;

  float scale = 1.0f;

  float sortingOffsetY = 0.0f;

  Vector2 origin = {0.5f, 1.0f};
  bool ignoreRelativeOrigin = false;
};

// ==========================================
//               Size
// ==========================================
namespace size {
// --- Resolution ---
constexpr float TILE = 32.0f;
constexpr float DOUBLE_TILE = TILE * 2;
constexpr float HALF_TILE = TILE / 2;
constexpr float QUATER_TILE = HALF_TILE / 2;
} // namespace size

// ==========================================
//               Texture Atlas Coordinates (source)
// ==========================================

namespace atlas {
struct Coords {
  int x, y;
  float width = 1;
  float height = 1;
};
// --- y = 0 ---
// --- Player ---
constexpr int PLAYER_X = 0;
constexpr int PLAYER_Y = 0;
constexpr Coords PLAYER_WALK = {PLAYER_X, PLAYER_Y};
constexpr Coords PLAYER_IDLE = {PLAYER_X, PLAYER_Y + 8};
constexpr Coords PLAYER_ATTACK = {PLAYER_X, PLAYER_Y + 16};
constexpr int PLAYER_X_MAX = PLAYER_X + 12;

// --- Tile ---
constexpr int TILE_X = PLAYER_X_MAX;
constexpr int TILES_X_MAX = TILE_X + 8;

// --- Details ---
constexpr int DETAILS_X = TILES_X_MAX;
constexpr int DETAILS_X_MAX = DETAILS_X + 12;

// --- Resource ---
constexpr Coords TREE = {32, 0, 1, 2};
constexpr Coords STONE = {32, 1};
constexpr int RESOURCE_X = DETAILS_X_MAX;
constexpr int RESOURCE_Y = 0;
constexpr int RESOURCE_X_MAX = RESOURCE_X + 8;

// --- UI ---
constexpr Coords ITEM_SLOT_BACKGROUND = {56, 1};
constexpr Coords ITEM_SLOT_BACKGROUND_HIGHLIGHTED = {57, 1};
constexpr Coords TILE_HIGHLIGHTED = {58, 1};
constexpr Coords INVENTORY = {52, 1, 4, 3};

// --- Number ---
constexpr Coords NUMBER = {52, 0};

// --- y = 50 ----
// --- Item ---
constexpr int ITEM_X = 0;
constexpr int ITEM_Y = 50;
constexpr int ITEM_X_MAX = 10;

// --- Null ---
constexpr Coords POS_NULL = {0, 0};

// --- Animation ---
constexpr float PLAYER_ANIMATION_SPEED = 10.0f;
constexpr float PLAYER_ANIMATION_SPEED_IDLE = 5.0f;
constexpr int PLAYER_WALK_MAX = 8;
constexpr float TILES_ANIMATION_SPEED = 0.3f;

//
// --- Items ---
constexpr Coords ITEM_NULL = {0, 0};
constexpr Coords ITEM_SET_GRASS = {ITEM_X + 0, ITEM_Y + 0};
constexpr Coords ITEM_SET_WATER = {ITEM_X + 1, ITEM_Y + 0};
constexpr Coords ITEM_SET_DIRT = {ITEM_X + 2, ITEM_Y + 0};

constexpr Coords ITEM_AXE_STONE = {ITEM_X + 0, ITEM_Y + 1};
constexpr Coords ITEM_WOOD_LOG = {ITEM_X + 1, ITEM_Y + 1};

inline const std::map<item::id, Coords> ITEM_TEXTURE_COORDS = {
    {item::NULL_ID, ITEM_NULL},        {item::SET_GRASS, ITEM_SET_GRASS},
    {item::SET_WATER, ITEM_SET_WATER}, {item::SET_DIRT, ITEM_SET_DIRT},
    {item::AXE, ITEM_AXE_STONE},       {item::WOOD, ITEM_WOOD_LOG},
};
} // namespace atlas
// ==========================================
//              Layout Options (destination)
// ==========================================

namespace opts {

// --- Number ---
// Don't go to low with the scale, it corruped the font. 0.5f good result.
constexpr Opts NUMBERS = {.scale = 0.5f, .origin = {0, 0}};

// --- UI ---
constexpr Opts ITEM_SLOT_BACKGROUND = {.origin{0, 0}};

// --- Invetory ---
constexpr Opts IVENTORY = {.scale = 6.0f, .origin = {0.475, 0.5}};
} // namespace opts

} // namespace tex

#endif // TEXTURE_H
