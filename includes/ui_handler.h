#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "enums.h"
#include "raylib.h"
#include "texture_handler.h"
#include <vector>

class UIHandler {
private:
  float scale;
  TextureHandler *textureHandler;
  std::vector<TileID> itemBarSlots;
  int selectedItemIndex;

public:
  UIHandler();
  void Init();
  void SetTextureHandler(TextureHandler *th);
  void SetSelectedItem(int index);
  void Draw();
  bool CheckClick(Vector2 mousePosition);
  TileID GetSelectedItem();
};

#endif // UI_HANDLER_H
