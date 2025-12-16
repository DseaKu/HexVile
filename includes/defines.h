#ifndef DEFINES_H
#define DEFINES_H
#include "enums.h"
#include "raylib.h"
#include <cstdint>
#include <vector>

namespace Conf {

// --- Core ---
constexpr int MAP_SIZE = 1800;
constexpr int FPS_MAX = 8000;
constexpr const char *WINDOW_TITLE = "HexVile";
constexpr const char *TA_PATH = "assets/images/texture_atlas.png";
constexpr const char *FONT_HACK_REGULAR_PATH = "assets/font/hack_regular.ttf";
constexpr const int TERRAIN_DETAIL_MAX = 2;
constexpr const int CALC_VISIBLE_TILE_PERIOD = 20;
constexpr const int RENDER_VIEW_OFFSET_XY = 50;
constexpr const int RENDER_VIEW_OFFSET_WH = Conf::RENDER_VIEW_OFFSET_XY * 2;

constexpr const int VISIBILTY_ESTIMATED_UPPER_BOUND = 3000;

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
constexpr float TRIGGER_UPDATE_GRID = 3.0f;

// --- Tile ---
constexpr int TILE_SIZE = 32;
constexpr int TILE_SIZE_HALF = TILE_SIZE / 2;
constexpr float TILE_GAP_X = 18.30f;
constexpr float TILE_GAP_Y = 15.95f;
const std::vector<TileID> WALKABLE_TILES = {TILE_GRASS, TILE_DIRT};

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
constexpr float PLAYER_SPEED = 90.0f;
constexpr int PLAYER_Y_OFFSET = -13;
constexpr float OFFSET_TO_OBSTACLE = 5;

// --- UI ---
constexpr float UI_SCALE = 1.0f;
constexpr float UI_TOOL_BAR_Y_BOTTOM_MARGIN = 20.0f * UI_SCALE;
constexpr float UI_TOOL_BAR_PADDING = 15.0f * UI_SCALE;
constexpr float UI_TOOL_SIZE = 1.8f * UI_SCALE * TILE_SIZE;
constexpr float UI_TOOL_BAR_SLOT_SIZE =
    (UI_TOOL_SIZE + UI_TOOL_BAR_PADDING) * UI_SCALE * 1.0f;

} // namespace Conf

// --- Assest & Animation --- TA = Texture Atlas
namespace TA {
constexpr int ASSEST_RESOLUTION = Conf::TILE_SIZE;
constexpr int ASSEST_RESOLUTION_HALF = ASSEST_RESOLUTION / 2;

// --- Player ---
constexpr int PLAYER_X_OFFSET_TILE = ASSEST_RESOLUTION * 0;
constexpr int PLAYER_X_OFFSET_PIXEL = PLAYER_X_OFFSET_TILE * ASSEST_RESOLUTION;
constexpr int PLAYER_X_MAX = 12;
constexpr int PLAYER_X_WALK_MAX = 8;
constexpr float PLAYER_ANIMATION_SPEED = 10.0f;
constexpr float PLAYER_ANIMATION_SPEED_IDLE = 5.0f;
constexpr int PLAYER_X_END =
    PLAYER_X_OFFSET_TILE + ASSEST_RESOLUTION * PLAYER_X_MAX;

// --- Tile ---
constexpr int TILE_X_OFFSET_TILE = PLAYER_X_END;
constexpr int TILE_X_OFFSET_PIXEL = TILE_X_OFFSET_TILE * ASSEST_RESOLUTION;
constexpr int TILES_X_MAX = 8;
constexpr int TILES_FRAME_COUNT = 1;
constexpr float TILES_ANIMATION_SPEED = 0.3f;
constexpr int TILES_X_END =
    TILE_X_OFFSET_TILE + ASSEST_RESOLUTION * TILES_X_MAX;

// --- Item ---
constexpr int ITEM_X_OFFSET_TILE = TILES_X_END;
constexpr int ITEM_X_OFFSET_PIXEL = ITEM_X_OFFSET_TILE * ASSEST_RESOLUTION;
constexpr int ITEM_X_MAX = 1;
constexpr int ITEM_X_END = ITEM_X_OFFSET_TILE + ASSEST_RESOLUTION * ITEM_X_MAX;

// --- Details ---
constexpr int DETAILS_X_OFFSET_TILE = ITEM_X_END;
constexpr int DETAILS_X_OFFSET_PIXEL =
    DETAILS_X_OFFSET_TILE * ASSEST_RESOLUTION;
constexpr int DETAILS_X_MAX = 8;
constexpr int DETAILS_X_END =
    DETAILS_X_OFFSET_TILE + ASSEST_RESOLUTION * DETAILS_X_MAX;

// --- UI ---

constexpr int UI_X_OFFSET_TILE = DETAILS_X_END;
constexpr int UI_X_OFFSET_PIXEL = UI_X_OFFSET_TILE * ASSEST_RESOLUTION;
constexpr int UI_X_MAX = 4;
constexpr int UI_X_END = UI_X_OFFSET_TILE + ASSEST_RESOLUTION * UI_X_MAX;
} // namespace TA
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
