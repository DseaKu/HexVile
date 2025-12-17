#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "GFX_manager.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "io_handler.h"
#include "item_handler.h"
#include "raylib.h"
#include <vector>

class UI_Handler {
private:
  int nToolBarItemMax;
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
  GFX_Manager *graphicsManager;
  ItemHandler *itemHandler;
  FontHandler *fontHandler;
  IO_Handler *io_Handler;
  HexGrid *hexGrid;
  int selectedItemIndex;
  bool isToolBarActive;
  void GenerateDrawData();
  void LoadToolBarGFX();
  void LoadHiTileGFX();
  void LoadItemBgGFX(int x, int y);
  void LoadItemGFX(int x, int y);
  void LoadItemNumGFX(int x, int y);

public:
  UI_Handler();
  void Update();
  void SetGFX_Manager(GFX_Manager *graphicsManager);
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
