#include "ui_handler.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "texture_atlas.h"
#include <string>
#include <utility>

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
void UI_Handler::Update(Vector2 mouseWorldPos) {
  DrawHighlighedTile(mouseWorldPos);
  DrawToolBar();
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
void UI_Handler::UpdateToolBarSelection(int *currentSelection) {
  KeyboardInput keyPress = frameContext->inputs.keyPress;

  int toolBarSlotBuffer = *currentSelection;
  if (keyPress.One)
    *currentSelection = 0;
  if (keyPress.Two)
    *currentSelection = 1;
  if (keyPress.Three)
    *currentSelection = 2;
  if (keyPress.Four)
    *currentSelection = 3;
  if (keyPress.Five)
    *currentSelection = 4;
  if (keyPress.Six)
    *currentSelection = 5;
  if (keyPress.Seven)
    *currentSelection = 6;
  if (keyPress.Eight)
    *currentSelection = 7;
  if (keyPress.Nine)
    *currentSelection = 8;
  if (keyPress.Zero)
    *currentSelection = 9;

  // Revert selection if the toolbar slot is out of range
  if (*currentSelection >= conf::TOOLBAR_SLOTS) {
    *currentSelection = toolBarSlotBuffer;
  }

  // Check if player clicked Tool Bar
  if (frameContext->inputs.mouseClick.left &&
      frameContext->mouseMask == mouseMask::TOOL_BAR) {

    // Local coordinate in the toolbar
    float localX = frameContext->pos.mouseScreen.x - toolBarLayout.posX -
                   toolBarLayout.padding;

    if (localX < 0)
      return;

    int slotIndex = (int)(localX / toolBarLayout.slotSize);

    if (slotIndex >= 0 && slotIndex < toolBarLayout.maxSlots) {
      *currentSelection = slotIndex;
    }
  }
}

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

void UI_Handler::SetFrameContext(const FrameContext *frameContext) {
  this->frameContext = frameContext;
}

// --- Rendering Helpers ---
void UI_Handler::DrawHighlighedTile(Vector2 mouseWorldPos) {
  if (!hexGrid)
    return;
  HexCoord coord = hexGrid->PointToHexCoord(mouseWorldPos);
  hexGrid->DrawTile(coord, tex_atlas::UI_X, ui::TILE_H, drawMask::GROUND_1);
}

void UI_Handler::DrawToolBar() {
  if (!isToolBarActive || !itemHandler)
    return;

  for (int i = 0; i < toolBarLayout.maxSlots; i++) {
    DrawToolBarSlot(i);
  }
}

void UI_Handler::DrawToolBarSlot(int slotIndex) {
  float slotPosX = toolBarLayout.posX + toolBarLayout.padding +
                   (slotIndex * toolBarLayout.slotSize);
  float slotPosY = toolBarLayout.posY + toolBarLayout.padding;

  Rectangle slotRect = {slotPosX, slotPosY, (float)toolBarLayout.contentSize,
                        (float)toolBarLayout.contentSize};

  // 1. Draw Background
  graphicsManager->LoadGFX_Data(drawMask::UI_0, tex_atlas::UI_X,
                                ui::ITEM_BAR_BG, slotRect, WHITE);
  if (slotIndex == frameContext->selToolBarSlot) {
    graphicsManager->LoadGFX_Data(drawMask::UI_0, tex_atlas::UI_X,
                                  ui::ITEM_BAR_BG_H, slotRect, WHITE);
  }

  // 2. Draw Content
  ItemStack *itemStack = itemHandler->GetToolBarItemPointer(slotIndex);
  if (itemStack && itemStack->itemID != item::NULL_ID) {
    DrawItemIcon(slotIndex, slotRect);
    DrawItemCount(slotIndex, slotRect);
  }
}

void UI_Handler::DrawItemIcon(int slotIndex, Rectangle slotRect) {
  ItemStack *itemStack = itemHandler->GetToolBarItemPointer(slotIndex);

  item::id itemID = itemStack->itemID;
  std::pair<float, float> textCoords =
      tex_atlas::ITEM_TEXTURE_COORDS.at(itemID);

  // Calculate shrunk rect for icon
  float newWidth = slotRect.width * toolBarLayout.itemScale;
  float newHeight = slotRect.height * toolBarLayout.itemScale;
  float offsetX = (slotRect.width - newWidth) / 2.0f;
  float offsetY = (slotRect.height - newHeight) / 2.0f;

  Rectangle dstRec = {slotRect.x + offsetX, slotRect.y + offsetY, newWidth,
                      newHeight};

  graphicsManager->LoadGFX_Data(drawMask::UI_0, textCoords.first,
                                textCoords.second, dstRec, WHITE);
}

void UI_Handler::DrawItemCount(int slotIndex, Rectangle slotRect) {
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

    graphicsManager->LoadGFX_Data(drawMask::UI_1, tex_atlas::NUMBER_X, digit,
                                  digitRect, WHITE);
    digitIndex++;
  }
}
