#ifndef DEFINES_H
#define DEFINES_H
#include "raylib.h"
#include <cstdint>

namespace Config {
constexpr int SCREEN_WIDTH = 2200;
constexpr int SCREEN_HEIGHT = 1000;
constexpr Vector2 SCREEN_CENTER = {(float)SCREEN_WIDTH / 2,
                                   (float)SCREEN_HEIGHT / 2};

constexpr int MAP_SIZE = 8;
constexpr int TILE_SIZE = 32;
constexpr int TILE_SIZE_HALF = TILE_SIZE / 2;

constexpr float TILE_GAP_X = 18.0f;
constexpr float TILE_GAP_Y = 11.0f;

constexpr Vector2 SCREEN_CENTER_FOR_HEX_TILES = {
    SCREEN_CENTER.x - (float)TILE_SIZE_HALF,
    SCREEN_CENTER.y - (float)TILE_SIZE_HALF};

constexpr float CAMERA_ZOOM = 2.0f;

constexpr bool DEBUGGER_FLAG = true;
} // namespace Config

// --- Unsigned Integer Aliases ---
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// --- Signed Integer Aliases ---
using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

#endif // !DEFINES_H
