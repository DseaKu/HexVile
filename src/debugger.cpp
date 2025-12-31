#include "debugger.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"

#ifdef __APPLE__
#include <mach/mach.h>
#endif

// --- Helper Functions ---
static double GetRamUsageMB() {
#ifdef __APPLE__
  struct mach_task_basic_info info;
  mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info,
                &infoCount) == KERN_SUCCESS) {
    return (double)info.resident_size / (1024 * 1024);
  }
#endif
  return 0.0;
}

// --- Debugger ---
Debugger::Debugger() {
  gfxManager = nullptr;
  fontHandler = nullptr;
  debugUpdateTimer = 0.0f;
  displayRenderTime = 0.0;
  displayLogicTime = 0.0;
  displayVisTime = 0.0;
  displayRamUsage = 0.0;
}

void Debugger::SetManagers(GFX_Manager *gfx, FontHandler *font) {
  this->gfxManager = gfx;
  this->fontHandler = font;
}

const char *Debugger::MouseMaskToString(mouseMask::id m) {
  switch (m) {
  case mouseMask::NULL_ID:
    return "Null";
  case mouseMask::GROUND:
    return "Ground";
  case mouseMask::TOOL_BAR:
    return "Tool Bar";
  default:
    return "Undefined";
  }
}

const char *Debugger::TileToString(tile::id t) {
  switch (t) {
  case tile::NULL_ID:
    return "NULL";
  case tile::GRASS:
    return "Grass";
  case tile::WATER:
    return "Water";
  case tile::DIRT:
    return "Dirt";
  default:
    return "Undefined";
  }
}

void Debugger::Update(const RenderState &rs, float dt, double logicTime,
                      double renderTime) {
  if (!gfxManager || !fontHandler)
    return;

  if (!conf::IS_DEBUG_OVERLAY_ENABLED)
    return;

  debugUpdateTimer += dt;
  if (debugUpdateTimer >= 1.0f) {
    displayRenderTime = renderTime;
    displayLogicTime = logicTime;
    displayVisTime = rs.visCalcTime;
    displayRamUsage = GetRamUsageMB();
    debugUpdateTimer = 0.0f;
  }

  debugData.clear();
  debugData.push_back(
      {"Resources",
       {
           TextFormat("FPS: %i", GetFPS()),
           TextFormat("RAM: %.2f MB", displayRamUsage),
           TextFormat("Screen: %ix%i", GetScreenWidth(), GetScreenHeight()),
           TextFormat("Render: %ix%i", GetRenderWidth(), GetRenderHeight()),
           TextFormat("Tiles Total: %i", rs.tilesTotal),
           TextFormat("Tiles Used: %i", rs.tilesUsed),
           TextFormat("Tiles Visible: %i", rs.tilesVisible),
           TextFormat("Map radius: %i", rs.mapRadius),
           TextFormat("Render Time: %.2f ms", displayRenderTime),
           TextFormat("Logic Time: %.2f ms", displayLogicTime),
           TextFormat("Culling Time: %.2f ms", displayVisTime),
       }});

  debugData.push_back(
      {"Mouse",
       {
           TextFormat("X,Y: %.1f,%.1f", GetMousePosition().x,
                      GetMousePosition().y),
           TextFormat("Tile Q,R: %i,%i", rs.mouseTileCoord.q,
                      rs.mouseTileCoord.r),
           TextFormat("Type: %s", TileToString(rs.mouseTileType)),
       }});

  debugData.push_back(
      {"Player",
       {
           TextFormat("X,Y: %.1f,%.1f", rs.playerPos.x, rs.playerPos.y),
           TextFormat("Tile Q,R: %i,%i", rs.playerTileCoord.q,
                      rs.playerTileCoord.r),
           TextFormat("State:  %s", rs.playerStateStr.c_str()),
           TextFormat("Face Dir: %s", rs.playerDirStr.c_str()),
           TextFormat("Frame: %i", rs.playerFrame),
           TextFormat("Type: %s", TileToString(rs.playerTileID)),
           TextFormat("Speed[1/s]: %.2f", rs.playerSpeed),
       }});

  debugData.push_back({"Tool Bar",
                       {
                           TextFormat("Item: %s", rs.selectedItemType.c_str()),
                           TextFormat("Slot: %i", rs.selectedToolBarSlot),
                       }});
}

void Debugger::LoadBackBuffer() {
  if (!gfxManager || !fontHandler)
    return;

  if (!conf::IS_DEBUG_OVERLAY_ENABLED)
    return;

  float sectionPosX = conf::DEBUG_OVERLAY_SECTION_X;
  int sectionPosY = conf::DEBUG_OVERLAY_SECTION_Y;
  int sectionGapY = conf::DEBUG_OVERLAY_SECTION_Y_GAP;
  Color sectionColor = conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float subSectionPosX = conf::DEBUG_OVERLAY_SUBSECTION_X_POS;
  int subSectionGapY = conf::DEBUG_OVERLAY_SUBSECTION_Y_GAP;
  Color subSectionColor = conf::DEBUG_OVERLAY_SECTION_FONT_COLOR;

  float currentY = (float)sectionPosY;

  // Draw text
  for (const DebugData &data : debugData) {
    fontHandler->QueueText(gfxManager, data.section.c_str(),
                           {sectionPosX, currentY}, sectionColor);
    currentY += sectionGapY;
    currentY += subSectionGapY;

    // Draw sub-section text
    for (const std::string &subSection : data.subSection) {
      fontHandler->QueueText(gfxManager, subSection.c_str(),
                             {subSectionPosX, currentY}, subSectionColor);
      currentY += subSectionGapY;
    }
    currentY += sectionGapY;
  }
}
