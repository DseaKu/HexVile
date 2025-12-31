#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include "GFX_manager.h"
#include "font_handler.h"
#include "frame_context.h"
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
  bool isInventoryOpen;

  // Dependencies
  GFX_Manager *graphicsManager;
  ItemHandler *itemHandler;
  FontHandler *fontHandler;
  HexGrid *hexGrid;
  const frame::Context *frameContext = nullptr;

  // Render Helpers
  void LoadHighlightGFX();
  void LoadHighlightTileGFX();
  void LoadHighlightResourceGFX(rsrc::ID id);

  void LoadToolBarGFX();
  void LoadInventoryBackgroundGFX();
  void LoadInventoryItemsGFX();
  void LoadItemSlotGFX(int slotIndex);
  void LoadItemIconGFX(int slotIndex, Rectangle slotRect);
  void LoadItemCountGFX(int slotIndex, Rectangle slotRect);

public:
  UI_Handler();

  // Core
  void Update();
  void LoadBackBuffer();
  void UpdateScreenSize(int width, int height);
  mouseMask::id UpdateMouseMask();
  void ToggleInventory();

  // Setters
  void SetGFX_Manager(GFX_Manager *p);
  void SetItemHandler(ItemHandler *p);
  void SetFontHandler(FontHandler *p);
  void SetHexGrid(HexGrid *p);
  void SetToolBarActive(bool is_active);
  void SetFrameContext(const frame::Context *curFrameContext);

  // Queries
  bool GetToolBarAvailability();
  Rectangle GetToolBarRect();

  // Getter
  int GetToolBarSelection();
};

#endif // UI_HANDLER_H
