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
  isInventoryOpen = false;

  // Dependencies
  graphicsManager = nullptr;
  itemHandler = nullptr;
  fontHandler = nullptr;
  hexGrid = nullptr;
}

// --- Core ---
void UI_Handler::Update() {
  if (frameContext->inputs.commands.toggleInventory) {
    ToggleInventory();
  }

  LoadToolBarGFX();
  LoadHighlightGFX();

  if (isInventoryOpen) {
    LoadInventoryBackgroundGFX();
    LoadInventoryItemsGFX();
  }
}

void UI_Handler::ToggleInventory() {
  if (this->isInventoryOpen) {
    isInventoryOpen = false;
  } else {
    isInventoryOpen = true;
  }
}

void UI_Handler::LoadHighlightGFX() {
  // Get to hightlithning object
  item::id itemID =
      itemHandler->GetToolBarItemType(frameContext->selToolBarSlot);
  if (itemID == item::AXE) {
    // LoadHighlightResourceGFX(rsrc::ID_TREE);
  }

  if (itemID == item::SET_GRASS) {
    LoadHighlightTileGFX();
  }
  if (itemID == item::SET_WATER) {
    LoadHighlightTileGFX();
  }
  if (itemID == item::SET_DIRT) {
    LoadHighlightTileGFX();
  }
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
  hexGrid->DrawTile(coord, tex_atlas::UI_X, ui::TILE_H, drawMask::GROUND1);
}

void UI_Handler::LoadHighlightResourceGFX(rsrc::ID id) {
  if (!frameContext || !frameContext->pos.hoveredTile)
    return;

  // Can be changed to indicate 'not in range'
  Color col = WHITE;

  // If player is in interact range change color indicator to yellow
  Vector2 curMousePos = frameContext->pos.mouseWorld;
  if (Vector2Distance(curMousePos, frameContext->pos.player) <
      conf::INTERACT_DISTANCE_PLAYER) {
    col = YELLOW;
  }

  const rsrc::Object &rsrc = frameContext->pos.hoveredTile->rsrc;
  if (rsrc.id == id) {
    // Get position of resource
    Vector2 rsrcPos = {rsrc.worldPos.x - tex_atlas::RES16_F,
                       rsrc.worldPos.y - tex_atlas::RES32_F};

    if (rsrc.id == rsrc::ID_TREE) {
      rsrcPos.y -= tex_atlas::RES32_F;
    }

    if (Vector2Distance(rsrcPos, curMousePos) < conf::INTERACT_DISTANCE_MOUSE) {
      graphicsManager->LoadTextureToBackbuffer(
          drawMask::ON_GROUND, tex_atlas::RSRC_TREE, rsrcPos,
          {.color = Fade(col, conf::HIGHLIGHT_ALPHA),
           .srcHeight = tex_atlas::RES64_F,
           .sortingOffsetY = tex_atlas::RES32_F});
    }
  }
}

void UI_Handler::LoadInventoryBackgroundGFX() {

  // Center inventory
  Vector2 dst = {conf::SCREEN_CENTER.x - tex_atlas::INVENTORY_WIDTH / 2,
                 conf::SCREEN_CENTER.y - tex_atlas::INVENTORY_HEIGTH / 2};

  graphicsManager->LoadTextureToBackbuffer(drawMask::UI_0,
                                           tex_atlas::INVENTORY_COORDS, dst,
                                           gfx::TextureOptsInventoryBG);
}

void UI_Handler::LoadInventoryItemsGFX() {
  int cols = conf::INVENTORY_CELL_COLS;
  int rows = conf::INVENTORY_SLOTS / cols;
  float cellSize = conf::INVENTORY_CELL_SIZE * conf::UI_SCALE;

  float totalWidth = cols * cellSize;
  float totalHeight = rows * cellSize;

  float startX = conf::SCREEN_CENTER.x - (totalWidth / 2.0f);
  float startY = conf::SCREEN_CENTER.y - (totalHeight / 2.0f);

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {
      Rectangle dstRect = {startX + (x * cellSize), startY + (y * cellSize),
                           cellSize, cellSize};
    }
  }
  // Draw item slots
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
  graphicsManager->LoadTextureToBackbuffer_Ex(
      drawMask::UI_0, {tex_atlas::UI_X, ui::ITEM_BAR_BG}, slotRect, {WHITE});
  if (slotIndex == frameContext->selToolBarSlot) {
    graphicsManager->LoadTextureToBackbuffer_Ex(
        drawMask::UI_0, {tex_atlas::UI_X, ui::ITEM_BAR_BG_H}, slotRect,
        {WHITE});
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

  graphicsManager->LoadTextureToBackbuffer_Ex(drawMask::UI_0, taCoords, dstRec,
                                              {WHITE});
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

    graphicsManager->LoadTextureToBackbuffer_Ex(
        drawMask::UI_1, {tex_atlas::NUMBER_X, digit}, digitRect, {WHITE});
    digitIndex++;
  }
}

// --- Getter ---
int UI_Handler::GetToolBarSelection() {
  int curSelection = frameContext->selToolBarSlot;
  if (curSelection < 0 || curSelection >= conf::TOOLBAR_SLOTS) {
    curSelection = 0;
  }
  frame::InputCommands keyPress = frameContext->inputs.commands;

  int toolBarSlotBuffer = curSelection;
  if (keyPress.slot0)
    curSelection = 0;
  if (keyPress.slot1)
    curSelection = 1;
  if (keyPress.slot2)
    curSelection = 2;
  if (keyPress.slot3)
    curSelection = 3;
  if (keyPress.slot4)
    curSelection = 4;
  if (keyPress.slot5)
    curSelection = 5;
  if (keyPress.slot6)
    curSelection = 6;
  if (keyPress.slot7)
    curSelection = 7;
  if (keyPress.slot8)
    curSelection = 8;
  if (keyPress.slot9)
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
