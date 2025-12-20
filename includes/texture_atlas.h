#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "defines.h"
#include "enums.h"
#include <map>

// ==========================================
//               32x32 Resolution
// ==========================================
namespace TA {
constexpr int RES32_OFFSET = 100;
constexpr int RES = Conf::TILE_RESOLUTION;
constexpr float RES_F = RES;

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

// --- UI ---
constexpr int UI_X = 50;
constexpr int UI_X_MAX = UI_X + 1;

// --- Number ---
constexpr int NUMBER_X = UI_X_MAX;
constexpr int NUMBER_X_MAX = 1;

// ==========================================
//               16x16 Resolution
// ==========================================
constexpr int RES16 = RES / 2;
constexpr float RES16_F = RES16;

// ==========================================
//               8x8 Resolution
// ==========================================
constexpr int RES8 = RES16 / 2;
constexpr float RES8_F = RES8;

// ==========================================
//               Details & Objects
// ==========================================
constexpr int SKIP_RENDER = -1;
constexpr int UNINITIALIZED = -2;

// --- Detail Masks ---
constexpr int GRASS_DETAILS =
    BIT(detail::IDX_0) | BIT(detail::IDX_10) | BIT(detail::IDX_11);

// Original: 0b00000000100 (Bit 2)
constexpr int WATER_DETAILS = BIT(detail::IDX_2);

// Original: 0b00100000010 (Bits 1, 9) -> Wait, 0b00100000010 is Bit 1 and Bit
// 9? 0b00100000010
//   ^ Bit 9   ^ Bit 1
constexpr int DIRT_DETAILS = BIT(detail::IDX_1) | BIT(detail::IDX_9);

inline const std::map<tile::id, int> RENDER_BIT_MASK_DETAIL = {
    {tile::NULL_ID, 0},
    {tile::GRASS, GRASS_DETAILS},
    {tile::WATER, WATER_DETAILS},
    {tile::DIRT, DIRT_DETAILS},
};

// --- Object Indices ---
enum ObjectIndex {
  OBJ_IDX_5 = 5 // Bit 5
};

// --- Object Masks ---
// Original: 0b00000100000 (Bit 5)
constexpr int GRASS_OBJECTS = BIT(OBJ_IDX_5);
constexpr int WATER_OBJECTS = 0;
constexpr int DIRT_OBJECTS = 0;

inline const std::map<tile::id, int> RENDER_BIT_MASK_OBJECT = {
    {tile::NULL_ID, 0},
    {tile::GRASS, GRASS_OBJECTS},
    {tile::WATER, WATER_OBJECTS},
    {tile::DIRT, DIRT_OBJECTS},
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
constexpr float NUMBER_SCALE = RES * 0.5f;
} // namespace TA

#endif // TEXTURE_ATLAS_H
