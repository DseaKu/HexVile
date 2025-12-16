#include "ui_handler.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "io_handler.h"
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
  this->toolBarRec = {barPosX, barPosY, barWidth, barHeight};
  toolBarItemSize = Conf::UI_TOOL_BAR_ITEM_SIZE;

  itemBG_Rec = Rectangle{.x = TA::UI_X_OFFSET_TILE,
                         .y = UI_ID_ITEM_BAR_BACKGROUND * TA::ASSEST_RESOLUTION,
                         .width = TA::ASSEST_RESOLUTION,
                         .height = TA::ASSEST_RESOLUTION};
  tileHighlightRec =
      Rectangle{.x = TA::UI_X_OFFSET_TILE,
                .y = UI_ID_HIGHLIGHTED_TILE * TA::ASSEST_RESOLUTION,
                .width = TA::ASSEST_RESOLUTION,
                .height = TA::ASSEST_RESOLUTION};

  scale = Conf::UI_SCALE;
  textureHandler = nullptr;
  itemHandler = nullptr;
  fontHandler = nullptr;
  selectedItemIndex = 0;
  isToolBarActive = false;
  toolBarRec = {.x = 0, .y = 0, .width = 0, .height = 0};
}

void UI_Handler::SetTextureHandler(TextureHandler *p) {
  this->textureHandler = p;
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

  DrawToolBar();
  DrawToolBarItems();
  DrawTileHighlight();
}

void UI_Handler::DrawToolBarItems() {
  if (!isToolBarActive) {
    return;
  }

  for (int i = 0; i < itemCount; ++i) {
    float slotPosX = barPosX + padding + (i * slotSize);
    float slotPosY = barPosY + padding;

    ItemID currentTile = itemHandler->GetToolBarItemType(i);

    Rectangle dstRec = {slotPosX, slotPosY, (float)itemSize, (float)itemSize};

    // Load Item Background
    textureHandler->LoadDrawData(DRAW_MASK_UI, dstRec.y, this->itemBG_Rec,
                                 dstRec, WHITE);

    // Load Item
    if (currentTile != ITEM_NULL) {
      Rectangle srcRec = {(float)TA::ITEM_X_OFFSET_TILE,
                          (float)TA::ASSEST_RESOLUTION * currentTile,
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
      textureHandler->LoadDrawData(DRAW_MASK_UI, dstRec.y, srcRec, dstRec,
                                   WHITE);
    }
  }
}
void UI_Handler::DrawToolBar() {
  if (!isToolBarActive) {
    return;
  }
  DrawRectangleRec(this->toolBarRec, Fade(GRAY, 0.8f));
}
void UI_Handler::DrawTileHighlight() {
  Vector2 mousePos = io_Handler->GetScaledMousePos();
  HexCoord coord = hexGrid->PointToHexCoord(mousePos);
  hexGrid->DrawTile(coord, tileHighlightRec, DRAW_MASK_GROUND1);
}

void UI_Handler::SetToolBarActive(bool is_active) {
  isToolBarActive = is_active;
}

bool UI_Handler::GetToolBarStatus() { return isToolBarActive; }

Rectangle UI_Handler::GetToolBarRect() { return this->toolBarRec; }

int UI_Handler::GetItemSlotAt(Vector2 point) {
  if (!isToolBarActive) {
    return -1;
  }

  const int itemCount = Conf::ITEM_STACK_MAX_TOOL_BAR;
  const float padding = Conf::UI_TOOL_BAR_PADDING;
  const int itemSize = Conf::UI_TOOL_SIZE;
  const int slotSize = Conf::UI_TOOL_BAR_SLOT_SIZE;

  float barPosX = this->toolBarRec.x;
  float barPosY = this->toolBarRec.y;

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
