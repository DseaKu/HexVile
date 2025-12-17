#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "font_handler.h"
#include "hex_tile_grid.h"
#include "io_handler.h"
#include "item_handler.h"
#include "raylib.h"
#include "graphics_manager.h"
#include <vector>

class UI_Handler {
private:
  int itemCount;
  int itemSize;
  int slotSize;
  float toolBarItemSize;
  float padding;
  float barPosY;
  float barPosX;
  float barWidth;
  float barHeight;
  Rectangle toolBarRec;
  Rectangle itemBG_Rec;
  Rectangle itemBG_Rec_h;
  Rectangle tileHighlightRec;
  std::vector<Rectangle> numRec;

  float scale;
  GraphicsManager *graphicsManager;
  ItemHandler *itemHandler;
  FontHandler *fontHandler;
  IO_Handler *io_Handler;
  HexGrid *hexGrid;
  int selectedItemIndex;
  bool isToolBarActive;
  void GenerateDrawData();
  void DrawToolBar();
  void DrawToolBarItems();
  void DrawTileHighlight();

public:
  UI_Handler();
  void Update();
  void SetGraphicsManager(GraphicsManager *graphicsManager);
  void SetItemHandler(ItemHandler *p);
  void SetFontHandler(FontHandler *p);
  void SetIO_Handler(IO_Handler *p);
  void SetHexGrid(HexGrid *p);
  void SetSelectedItem(int index);
  void SetToolBarActive(bool is_active);
  bool CheckClick(Vector2 mousePosition);
  bool GetToolBarAvailability();
  Rectangle GetToolBarRect();
  int GetItemSlotAt(Vector2 point);
};

#endif // UI_HANDLER_H
