#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "GFX_manager.h"
#include "font_handler.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "raylib.h"

class UI_Handler {
private:
  struct ToolBarLayout {
    int maxSlots;
    int slotSize;
    int contentSize;
    float padding;
    float bottomMargin;
    float scale;
    float itemScale;

    // Calculated params
    float width;
    float height;
    float posX;
    float posY;
    Rectangle rect;
  };

  ToolBarLayout toolBarLayout;
  bool isToolBarActive;
  int selToolBarSlot;

  // Dependencies
  GFX_Manager *graphicsManager;
  ItemHandler *itemHandler;
  FontHandler *fontHandler;
  HexGrid *hexGrid;

  // Render Helpers
  void DrawHighlighedTile(Vector2 mouseWorldPos);
  void DrawToolBar();
  void DrawToolBarSlot(int slotIndex);
  void DrawItemIcon(int slotIndex, Rectangle slotRect);
  void DrawItemCount(int slotIndex, Rectangle slotRect);

public:
  UI_Handler();

  // Core
  void Update(Vector2 mouseWorldPos);
  void UpdateScreenSize(int width, int height);

  // Setters
  void SetGFX_Manager(GFX_Manager *p);
  void SetItemHandler(ItemHandler *p);
  void SetFontHandler(FontHandler *p);
  void SetHexGrid(HexGrid *p);
  void SetSelToolBarSlot(int index);
  void SetToolBarActive(bool is_active);

  // Queries
  bool GetToolBarAvailability();
  Rectangle GetToolBarRect();

  // Getter
  int GetItemSlotAt(Vector2 screenPos);
  int GetSelToolBarSlot();

  // Input
  int GetToolBarSelection(KeyboardInput keyPress, int currentSelection);
};

#endif // UI_HANDLER_H
