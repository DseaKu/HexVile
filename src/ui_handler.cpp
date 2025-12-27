#include "ui_handler.h"
#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "frame_context.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "raymath.h"
#include "resource.h"
#include "texture_atlas.h"
#include <string>

// --- Initialization ---
UI_Handler::UI_Handler() {
  // Initialize Layout Configuration
  toolBarLayout.maxSlots = conf::TOOLBAR_SLOTS;
  toolBarLayout.padding = conf::TOOLBAR_PADDING;
  toolBarLayout.contentSize = conf::TOOLBAR_SLOT_CONTENT_SIZE;
  toolBarLayout.slotSize = conf::TOOLBAR_SLOT_SIZE;
  toolBarLayout.itemScale = conf::TOOLBAR_ITEM_ICON_SCALE;
  toolBarLayout.scale = conf::UI_SCALE;
  toolBarLayout.bottomMargin = conf::TOOLBAR_BOTTOM_MARGIN;

  // Calculate Geometry
  toolBarLayout.width =
      (toolBarLayout.maxSlots * toolBarLayout.slotSize) + toolBarLayout.padding;
  toolBarLayout.height =
      toolBarLayout.contentSize + (2 * toolBarLayout.padding);

  // Initial Position (centered horizontally, bottom of screen)
  toolBarLayout.posX = conf::SCREEN_CENTER.x - (toolBarLayout.width / 2.0f);
  toolBarLayout.posY =
      conf::SCREEN_HEIGHT - toolBarLayout.height - toolBarLayout.bottomMargin;

  toolBarLayout.rect = {toolBarLayout.posX, toolBarLayout.posY,
                        toolBarLayout.width, toolBarLayout.height};

  // State
  isToolBarActive = false;

  // Dependencies
  graphicsManager = nullptr;
  itemHandler = nullptr;
  fontHandler = nullptr;
  hexGrid = nullptr;
}

// --- Core ---
void UI_Handler::Update() {
  LoadToolBarGFX();
  LoadHighlightGFX();
}

void UI_Handler::LoadHighlightGFX() {
  // Get to hightlithning object
  item::id itemID =
      itemHandler->GetToolBarItemType(frameContext->selToolBarSlot);
  if (itemID == item::AXE) {
    LoadHighlightResourceGFX(rsrc::ID_TREE);
  }

  LoadHighlightTileGFX();
}
void UI_Handler::UpdateScreenSize(int width, int height) {
  toolBarLayout.posX = (width / 2.0f) - (toolBarLayout.width / 2.0f);
  toolBarLayout.posY =
      height - toolBarLayout.height - toolBarLayout.bottomMargin;
  toolBarLayout.rect = {toolBarLayout.posX, toolBarLayout.posY,
                        toolBarLayout.width, toolBarLayout.height};
}

mouseMask::id UI_Handler::UpdateMouseMask() {
  if (isToolBarActive && CheckCollisionPointRec(frameContext->pos.mouseScreen,
                                                toolBarLayout.rect)) {
    return mouseMask::TOOL_BAR;
  } else {
    return mouseMask::GROUND;
  }
}
// --- Input & Queries ---
bool UI_Handler::GetToolBarAvailability() { return isToolBarActive; }
Rectangle UI_Handler::GetToolBarRect() { return toolBarLayout.rect; }

// --- Setters ---
void UI_Handler::SetGFX_Manager(GFX_Manager *p) { graphicsManager = p; }
void UI_Handler::SetItemHandler(ItemHandler *p) { itemHandler = p; }
void UI_Handler::SetFontHandler(FontHandler *p) { fontHandler = p; }
void UI_Handler::SetHexGrid(HexGrid *p) { hexGrid = p; }
void UI_Handler::SetToolBarActive(bool is_active) {
  isToolBarActive = is_active;
}

void UI_Handler::SetFrameContext(const frame::Context *frameContext) {
  this->frameContext = frameContext;
}

// --- Load GFX Data ---
void UI_Handler::LoadHighlightTileGFX() {
  if (!hexGrid)
    return;
  HexCoord coord = hexGrid->PointToHexCoord(frameContext->pos.mouseWorld);
  hexGrid->DrawTile(coord, tex_atlas::UI_X, ui::TILE_H, drawMask::GROUND_1);
}
void UI_Handler::LoadHighlightResourceGFX(rsrc::ID id) {
  if (!frameContext || !frameContext->hoveredTile)
    return;

  Vector2 curMousePos = frameContext->pos.mouseWorld;

  for (const rsrc::Object &rsrc : frameContext->hoveredTile->rsrc) {
    if (rsrc.id != id)
      continue;

    // Get position of resource
    Vector2 rsrcPos = {frameContext->pos.hoveredTilePoint.x -
                           tex_atlas::RES16_F + rsrc.tilePos.x,
                       frameContext->pos.hoveredTilePoint.y -
                           tex_atlas::RES32_F + rsrc.tilePos.y};

    if (Vector2Distance(rsrcPos, curMousePos) < conf::INTERACT_DISTANCE) {
      graphicsManager->LoadGFX_Data_32x64(
          drawMask::UI_0, tex_atlas::RSRC_TREE_HIGHLIGHTED, rsrcPos, WHITE);
    }
  }
}
void UI_Handler::LoadToolBarGFX() {
  if (!isToolBarActive || !itemHandler)
    return;

  for (int i = 0; i < toolBarLayout.maxSlots; i++) {
    LoadToolBarSlotGFX(i);
  }
}

