#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "defines.h"
#include "enums.h"
#include <map>

// ==========================================
//               32x32 Resolution
// ==========================================
namespace ta {
constexpr int RES32_OFFSET = 100;
constexpr int RES32 = conf::TILE_RESOLUTION;
constexpr float RES32_F = static_cast<float>(RES32);

// --- Player ---
constexpr int PLAYER_X = 0;
constexpr int PLAYER_X_MAX = PLAYER_X + 12;

// --- Tile ---
constexpr int TILE_X = PLAYER_X_MAX;
constexpr int TILES_X_MAX = TILE_X + 7;

// --- Item ---
constexpr int ITEM_X = TILES_X_MAX;
constexpr int ITEM_X_MAX = ITEM_X + 1;

// --- Details ---
constexpr int DETAILS_X = ITEM_X_MAX;
constexpr int DETAILS_X_MAX = DETAILS_X + 12;

// --- Resource ---
constexpr int RESOURCE_X = DETAILS_X_MAX;
constexpr int RESOURCE_X_MAX = RESOURCE_X + 8;

// --- UI ---
constexpr int UI_X = 50;
constexpr int UI_X_MAX = UI_X + 1;

// --- Number ---
constexpr int NUMBER_X = UI_X_MAX;
constexpr int NUMBER_X_MAX = 1;

// ==========================================
//               16x16 Resolution
// ==========================================
constexpr int RES16 = RES32 / 2;
constexpr float RES16_F = static_cast<float>(RES16);

// ==========================================
//               8x8 Resolution
// ==========================================
constexpr int RES8 = RES16 / 2;
constexpr float RES8_F = static_cast<float>(RES8);

// ==========================================
//               Details
// ==========================================
constexpr int GRASS_DETAILS =
    BIT(detail::IDX_0) | BIT(detail::IDX_1) | BIT(detail::IDX_10);
constexpr int WATER_DETAILS = BIT(detail::IDX_8);
constexpr int DIRT_DETAILS = BIT(detail::IDX_10);

inline const std::map<tile::id, int> RENDER_BIT_MASK_DETAIL = {
    {tile::NULL_ID, 0},
    {tile::GRASS, GRASS_DETAILS},
    {tile::WATER, WATER_DETAILS},
    {tile::DIRT, DIRT_DETAILS},
};

// ==========================================
//               Resource
// ==========================================
constexpr int GRASS_RESOURCE = BIT(res::TREE);
constexpr int WATER_RESOURCE = 0;
constexpr int DIRT_RESOURCE = 0;

inline const std::map<tile::id, int> RENDER_BIT_MASK_RESOURCE = {
    {tile::NULL_ID, 0},
    {tile::GRASS, GRASS_RESOURCE},
    {tile::WATER, WATER_RESOURCE},
    {tile::DIRT, DIRT_RESOURCE},
};

// ==========================================
//               Animation
// ==========================================

// --- Animation ---
constexpr float PLAYER_ANIMATION_SPEED = 10.0f;
constexpr float PLAYER_ANIMATION_SPEED_IDLE = 5.0f;
constexpr int PLAYER_WALK_MAX = 8;

constexpr float TILES_ANIMATION_SPEED = 0.3f;

// Don't go to low, it can corruped the font
constexpr float NUMBER_SCALE = RES32 * 0.5f;
} // namespace ta

#endif // TEXTURE_ATLAS_H
