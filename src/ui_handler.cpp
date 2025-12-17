#include "ui_handler.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "io_handler.h"
#include "raylib.h"
#include <string>

UI_Handler::UI_Handler() {

  nToolBarItemMax = Conf::ITEM_STACK_MAX_TOOL_BAR;
  padding = Conf::UI_TOOL_BAR_PADDING;
  itemSize = Conf::UI_TOOL_SIZE;
  slotSize = Conf::UI_TOOL_BAR_SLOT_SIZE;
  barWidth = (nToolBarItemMax * slotSize) + padding;
  barHeight = itemSize + (2 * padding);
  barPosX = Conf::SCREEN_CENTER.x - (barWidth / 2.0f);
  barPosY = Conf::SCREEN_HEIGHT - barHeight - Conf::UI_TOOL_BAR_Y_BOTTOM_MARGIN;
  this->toolBarRec = {barPosX, barPosY, barWidth, barHeight};
  toolBarItemSize = Conf::UI_TOOL_BAR_ITEM_SIZE;
  scale = Conf::UI_SCALE;
  graphicsManager = nullptr;
  itemHandler = nullptr;
  fontHandler = nullptr;
  selectedItemIndex = 0;
  isToolBarActive = false;

  itemBG_Rec = Rectangle{.x = TA::UI_X_OFFSET_TILE,
                         .y = UI_ID_ITEM_BAR_BACKGROUND * TA::ASSEST_RESOLUTION,
                         .width = TA::ASSEST_RESOLUTION,
                         .height = TA::ASSEST_RESOLUTION};

  itemBG_Rec_h = Rectangle{.x = TA::UI_X_OFFSET_TILE,
                           .y = UI_ID_ITEM_BAR_BACKGROUND_HIGHLIGHTED *
                                TA::ASSEST_RESOLUTION,
                           .width = TA::ASSEST_RESOLUTION,
                           .height = TA::ASSEST_RESOLUTION};
  tileHighlightRec =
      Rectangle{.x = TA::UI_X_OFFSET_TILE,
                .y = UI_ID_TILE_HIGHLIGHTED * TA::ASSEST_RESOLUTION,
                .width = TA::ASSEST_RESOLUTION,
                .height = TA::ASSEST_RESOLUTION};

  numRec.resize(10, nullRec);
  for (int i = 0; i < 10; i++) {
    numRec[i] = Rectangle{.x = TA::NUMBER_X_OFFSET,
                          .y = ((float)i + 1.0f) * TA::ASSEST_RESOLUTION,
                          .width = TA::ASSEST_RESOLUTION,
                          .height = TA::ASSEST_RESOLUTION};
  }

  toolBarRec = nullRec;
}

void UI_Handler::SetGFX_Manager(GFX_Manager *graphicsManager) {
  this->graphicsManager = graphicsManager;
}
void UI_Handler::SetItemHandler(ItemHandler *p) { this->itemHandler = p; }
void UI_Handler::SetFontHandler(FontHandler *p) { this->fontHandler = p; }
void UI_Handler::SetIO_Handler(IO_Handler *p) { this->io_Handler = p; }
void UI_Handler::SetHexGrid(HexGrid *p) { this->hexGrid = p; }

void UI_Handler::SetSelectedItem(int index) {
  if (index >= 0 && index < 10) {
    selectedItemIndex = index;
  }
}

void UI_Handler::Update() { GenerateDrawData(); }
void UI_Handler::GenerateDrawData() {

  LoadHiTileGFX();
  LoadToolBarGFX();
}

void UI_Handler::LoadToolBarGFX() {
  if (!isToolBarActive) {
    return;
  }

  // DrawToolBar
  for (int i = 0; i < nToolBarItemMax; i++) {
    LoadItemBgGFX(i, 0);
    LoadItemGFX(i, 0);
    LoadItemNumGFX(i, 0);
  }
}
void UI_Handler::LoadItemBgGFX(int x, int y) {

  // Load item background
  float slotPosX = barPosX + padding + (x * slotSize);
  float slotPosY = barPosY + padding;

  Item *item = itemHandler->GetToolBarItemPointer(x);

  Rectangle dstRec = {slotPosX, slotPosY, (float)itemSize, (float)itemSize};

  // Load item background
  graphicsManager->LoadGFX_Data(DRAW_MASK_UI_0, dstRec.y, this->itemBG_Rec,
                                dstRec, WHITE);
  // Highlight, if selected
  if (x == itemHandler->GetSelectionToolBar()) {
    graphicsManager->LoadGFX_Data(DRAW_MASK_UI_0, dstRec.y, this->itemBG_Rec_h,
                                  dstRec, WHITE);
  }
}

