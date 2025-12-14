#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "enums.h"
#include "raylib.h"
#include "texture_handler.h"
#include <vector>

class UI_Handler {
private:
  float scale;
  TextureHandler *textureHandler;
  std::vector<TileID> toolBarSlots;
  int selectedItemIndex;
  bool isToolBarActive;
  Rectangle toolBarRect;

public:
  UI_Handler();
  void Init();
  void SetTextureHandler(TextureHandler *th);
  void SetSelectedItem(int index);
  void DrawToolBar();
  bool CheckClick(Vector2 mousePosition);
  TileID GetSelectedItem();
  void SetToolBarActive(bool is_active);
  bool GetToolBarStatus();
  Rectangle GetToolBarRect();
  int GetItemSlotAt(Vector2 point);
};

#endif // UI_HANDLER_H
