#include "ui_handler.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "raylib.h"

UI_Handler::UI_Handler() {

  itemCount = Conf::ITEM_STACK_MAX_TOOL_BAR;
  padding = Conf::UI_TOOL_BAR_PADDING;
  itemSize = Conf::UI_TOOL_SIZE;
  slotSize = Conf::UI_TOOL_BAR_SLOT_SIZE;
  barWidth = (itemCount * slotSize) + padding;
  barHeight = itemSize + (2 * padding);
  barPosX = Conf::SCREEN_CENTER.x - (barWidth / 2.0f);
  barPosY = Conf::SCREEN_HEIGHT - barHeight - Conf::UI_TOOL_BAR_Y_BOTTOM_MARGIN;
  this->toolBarRect = {barPosX, barPosY, barWidth, barHeight};

  scale = Conf::UI_SCALE;
  textureHandler = nullptr;
  itemHandler = nullptr;
  fontHandler = nullptr;
  selectedItemIndex = 0;
  isToolBarActive = false;
  toolBarRect = {.x = 0, .y = 0, .width = 0, .height = 0};
}

void UI_Handler::SetTextureHandler(TextureHandler *th) {
  this->textureHandler = th;
}

void UI_Handler::SetItemHandler(ItemHandler *ih) { this->itemHandler = ih; }

void UI_Handler::SetFontHandler(FontHandler *fh) { this->fontHandler = fh; }

void UI_Handler::SetSelectedItem(int index) {
  if (index >= 0 && index < 10) {
    selectedItemIndex = index;
  }
}

void UI_Handler::Update() { GenerateDrawData(); }
void UI_Handler::GenerateDrawData() {

  // DrawToolBar();
  DrawToolBarItems();
}

void UI_Handler::DrawToolBarItems() {
  if (!isToolBarActive) {
    return;
  }

  for (int i = 0; i < itemCount; ++i) {
    float slotPosX = barPosX + padding + (i * slotSize);
    float slotPosY = barPosY + padding;

    if (itemHandler) {
      ItemID currentTile = itemHandler->GetToolBarItemType(i);
      if (currentTile != ITEM_NULL) {
        if (textureHandler) {
          Rectangle tile_rect = {(float)TA::ITEM_X_OFFSET_TILE,
                                 (float)TA::ASSEST_RESOLUTION * currentTile,
                                 (float)TA::ASSEST_RESOLUTION,
                                 (float)Conf::TILE_SIZE};

          Rectangle dest_rect = {slotPosX, slotPosY, (float)itemSize,
                                 (float)itemSize};

          textureHandler->LoadDrawData(DRAW_MASK_UI, dest_rect.y, tile_rect,
                                       dest_rect, WHITE);

          // Draw item count
          if (itemHandler->GetToolBarItemPointer(i)->count > 0) {
            char countText[5];
            snprintf(countText, sizeof(countText), "%d",
                     itemHandler->GetToolBarItemPointer(i)->count);
            fontHandler->DrawTextHackRegular(
                countText,
                {slotPosX + itemSize -
                     (float)MeasureText(countText, Conf::FONT_SIZE_DEFAULT) - 5,
                 slotPosY + itemSize - Conf::FONT_SIZE_DEFAULT - 5},
                WHITE);
          }
        }
      }
    }
  }
}
void UI_Handler::DrawToolBar() {
  if (!isToolBarActive) {
    return;
  }

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