void UI_Handler::LoadItemGFX(int x, int y) {

  // Load item graphics
  Item *item = itemHandler->GetToolBarItemPointer(x);
  if (item->id != ITEM_NULL) {

    float slotPosX = barPosX + padding + (x * slotSize);
    float slotPosY = barPosY + padding;
    Rectangle dstRec = {slotPosX, slotPosY, (float)itemSize, (float)itemSize};
    Rectangle srcRec = {(float)TA::ITEM_X_OFFSET_TILE,
                        (float)TA::ASSEST_RESOLUTION * item->id,
                        (float)TA::ASSEST_RESOLUTION, (float)Conf::TILE_SIZE};

    // Shrink item
    float newWidth = dstRec.width * toolBarItemSize;
    float newHeight = dstRec.height * toolBarItemSize;
    float offsetX = (dstRec.width - newWidth) / 2.0f;
    float offsetY = (dstRec.height - newHeight) / 2.0f;

    dstRec = Rectangle{.x = dstRec.x + offsetX,
                       .y = dstRec.y + offsetY,
                       .width = newWidth,
                       .height = newHeight};
    graphicsManager->LoadGFX_Data(DRAW_MASK_UI_0, dstRec.y, srcRec, dstRec,
                                  WHITE);
  }
}

void UI_Handler::LoadItemNumGFX(int x, int y) {

  Item *item = itemHandler->GetToolBarItemPointer(x);
  float slotPosX = barPosX + padding + (x * slotSize);
  float slotPosY = barPosY + padding;
  Rectangle dstRec = {slotPosX, slotPosY, (float)itemSize, (float)itemSize};

  if (item->id != ITEM_NULL) {

    // Load Item GFX
    Rectangle srcRec = {(float)TA::ITEM_X_OFFSET_TILE,
                        (float)TA::ASSEST_RESOLUTION * item->id,
                        (float)TA::ASSEST_RESOLUTION, (float)Conf::TILE_SIZE};

    // Shrink item
    float newWidth = dstRec.width * toolBarItemSize;
    float newHeight = dstRec.height * toolBarItemSize;
    float offsetX = (dstRec.width - newWidth) / 2.0f;
    float offsetY = (dstRec.height - newHeight) / 2.0f;

    dstRec = Rectangle{.x = dstRec.x + offsetX,
                       .y = dstRec.y + offsetY,
                       .width = newWidth,
                       .height = newHeight};
    graphicsManager->LoadGFX_Data(DRAW_MASK_UI_1, dstRec.y, srcRec, dstRec,
                                  WHITE);

    // Draw Numbers
    std::string num_str = std::to_string(item->count);
    int i = 0;
    for (auto it = num_str.rbegin(); it != num_str.rend(); ++it) {
      int digit = *it - '0';

      Vector2 rbCorner =
          Vector2{.x = dstRec.x + dstRec.width, .y = dstRec.y + dstRec.height};

      Rectangle digit_dst_rec = {.x = rbCorner.x - (i * TA::NUMBER_SCALE),
                                 .y = rbCorner.y,
                                 .width = -TA::NUMBER_SCALE,
                                 .height = -TA::NUMBER_SCALE};

      graphicsManager->LoadGFX_Data(DRAW_MASK_UI_1, digit_dst_rec.y,
                                    numRec[digit], digit_dst_rec, WHITE);
      i++;
    }
  }
}
void UI_Handler::LoadHiTileGFX() {
  Vector2 mousePos = io_Handler->GetScaledMousePos();
  HexCoord coord = hexGrid->PointToHexCoord(mousePos);
  hexGrid->DrawTile(coord, tileHighlightRec, DRAW_MASK_GROUND_1);
}

void UI_Handler::SetToolBarActive(bool is_active) {
  isToolBarActive = is_active;
}

bool UI_Handler::GetToolBarAvailability() { return isToolBarActive; }

Rectangle UI_Handler::GetToolBarRect() { return this->toolBarRec; }

int UI_Handler::GetItemSlotAt(Vector2 point) {
  if (!isToolBarActive) {
    return -1;
  }

  float barPosX = this->toolBarRec.x;
  float barPosY = this->toolBarRec.y;

  for (int i = 0; i < nToolBarItemMax; ++i) {
    float slotPosX = barPosX + padding + (i * slotSize);
    float slotPosY = barPosY + padding;
    Rectangle slotRect = {slotPosX, slotPosY, (float)itemSize, (float)itemSize};
    if (CheckCollisionPointRec(point, slotRect)) {
      return i;
    }
  }

  return -1;
}
