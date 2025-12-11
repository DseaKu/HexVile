#ifndef DEFINES_H
#define DEFINES_H
#include "raylib.h"
#include <cstdint>

namespace Config {

// --- Game ---
constexpr int MAP_SIZE = 64;
constexpr int FPS_MAX = 1000;
constexpr const char *WINDOW_TITLE = "HexVile";
constexpr const char *TEXTURE_ATLAS_PATH = "assets/images/texture_atlas.png";
constexpr const char *FONT_HACK_REGULAR_PATH = "assets/font/hack_regular.ttf";

// --- Screen ---
// constexpr int SCREEN_WIDTH = 1400;
// constexpr int SCREEN_HEIGHT = 800;
constexpr int SCREEN_WIDTH = 2800;
constexpr int SCREEN_HEIGHT = 1200;
constexpr Vector2 SCREEN_CENTER = {(float)SCREEN_WIDTH / 2,
                                   (float)SCREEN_HEIGHT / 2};

// --- Assest ---
constexpr int ASSEST_RESOLUTION = 32;
constexpr int ASSEST_RESOLUTION_HALF = ASSEST_RESOLUTION / 2;
constexpr int TEXTURE_ATLAS_PLAYER_ANIMATION_X_OFFSET = ASSEST_RESOLUTION * 0;
constexpr int TEXTURE_ATLAS_PLAYER_ANIMATION_FRAME_COUNT_MAX = 12;
constexpr int TEXTURE_ATLAS_PLAYER_ANIMATION_FRAME_COUNT_WALK = 8;
constexpr int TEXTURE_ATLAS_TILES_X_OFFSET =
    ASSEST_RESOLUTION * TEXTURE_ATLAS_PLAYER_ANIMATION_FRAME_COUNT_MAX;

// --- Tile ---
constexpr int TILE_SIZE = ASSEST_RESOLUTION;
constexpr int TILE_SIZE_HALF = ASSEST_RESOLUTION_HALF;
constexpr float TILE_GAP_X = 18.4f;
constexpr float TILE_GAP_Y = 16.0f;

// --- Camera ---
constexpr float CAMERA_ZOOM = 5.0f;
constexpr float CAMERA_WIDTH = (float)SCREEN_WIDTH / CAMERA_ZOOM;
constexpr float CAMERA_HEIGTH = (float)SCREEN_HEIGHT / CAMERA_ZOOM;

// --- Font ---
constexpr int FONT_SIZE_DEFAULT = 20;
constexpr int DEBUG_OVERLAY_SECTION_FONT_SIZE = 20;
constexpr int DEBUG_OVERLAY_SUBSECTION_FONT_SIZE = 15;
constexpr int HACK_REGULAR_GLYPH_COUNT = 95;

// --- Debug ---
constexpr bool DEBUG_FLAG = true;
constexpr int DEBUG_OVERLAY_SECTION_X_POS = 30;
constexpr int DEBUG_OVERLAY_SECTION_Y_POS = 30;
constexpr int DEBUG_OVERLAY_SECTION_Y_GAP = 3;
constexpr int DEBUG_OVERLAY_SUBSECTION_X_GAP = 30;
constexpr int DEBUG_OVERLAY_SUBSECTION_X_POS =
    DEBUG_OVERLAY_SECTION_X_POS + DEBUG_OVERLAY_SUBSECTION_X_GAP;
constexpr int DEBUG_OVERLAY_SUBSECTION_Y_GAP =
    DEBUG_OVERLAY_SUBSECTION_FONT_SIZE + 3;
constexpr int DEBUG_OVERLAY_SUBSECTION_Y_POS =
    DEBUG_OVERLAY_SECTION_Y_POS + DEBUG_OVERLAY_SUBSECTION_Y_GAP;

// --- Player ---
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
