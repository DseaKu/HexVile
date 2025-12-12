#include "ui_handler.h"
#include "defines.h"
#include "raylib.h"

UIHandler::UIHandler() {
  scale = Conf::UI_SCALE;
  textureHandler = nullptr;
  selectedItemIndex = 0; // Default to first item
}

void UIHandler::Init() {
  itemBarSlots.assign(10, TILE_VOID);
  itemBarSlots[0] = TILE_GRASS;
  itemBarSlots[1] = TILE_WATER;
}

void UIHandler::SetTextureHandler(TextureHandler *th) {
  this->textureHandler = th;
}

void UIHandler::SetSelectedItem(int index) {
  if (index >= 0 && index < 10) {
    selectedItemIndex = index;
  }
}

void UIHandler::Draw() {
  const int itemCount = 10;
  const int padding = 10;
  const int itemSize = Conf::ASSEST_RESOLUTION;
  const int slotSize = itemSize + padding;

  int barWidth = (itemCount * slotSize) + padding;
  int barHeight = itemSize + (2 * padding);

  float barPosX = Conf::SCREEN_CENTER.x - (barWidth / 2.0f);
  float barPosY = Conf::SCREEN_HEIGHT - barHeight - 20; // 20px margin from bottom

  DrawRectangle(barPosX, barPosY, barWidth, barHeight,
                Fade(GRAY, 0.8f)); // Background bar

  for (int i = 0; i < itemCount; ++i) {
    float slotPosX = barPosX + padding + (i * slotSize);
    float slotPosY = barPosY + padding;

    // Draw slot border
    if (i == selectedItemIndex) {
      DrawRectangleLinesEx({slotPosX, slotPosY, (float)itemSize, (float)itemSize}, 3, YELLOW);
    } else {
      DrawRectangleLines(slotPosX, slotPosY, itemSize, itemSize, DARKGRAY);
    }

    TileID currentTile = itemBarSlots[i];
    if (currentTile != TILE_VOID && currentTile != TILE_NULL) {
      if (textureHandler) {
        Rectangle tile_rect = {
            (float)Conf::TEXTURE_ATLAS_TILES_X_OFFSET,
            (float)Conf::ASSEST_RESOLUTION * currentTile,
            (float)Conf::ASSEST_RESOLUTION, (float)Conf::TILE_SIZE};

        Rectangle dest_rect = {slotPosX, slotPosY, (float)itemSize,
                               (float)itemSize};

        textureHandler->Draw(tile_rect, dest_rect, {0, 0}, 0.0f, WHITE);
      }
    }
  }
}
