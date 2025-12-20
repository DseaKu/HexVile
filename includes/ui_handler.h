#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "GFX_manager.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "input_handler.h"
#include "item_handler.h"
#include "raylib.h"

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

  float scale;
  GFX_Manager *graphicsManager;
  ItemHandler *itemHandler;
  FontHandler *fontHandler;
  InputHandler *inputHandler;
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
  void SetGFX_Manager(GFX_Manager *p);
  void SetItemHandler(ItemHandler *p);
  void SetFontHandler(FontHandler *p);
  void SetInputHandler(InputHandler *p);
  void SetHexGrid(HexGrid *p);
  void SetSelectedItem(int index);
  void SetToolBarActive(bool is_active);
  bool CheckClick(Vector2 mousePosition);
  bool GetToolBarAvailability();
  Rectangle GetToolBarRect();
  int GetItemSlotAt(Vector2 point);
  void UpdateScreenSize(int width, int height);
};

#endif // UI_HANDLER_H
