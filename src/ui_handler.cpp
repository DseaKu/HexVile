#include "ui_handler.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"

UI_Handler::UI_Handler() {
  scale = Conf::UI_SCALE;
  textureHandler = nullptr;
  itemHandler = nullptr;
  selectedItemIndex = 0;
  isToolBarActive = false;
}

void UI_Handler::SetTextureHandler(TextureHandler *th) {
  this->textureHandler = th;
}

void UI_Handler::SetItemHandler(ItemHandler *ih) { this->itemHandler = ih; }

void UI_Handler::SetSelectedItem(int index) {
  if (index >= 0 && index < 10) {
    selectedItemIndex = index;
  }
}

void UI_Handler::DrawToolBar() {
  if (!isToolBarActive) {
    return;
  }
  const int itemCount = Conf::ITEM_STACK_MAX_TOOL_BAR;
  const float padding = Conf::UI_TOOL_BAR_PADDING;
  const int itemSize = Conf::UI_TOOL_SIZE;
  const int slotSize = Conf::UI_TOOL_BAR_SLOT_SIZE;

  float barWidth = (itemCount * slotSize) + padding;
  float barHeight = itemSize + (2 * padding);

  float barPosX = Conf::SCREEN_CENTER.x - (barWidth / 2.0f);
  float barPosY =
      Conf::SCREEN_HEIGHT - barHeight - Conf::UI_TOOL_BAR_Y_BOTTOM_MARGIN;

  this->toolBarRect = {barPosX, barPosY, barWidth, barHeight};

  DrawRectangleRec(this->toolBarRect, Fade(GRAY, 0.8f));

  for (int i = 0; i < itemCount; ++i) {
    float slotPosX = barPosX + padding + (i * slotSize);
    float slotPosY = barPosY + padding;

    // Draw slot border
    if (i == selectedItemIndex) {
      DrawRectangleLinesEx(
          {slotPosX, slotPosY, (float)itemSize, (float)itemSize}, 3, YELLOW);
    } else {
      DrawRectangleLines(slotPosX, slotPosY, itemSize, itemSize, DARKGRAY);
    }

    if (itemHandler) {
      ItemID currentTile = itemHandler->GetToolBarItemType(i);
      if (currentTile != ITEM_NULL) {
        if (textureHandler) {
          Rectangle tile_rect = {(float)Conf::TA_ITEM_X_OFFSET,
                                 (float)Conf::ASSEST_RESOLUTION * currentTile,
                                 (float)Conf::ASSEST_RESOLUTION,
                                 (float)Conf::TILE_SIZE};

          Rectangle dest_rect = {slotPosX, slotPosY, (float)itemSize,
                                 (float)itemSize};

          textureHandler->Draw(tile_rect, dest_rect, {0, 0}, 0.0f, WHITE);
        }
      }
    }
  }
}

void UI_Handler::SetToolBarActive(bool is_active) {
  isToolBarActive = is_active;
}

bool UI_Handler::GetToolBarStatus() { return isToolBarActive; }

Rectangle UI_Handler::GetToolBarRect() { return this->toolBarRect; }

int UI_Handler::GetItemSlotAt(Vector2 point) {
  if (!isToolBarActive) {
    return -1;
  }

  const int itemCount = Conf::ITEM_STACK_MAX_TOOL_BAR;
  const float padding = Conf::UI_TOOL_BAR_PADDING;
  const int itemSize = Conf::UI_TOOL_SIZE;
  const int slotSize = Conf::UI_TOOL_BAR_SLOT_SIZE;

  float barPosX = this->toolBarRect.x;
  float barPosY = this->toolBarRect.y;

  for (int i = 0; i < itemCount; ++i) {
    float slotPosX = barPosX + padding + (i * slotSize);
    float slotPosY = barPosY + padding;
    Rectangle slotRect = {slotPosX, slotPosY, (float)itemSize, (float)itemSize};
    if (CheckCollisionPointRec(point, slotRect)) {
      return i;
    }
  }

  return -1;
}
