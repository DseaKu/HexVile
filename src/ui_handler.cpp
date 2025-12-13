#include "ui_handler.h"
#include "defines.h"
#include "raylib.h"

UI_Handler::UI_Handler() {
  scale = Conf::UI_SCALE;
  textureHandler = nullptr;
  selectedItemIndex = 0; // Default to first item
}

void UI_Handler::Init() {
  itemBarSlots.assign(10, TILE_VOID);
  itemBarSlots[0] = TILE_GRASS;
  itemBarSlots[1] = TILE_WATER;
}

void UI_Handler::SetTextureHandler(TextureHandler *th) {
  this->textureHandler = th;
}

void UI_Handler::SetSelectedItem(int index) {
  if (index >= 0 && index < 10) {
    selectedItemIndex = index;
  }
}

void UI_Handler::DrawItemBar() {
  if (!isItemBarActive) {
    return;
  }
  const int itemCount = Conf::N_ELEMENT_ITEM_BAR * Conf::UI_SCALE;
  const float padding = Conf::UI_ITEM_BAR_PADDING * Conf::UI_SCALE;
  const int itemSize = Conf::ASSEST_RESOLUTION * Conf::UI_SCALE;
  const int slotSize = itemSize + padding * Conf::UI_SCALE;

  float barWidth = (itemCount * slotSize) + padding;
  float barHeight = itemSize + (2 * padding);

  float barPosX = Conf::SCREEN_CENTER.x - (barWidth / 2.0f);
  float barPosY =
      Conf::SCREEN_HEIGHT - barHeight - Conf::UI_ITEM_BAR_Y_BOTTOM_MARGIN;

  this->itemBarRect = {barPosX, barPosY, barWidth, barHeight};

  DrawRectangleRec(this->itemBarRect, Fade(GRAY, 0.8f));

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

    TileID currentTile = itemBarSlots[i];
    if (currentTile != TILE_VOID && currentTile != TILE_NULL) {
      if (textureHandler) {
        Rectangle tile_rect = {(float)Conf::TEXTURE_ATLAS_TILES_X_OFFSET,
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
void UI_Handler::SetItemBarActive(bool status) { isItemBarActive = status; }

bool UI_Handler::GetItemBarStatus() { return isItemBarActive; }
Rectangle UI_Handler::GetItemBarRect() { return this->itemBarRect; }
