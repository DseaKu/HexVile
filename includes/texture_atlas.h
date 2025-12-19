#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "defines.h"
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
constexpr float GAUSIAN_EFFECT = 6.0f;
constexpr int GRASS_DETAILS = 0b110000000011;
constexpr int WATER_DETAILS = 0b000000000100;
constexpr int DIRT_DETAILS = 0b001000000010;
constexpr int SKIP_RENDER = -1;
constexpr int UNINITIALIZED = -2;
inline const std::map<TileID, int> RENDER_BIT_MASK_DETAIL = {
    {TILE_GRASS, GRASS_DETAILS},
    {TILE_WATER, WATER_DETAILS},
    {TILE_DIRT, DIRT_DETAILS},
};

constexpr int GRASS_OBJECTS = 0b000000100000;
constexpr int WATER_OBJECTS = 0b000000000000;
constexpr int DIRT_OBJECTS = 0b000000000000;
inline const std::map<TileID, int> RENDER_BIT_MASK_OBJECT = {
    {TILE_GRASS, GRASS_OBJECTS},
    {TILE_WATER, WATER_OBJECTS},
    {TILE_DIRT, DIRT_OBJECTS},
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
