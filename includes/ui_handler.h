#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "font_handler.h"
#include "item_handler.h"
#include "raylib.h"
#include "texture_handler.h"

class UI_Handler {
private:
  int itemCount;
  float padding;
  int itemSize;
  int slotSize;
  float barPosY;
  float barPosX;
  float barWidth;
  float barHeight;
  Rectangle toolBarRect;
  Rectangle itemBackground;

  float scale;
  TextureHandler *textureHandler;
  ItemHandler *itemHandler;
  FontHandler *fontHandler;
  int selectedItemIndex;
  bool isToolBarActive;
  void GenerateDrawData();

public:
  UI_Handler();
  void Update();
  void SetTextureHandler(TextureHandler *th);
  void SetItemHandler(ItemHandler *ih);
  void SetFontHandler(FontHandler *fh);
  void SetSelectedItem(int index);
  void DrawToolBar();
  void DrawToolBarItems();
  bool CheckClick(Vector2 mousePosition);
  void SetToolBarActive(bool is_active);
  bool GetToolBarStatus();
  Rectangle GetToolBarRect();
  int GetItemSlotAt(Vector2 point);
};

#endif // UI_HANDLER_H