void UI_Handler::LoadToolBarSlotGFX(int slotIndex) {
  float slotPosX = toolBarLayout.posX + toolBarLayout.padding +
                   (slotIndex * toolBarLayout.slotSize);
  float slotPosY = toolBarLayout.posY + toolBarLayout.padding;

  Rectangle slotRect = {slotPosX, slotPosY, (float)toolBarLayout.contentSize,
                        (float)toolBarLayout.contentSize};

  // 1. Draw Background
  graphicsManager->LoadGFX_Data(
      drawMask::UI_0, {tex_atlas::UI_X, ui::ITEM_BAR_BG}, slotRect, WHITE);
  if (slotIndex == frameContext->selToolBarSlot) {
    graphicsManager->LoadGFX_Data(
        drawMask::UI_0, {tex_atlas::UI_X, ui::ITEM_BAR_BG_H}, slotRect, WHITE);
  }

  // 2. Draw Content
  ItemStack *itemStack = itemHandler->GetToolBarItemPointer(slotIndex);
  if (itemStack && itemStack->itemID != item::NULL_ID) {
    LoadItemIconGFX(slotIndex, slotRect);
    LoadItemCountGFX(slotIndex, slotRect);
  }
}

void UI_Handler::LoadItemIconGFX(int slotIndex, Rectangle slotRect) {
  ItemStack *itemStack = itemHandler->GetToolBarItemPointer(slotIndex);

  item::id itemID = itemStack->itemID;
  tex_atlas::Coords taCoords = tex_atlas::ITEM_TEXTURE_COORDS.at(itemID);

  // Calculate shrunk rect for icon
  float newWidth = slotRect.width * toolBarLayout.itemScale;
  float newHeight = slotRect.height * toolBarLayout.itemScale;
  float offsetX = (slotRect.width - newWidth) / 2.0f;
  float offsetY = (slotRect.height - newHeight) / 2.0f;

  Rectangle dstRec = {slotRect.x + offsetX, slotRect.y + offsetY, newWidth,
                      newHeight};

  graphicsManager->LoadGFX_Data(drawMask::UI_0, taCoords, dstRec, WHITE);
}

void UI_Handler::LoadItemCountGFX(int slotIndex, Rectangle slotRect) {
  ItemStack *item = itemHandler->GetToolBarItemPointer(slotIndex);

  // Re-calculate the icon rect to position numbers correctly relative to it?
  // Original code used the icon rect for numbers too.

  float newWidth = slotRect.width * toolBarLayout.itemScale;
  float newHeight = slotRect.height * toolBarLayout.itemScale;
  float offsetX = (slotRect.width - newWidth) / 2.0f;
  float offsetY = (slotRect.height - newHeight) / 2.0f;

  Rectangle iconRect = {slotRect.x + offsetX, slotRect.y + offsetY, newWidth,
                        newHeight};

  // Draw Numbers (Right-aligned to bottom-right corner)
  std::string num_str = std::to_string(item->count);
  int digitIndex = 0;

  // Iterate backwards to draw from right to left
  for (auto it = num_str.rbegin(); it != num_str.rend(); ++it) {
    int digit = *it - '0';
    if (digit == 0)
      digit = 10; // Atlas mapping

    Vector2 rbCorner = {iconRect.x + iconRect.width,
                        iconRect.y + iconRect.height};

    Rectangle digitRect = {
        rbCorner.x - (digitIndex * tex_atlas::NUMBER_SCALE), rbCorner.y,
        -tex_atlas::NUMBER_SCALE, // Negative width/height implies flip? Or just
                                  // anchor?
        -tex_atlas::NUMBER_SCALE  // Original code had negative. Assuming it
                                  // means "draw up/left from anchor"
    };

    graphicsManager->LoadGFX_Data(drawMask::UI_1, {tex_atlas::NUMBER_X, digit},
                                  digitRect, WHITE);
    digitIndex++;
  }
}

// --- Getter ---
int UI_Handler::GetToolBarSelection() {
  int curSelection = frameContext->selToolBarSlot;
  if (curSelection < 0 || curSelection >= conf::TOOLBAR_SLOTS) {
    curSelection = 0;
  }
  frame::KeyboardInput keyPress = frameContext->inputs.keyPress;

  int toolBarSlotBuffer = curSelection;
  if (keyPress.one)
    curSelection = 0;
  if (keyPress.two)
    curSelection = 1;
  if (keyPress.three)
    curSelection = 2;
  if (keyPress.four)
    curSelection = 3;
  if (keyPress.five)
    curSelection = 4;
  if (keyPress.six)
    curSelection = 5;
  if (keyPress.seven)
    curSelection = 6;
  if (keyPress.eight)
    curSelection = 7;
  if (keyPress.nine)
    curSelection = 8;
  if (keyPress.zero)
    curSelection = 9;

  // Revert selection if the toolbar slot is out of range
  if (curSelection >= conf::TOOLBAR_SLOTS) {
    curSelection = toolBarSlotBuffer;
  }

  // Check if player clicked Tool Bar
  if (frameContext->inputs.mouseClick.left &&
      frameContext->mouseMask == mouseMask::TOOL_BAR) {

    // Local coordinate in the toolbar
    float localX = frameContext->pos.mouseScreen.x - toolBarLayout.posX -
                   toolBarLayout.padding;

    if (localX < 0)
      return 0;

    int slotIndex = (int)(localX / toolBarLayout.slotSize);

    if (slotIndex >= 0 && slotIndex < toolBarLayout.maxSlots) {
      curSelection = slotIndex;
    }
  }
  return curSelection;
}
