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
  std::vector<TileID> itemBarSlots;
  int selectedItemIndex;
  bool isItemBarActive;
  Rectangle itemBarRect;

public:
  UI_Handler();
  void Init();
  void SetTextureHandler(TextureHandler *th);
  void SetSelectedItem(int index);
  void DrawItemBar();
  bool CheckClick(Vector2 mousePosition);
  TileID GetSelectedItem();
  void SetItemBarActive(bool is_active);
  bool GetItemBarStatus();
  Rectangle GetItemBarRect();
  int GetItemSlotAt(Vector2 point);
};

#endif // UI_HANDLER_H
