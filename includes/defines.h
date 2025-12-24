#ifndef DEFINES_H
#define DEFINES_H
#include "enums.h"
#include "raylib.h"
#include <cstdint>
#include <vector>

// --- Helper ---
constexpr Rectangle nullRec = Rectangle{0, 0, 0, 0};
constexpr int BIT(int index) { return 1 << index; }

namespace conf {

// ==========================================
//               Core
// ==========================================
constexpr int MAP_RADIUS = 1800;
constexpr int MAX_FPS = 8000;
constexpr const char *WINDOW_TITLE = "HexVile";
constexpr const char *TEXTURE_ATLAS_PATH = "assets/images/texture_atlas.png";
constexpr const char *FONT_HACK_REGULAR_PATH = "assets/font/hack_regular.ttf";
constexpr const int VISIBLE_TILE_CALC_PERIOD = 20;
constexpr float GRID_UPDATE_PLAYER_MOVE_THRESHOLD = 3.0f;
constexpr const int RENDER_VIEW_CULLING_MARGIN = 150;
constexpr const int RENDER_VIEW_CULLING_EXPANSION =
    conf::RENDER_VIEW_CULLING_MARGIN * 2;

constexpr const int ESTIMATED_VISIBLE_TILES = 3000;

// ==========================================
//               Screen
// ==========================================
constexpr int SCREEN_WIDTH = 1400;
constexpr int SCREEN_HEIGHT = 900;
// constexpr int SCREEN_WIDTH = 2800;
// constexpr int SCREEN_HEIGHT = 1200;
constexpr Vector2 SCREEN_CENTER = {(float)SCREEN_WIDTH / 2,
                                   (float)SCREEN_HEIGHT / 2};

// ==========================================
//               Game
// ==========================================
constexpr int TOOLBAR_SLOTS = 8;
constexpr int INVENTORY_SLOTS = 30;
constexpr const int TERRAIN_DETAILS_MAX = 2;
constexpr const int TERRAIN_RESOURCE_MAX = 1;

constexpr float TREE_COLLISION_RADIUS = 3.0f;
constexpr float INTERACT_DISTANCE = 32.0f;
// ==========================================
//               Hex Tile
// ==========================================
constexpr int TILE_RESOLUTION = 32;
constexpr int TILE_RESOLUTION_HALF = TILE_RESOLUTION / 2;
constexpr float TILE_SPACING_X = 18.30f;
constexpr float TILE_SPACING_Y = 15.95f;
const std::vector<tile::id> WALKABLE_TILE_IDS = {tile::GRASS, tile::DIRT};

// ==========================================
//               Camera
// ==========================================
constexpr float INITIAL_CAMERA_ZOOM = 4.2f; // 3.8f;
// constexpr float CAMERA_ZOOM = 8.0f; // 3.8f;
constexpr float CAMERA_WIDTH = (float)SCREEN_WIDTH / INITIAL_CAMERA_ZOOM;
constexpr float CAMERA_HEIGHT = (float)SCREEN_HEIGHT / INITIAL_CAMERA_ZOOM;

// ==========================================
//               Player
// ==========================================
constexpr float PLAYER_MOVE_SPEED = 90.0f;
constexpr int PLAYER_SPRITE_Y_OFFSET = -13;
constexpr float PLAYER_COLLISION_CHECK_OFFSET = 5;

// ==========================================
//               UI
// ==========================================
constexpr float UI_SCALE = 1.0f;
constexpr float TOOLBAR_BOTTOM_MARGIN = 20.0f * UI_SCALE;
constexpr float TOOLBAR_PADDING = 10.0f * UI_SCALE;
constexpr float TOOLBAR_SLOT_CONTENT_SIZE = 1.8f * UI_SCALE * TILE_RESOLUTION;
constexpr float TOOLBAR_SLOT_SIZE =
    (TOOLBAR_SLOT_CONTENT_SIZE + TOOLBAR_PADDING) * UI_SCALE * 1.0f;
constexpr float TOOLBAR_ITEM_ICON_SCALE = 0.8f * UI_SCALE;
// ==========================================
//               Details & Objects
// ==========================================
constexpr int SKIP_RENDER = -1;
constexpr int UNINITIALIZED = -2;
constexpr float GAUSIAN_EFFECT = 6.5f;

// ==========================================
//               Font
// ==========================================
constexpr int DEFAULT_FONT_SIZE = 20;
constexpr int DEFAULT_FONT_SPACING = 0.0f;
constexpr Color DEBUG_OVERLAY_SECTION_FONT_COLOR = RED;
constexpr Color DEBUG_OVERLAY_SUBSECTION_FONT_COLOR = RED;
constexpr int DEBUG_OVERLAY_SECTION_FONT_SIZE = 20;
constexpr int DEBUG_OVERLAY_SUBSECTION_FONT_SIZE = 15;
constexpr int HACK_REGULAR_GLYPH_COUNT = 95;

// ==========================================
//               Debug Overlay
// ==========================================
constexpr bool IS_DEBUG_OVERLAY_ENABLED = true;
constexpr int DEBUG_OVERLAY_SECTION_X = 30;
constexpr int DEBUG_OVERLAY_SECTION_Y = 30;
constexpr int DEBUG_OVERLAY_SECTION_Y_GAP = 3;
constexpr int DEBUG_OVERLAY_SUBSECTION_X_GAP = 30;
constexpr int DEBUG_OVERLAY_SUBSECTION_X_POS =
    DEBUG_OVERLAY_SECTION_X + DEBUG_OVERLAY_SUBSECTION_X_GAP;
constexpr int DEBUG_OVERLAY_SUBSECTION_Y_GAP =
    DEBUG_OVERLAY_SUBSECTION_FONT_SIZE + 3;
constexpr int DEBUG_OVERLAY_SUBSECTION_Y_POS =
    DEBUG_OVERLAY_SECTION_Y + DEBUG_OVERLAY_SUBSECTION_Y_GAP;
} // namespace conf

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

struct KeyboardInput {
  bool One;
  bool Two;
  bool Three;
  bool Four;
  bool Five;
  bool Six;
  bool Seven;
  bool Eight;
  bool Nine;
  bool Zero;
  bool Up;
  bool Down;
  bool Left;
  bool Right;
};

struct MouseInput {
  bool left;
  bool right;
};

struct Inputs {
  MouseInput mousePress;
  KeyboardInput keyPress;
};

struct FrameContext {
  Inputs inputs;
  Vector2 mouseScreenPos;
  Vector2 mouseWorldPos;
  mouseMask::id mouseMask;

  // Screen
  int screenWidth;
  int screenHeight;

  float deltaTime;
};

#endif // !DEFINES_H
