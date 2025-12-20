#include "ui_handler.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "texture_atlas.h"
#include <string>

// --- Initialization ---
UI_Handler::UI_Handler() {
  // Initialize Layout Configuration
  toolBarLayout.maxSlots = Conf::TOOLBAR_SLOTS;
  toolBarLayout.padding = Conf::TOOLBAR_PADDING;
  toolBarLayout.contentSize = Conf::TOOLBAR_SLOT_CONTENT_SIZE;
  toolBarLayout.slotSize = Conf::TOOLBAR_SLOT_SIZE;
  toolBarLayout.itemScale = Conf::TOOLBAR_ITEM_ICON_SCALE;
  toolBarLayout.scale = Conf::UI_SCALE;
  toolBarLayout.bottomMargin = Conf::TOOLBAR_BOTTOM_MARGIN;

  // Calculate Geometry
  toolBarLayout.width =
      (toolBarLayout.maxSlots * toolBarLayout.slotSize) + toolBarLayout.padding;
  toolBarLayout.height =
      toolBarLayout.contentSize + (2 * toolBarLayout.padding);

  // Initial Position (centered horizontally, bottom of screen)
  toolBarLayout.posX = Conf::SCREEN_CENTER.x - (toolBarLayout.width / 2.0f);
  toolBarLayout.posY =
      Conf::SCREEN_HEIGHT - toolBarLayout.height - toolBarLayout.bottomMargin;

  toolBarLayout.rect = {toolBarLayout.posX, toolBarLayout.posY,
                        toolBarLayout.width, toolBarLayout.height};

  // State
  isToolBarActive = false;
  selectedItemIndex = 0;

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

// --- Input & Queries ---
int UI_Handler::GetToolBarSelection(KeyboardInput keyPress,
                                    int currentSelection) {
  if (keyPress.One)
    return 0;
  if (keyPress.Two)
    return 1;
  if (keyPress.Three)
    return 2;
  if (keyPress.Four)
    return 3;
  if (keyPress.Five)
    return 4;
  if (keyPress.Six)
    return 5;
  if (keyPress.Seven)
    return 6;
  if (keyPress.Eight)
    return 7;
  if (keyPress.Nine)
    return 8;
  if (keyPress.Zero)
    return 9;

  return currentSelection;
}

int UI_Handler::GetItemSlotAt(Vector2 screenPos) {
  if (!isToolBarActive)
    return -1;

  if (CheckCollisionPointRec(screenPos, toolBarLayout.rect)) {
    // Local coordinate in the toolbar
    float localX = screenPos.x - toolBarLayout.posX - toolBarLayout.padding;

    if (localX < 0)
      return -1;

    int slotIndex = (int)(localX / toolBarLayout.slotSize);

    if (slotIndex >= 0 && slotIndex < toolBarLayout.maxSlots) {
      return slotIndex;
    }
  }
  return -1;
}

bool UI_Handler::GetToolBarAvailability() { return isToolBarActive; }
Rectangle UI_Handler::GetToolBarRect() { return toolBarLayout.rect; }

// --- Setters ---
void UI_Handler::SetGFX_Manager(GFX_Manager *p) { graphicsManager = p; }
void UI_Handler::SetItemHandler(ItemHandler *p) { itemHandler = p; }
void UI_Handler::SetFontHandler(FontHandler *p) { fontHandler = p; }
void UI_Handler::SetHexGrid(HexGrid *p) { hexGrid = p; }
void UI_Handler::SetSelectedItem(int index) { selectedItemIndex = index; }
void UI_Handler::SetToolBarActive(bool is_active) {
  isToolBarActive = is_active;
}

// --- Rendering Helpers ---
void UI_Handler::DrawHighlighedTile(Vector2 mouseWorldPos) {
  if (!hexGrid)
    return;
  HexCoord coord = hexGrid->PointToHexCoord(mouseWorldPos);
  hexGrid->DrawTile(coord, TA::UI_X, UI_ID_TILE_H, DRAW_MASK_GROUND_1);
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
  int uiId = (slotIndex == itemHandler->GetSelectionToolBar())
                 ? UI_ID_ITEM_BAR_BG_H
                 : UI_ID_ITEM_BAR_BG;
  graphicsManager->LoadGFX_Data(DRAW_MASK_UI_0, slotRect.y, TA::UI_X, uiId,
                                slotRect, WHITE);

  // 2. Draw Content
  Item *item = itemHandler->GetToolBarItemPointer(slotIndex);
  if (item && item->id != ITEM_NULL) {
    DrawItemIcon(slotIndex, slotRect);
    DrawItemCount(slotIndex, slotRect);
  }
}

void UI_Handler::DrawItemIcon(int slotIndex, Rectangle slotRect) {
  Item *item = itemHandler->GetToolBarItemPointer(slotIndex);

  // Calculate shrunk rect for icon
  float newWidth = slotRect.width * toolBarLayout.itemScale;
  float newHeight = slotRect.height * toolBarLayout.itemScale;
  float offsetX = (slotRect.width - newWidth) / 2.0f;
  float offsetY = (slotRect.height - newHeight) / 2.0f;

  Rectangle iconRect = {slotRect.x + offsetX, slotRect.y + offsetY, newWidth,
                        newHeight};

  graphicsManager->LoadGFX_Data(DRAW_MASK_UI_0, iconRect.y, TA::ITEM_X,
                                item->id, iconRect, WHITE);
}

void UI_Handler::DrawItemCount(int slotIndex, Rectangle slotRect) {
  Item *item = itemHandler->GetToolBarItemPointer(slotIndex);

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
        rbCorner.x - (digitIndex * TA::NUMBER_SCALE), rbCorner.y,
        -TA::NUMBER_SCALE, // Negative width/height implies flip? Or just
                           // anchor?
        -TA::NUMBER_SCALE // Original code had negative. Assuming it means "draw
                          // up/left from anchor"
    };

    graphicsManager->LoadGFX_Data(DRAW_MASK_UI_1, digitRect.y, TA::NUMBER_X,
                                  digit, digitRect, WHITE);
    digitIndex++;
  }
}
