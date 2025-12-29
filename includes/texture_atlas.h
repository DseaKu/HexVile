#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "enums.h"
#include <map>

// ==========================================
//               Texture atlas
// ==========================================
namespace tex_atlas {
struct Coords {
  int x;
  int y;
};

// --- Resolution ---
constexpr int RES32 = 32u;
constexpr float RES32_F = static_cast<float>(RES32);
constexpr int RES64 = 64u;
constexpr float RES64_F = static_cast<float>(RES64);
constexpr int RES16 = RES32 / 2;
constexpr float RES16_F = static_cast<float>(RES16);
constexpr int RES8 = RES16 / 2;
constexpr float RES8_F = static_cast<float>(RES8);

// ==========================================
//               Positioning
// ==========================================
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
constexpr Coords RSRC = {DETAILS_X_MAX, 0};
constexpr Coords RSRC_TREE = {RSRC.x, RSRC.y};
constexpr Coords RSRC_STONE = {RSRC.x, RSRC.y + 1};
constexpr int RESOURCE_X = DETAILS_X_MAX;
constexpr int RESOURCE_Y = 0;
constexpr int RESOURCE_X_MAX = RESOURCE_X + 8;

// --- UI ---
constexpr int UI_X = 50;
constexpr int UI_X_MAX = UI_X + 1;

constexpr Coords INVENTORY_BACKGROUND_TL = {52, 0};
constexpr Coords INVENTORY_BACKGROUND_BR = {63, 7};

// --- Number ---
constexpr int NUMBER_X = UI_X_MAX;
constexpr int NUMBER_X_MAX = 1;

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

// --- Number ---
constexpr float NUMBER_SCALE =
    RES32 * 0.5f; // Don't go to low, it can corruped the font
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
} // namespace tex_atlas

#endif // TEXTURE_ATLAS_H
