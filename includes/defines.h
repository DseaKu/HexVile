#ifndef DEFINES_H
#define DEFINES_H
#include "enums.h"
#include "raylib.h"
#include <cstdint>
#include <vector>

namespace Conf {

// --- Core ---
constexpr int MAP_SIZE = 8;
constexpr int FPS_MAX = 1000;
constexpr const char *WINDOW_TITLE = "HexVile";
constexpr const char *TA_PATH = "assets/images/texture_atlas.png";
constexpr const char *FONT_HACK_REGULAR_PATH = "assets/font/hack_regular.ttf";

// --- Screen ---
// constexpr int SCREEN_WIDTH = 1400;
// constexpr int SCREEN_HEIGHT = 900;
constexpr int SCREEN_WIDTH = 2800;
constexpr int SCREEN_HEIGHT = 1200;
constexpr Vector2 SCREEN_CENTER = {(float)SCREEN_WIDTH / 2,
                                   (float)SCREEN_HEIGHT / 2};

// --- Game ---
constexpr int ITEM_STACK_MAX_TOOL_BAR = 8;
constexpr int ITEM_STACK_MAX_INVENTORY = 30;

// --- Assest & Animation --- TA = Texture Atlas
constexpr int ASSEST_RESOLUTION = 32;
constexpr int ASSEST_RESOLUTION_HALF = ASSEST_RESOLUTION / 2;

constexpr int TA_PLAYER_X_OFFSET = ASSEST_RESOLUTION * 0;
constexpr int TA_PLAYER_X_MAX = 12;
constexpr int TA_PLAYER_X_WALK_MAX = 8;
constexpr float PLAYER_ANIMATION_SPEED = 10.0f;
constexpr float TA_PLAYER_ANIMATION_SPEED_IDLE = 5.0f;
constexpr int TA_PLAYER_X_END =
    TA_PLAYER_X_OFFSET + ASSEST_RESOLUTION * TA_PLAYER_X_MAX;

constexpr int TA_TILE_X_OFFSET = TA_PLAYER_X_END;
constexpr int TA_TILES_X_MAX = 8;
constexpr int TA_TILES_X_END =
    TA_TILE_X_OFFSET + ASSEST_RESOLUTION * TA_TILES_X_MAX;

constexpr int TA_ITEM_X_OFFSET = TA_TILES_X_END;
constexpr int TA_ITEM_X_MAX = 8;
constexpr int TA_ITEM_X_END =
    TA_ITEM_X_OFFSET + ASSEST_RESOLUTION * TA_ITEM_X_MAX;

// --- Tile ---
constexpr int TILE_SIZE = ASSEST_RESOLUTION;
constexpr int TILE_SIZE_HALF = ASSEST_RESOLUTION_HALF;
constexpr float TILE_GAP_X = 18.4f;
constexpr float TILE_GAP_Y = 16.0f;
const std::vector<TileID> WALKABLE_TILES = {TILE_GRASS};

// --- Camera ---
constexpr float CAMERA_ZOOM = 3.8f;
constexpr float CAMERA_WIDTH = (float)SCREEN_WIDTH / CAMERA_ZOOM;
constexpr float CAMERA_HEIGTH = (float)SCREEN_HEIGHT / CAMERA_ZOOM;

// --- Font ---
constexpr int FONT_SIZE_DEFAULT = 20;
constexpr int FONT_SPACING_DEFAULT = 0.0f;
constexpr Color DEBUG_OVERLAY_SECTION_FONT_COLOR = RED;
constexpr Color DEBUG_OVERLAY_SUBSECTION_FONT_COLOR = RED;
constexpr int DEBUG_OVERLAY_SECTION_FONT_SIZE = 20;
constexpr int DEBUG_OVERLAY_SUBSECTION_FONT_SIZE = 15;
constexpr int HACK_REGULAR_GLYPH_COUNT = 95;

// --- Debug --- DO = Debug Overlay
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
constexpr float PLAYER_SPEED = 70.0f;
constexpr int PLAYER_Y_OFFSET = -13;
constexpr float OFFSET_TO_OBSTACLE = 5;

// --- UI ---
constexpr float UI_SCALE = 1.0f;
constexpr float UI_ITEM_BAR_Y_BOTTOM_MARGIN = 20.0f;
constexpr float UI_ITEM_BAR_SLOT_SIZE = 20.0f;
constexpr float UI_ITEM_BAR_PADDING = 15.0f;

} // namespace Conf

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
