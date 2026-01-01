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
    // int maxSlots;
    // int slotSize;
    // int contentSize;
    // float padding;
    // float bottomMargin;
    // float scale;
    // float itemScale;
    //
    // // Calculated params
    // float width;
    // float height;
    // float posX;
    // float posY;
    // Rectangle rect;
  };

  // --- Members ---
  ToolBarLayout toolBarLayout;
  bool isToolBarActive;
  bool isInventoryOpen;
  float ToolBarInventorySpace;

  // --- Dependencies ---
  GFX_Manager *graphicsManager;
  ItemHandler *itemHandler;
  FontHandler *fontHandler;
  HexGrid *hexGrid;
  const frame::Context *frameContext = nullptr;

  // --- Private Methods ---
  void LoadHighlightGFX();
  void LoadHighlightTileGFX();
  void LoadHighlightResourceGFX(rsrc::ID id);

  void LoadToolBarGFX();
  void LoadItemSlotBackgroundGFX(Vector2 dst);
  void LoadItemSlotHighlightGFX(Vector2 dst);
  void LoadInventoryBackgroundGFX();
  void LoadItemSlotGFX(const ItemStack *item, Vector2 centerPos,
                       bool isSelected);
  void LoadInventoryItemsGFX();
  void LoadItemCountGFX(const ItemStack *item, Vector2 dst);
  void LoadItemIconGFX(tex::atlas::Coords taCoords, Vector2 dst);

public:
  // --- Constructors ---
  UI_Handler();

  // --- Core Lifecycle ---
  void Update();
  void UpdateScreenSize(int width, int height);
  mouseMask::id UpdateMouseMask();
  void ToggleInventory();

  // --- Graphics / Backbuffer ---
  void LoadBackBuffer();

  // --- Setters ---
  void SetGFX_Manager(GFX_Manager *p);
  void SetItemHandler(ItemHandler *p);
  void SetFontHandler(FontHandler *p);
  void SetHexGrid(HexGrid *p);
  void SetToolBarActive(bool is_active);
  void SetFrameContext(const frame::Context *curFrameContext);

  // --- Getters & Queries ---
  bool GetToolBarAvailability();
  Rectangle GetToolBarRect();
  int GetToolBarSelection();
};

#endif // UI_HANDLER_H
