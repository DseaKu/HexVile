#ifndef DEFINES_H
#define DEFINES_H
#include "raylib.h"
#include <cstdint>

namespace Config {
// --- SCREEN ---
constexpr int SCREEN_WIDTH = 1400;
constexpr int SCREEN_HEIGHT = 800;
constexpr Vector2 SCREEN_CENTER = {(float)SCREEN_WIDTH / 2,
                                   (float)SCREEN_HEIGHT / 2};

// --- ASSEST ---
constexpr int ASSEST_RESOLUTION = 32;
constexpr int ASSEST_RESOLUTION_HALF = ASSEST_RESOLUTION / 2;
constexpr int TEXTURE_ATLAS_ANIMATION = ASSEST_RESOLUTION * 0;
constexpr int TEXTURE_ATLAS_TILES = ASSEST_RESOLUTION * 10;

// --- TILE ---
constexpr int MAP_SIZE = 64;
constexpr int TILE_SIZE = ASSEST_RESOLUTION;
constexpr int TILE_SIZE_HALF = ASSEST_RESOLUTION_HALF;
constexpr float TILE_GAP_X = 18.4f;
constexpr float TILE_GAP_Y = 16.0f;

// --- CAMERA ---
constexpr float CAMERA_ZOOM = 5.0f;
constexpr float CAMERA_WIDTH = (float)SCREEN_WIDTH / CAMERA_ZOOM;
constexpr float CAMERA_HEIGTH = (float)SCREEN_HEIGHT / CAMERA_ZOOM;

// --- GAME ---
constexpr bool DEBUG_FLAG = true;
constexpr int DEBUG_OVERLAY_SECTION_FONT_SIZE = 20;
constexpr int DEBUG_OVERLAY_SECTION_X_POS = 30;
constexpr int DEBUG_OVERLAY_SECTION_Y_POS = 30;
constexpr int DEBUG_OVERLAY_SECTION_Y_GAP = 3;

constexpr int DEBUG_OVERLAY_SUBSECTION_FONT_SIZE = 15;
constexpr int DEBUG_OVERLAY_SUBSECTION_X_GAP = 30;
constexpr int DEBUG_OVERLAY_SUBSECTION_X_POS =
    DEBUG_OVERLAY_SECTION_X_POS + DEBUG_OVERLAY_SUBSECTION_X_GAP;
constexpr int DEBUG_OVERLAY_SUBSECTION_Y_GAP =
    DEBUG_OVERLAY_SUBSECTION_FONT_SIZE + 3;
constexpr int DEBUG_OVERLAY_SUBSECTION_Y_POS =
    DEBUG_OVERLAY_SECTION_Y_POS + DEBUG_OVERLAY_SUBSECTION_Y_GAP;

// --- PLAYER ---
constexpr float PLAYER_SPEED = 50.0f;
constexpr float PLAYER_ANIMATION_SPEED = 10.0f;
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
