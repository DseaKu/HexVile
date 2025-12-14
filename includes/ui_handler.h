#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "item.h"
#include "raylib.h"
#include "texture_handler.h"

class UI_Handler {
private:
  float scale;
  TextureHandler *textureHandler;
  ItemHandler *itemHandler;
  int selectedItemIndex;
  bool isToolBarActive;
  Rectangle toolBarRect;

public:
  UI_Handler();
  void SetTextureHandler(TextureHandler *th);
  void SetItemHandler(ItemHandler *ih);
  void SetSelectedItem(int index);
  void DrawToolBar();
  bool CheckClick(Vector2 mousePosition);
  void SetToolBarActive(bool is_active);
  bool GetToolBarStatus();
  Rectangle GetToolBarRect();
  ItemID GetItemSlotAt(Vector2 point);
};

#endif // UI_HANDLER_H
