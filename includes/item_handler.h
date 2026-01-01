#ifndef ITEM_HANDLER_H
#define ITEM_HANDLER_H

#include "enums.h"
#include "frame_context.h"
#include <vector>

// --- Structs ---
struct ItemStack {
  item::id itemID;
  int count;
};

typedef std::vector<ItemStack> ItemContainer;

// --- Item Handler ---
class ItemHandler {
private:
  // --- Members ---
  ItemContainer inventory;
  ItemContainer toolBar;
  const frame::Context *frameContext;

  // --- Private Methods ---
  void Init();

public:
  // --- Constructors ---
  ItemHandler();

  // --- Logic / Actions ---
  bool TakeItemFromToolBar(ItemStack *itemStack, int amount);
  bool AddItem(item::id itemID, int count);

  // --- Setters ---
  void SetFrameContext(const frame::Context *curFrameContext);

  // --- Getters ---
  ItemStack *GetInventoryItemPointer(int pos);
  ItemStack *GetToolBarItemPointer(int pos);
  item::id GetToolBarItemType(int pos) const;
  const char *GetSelectedItemType() const;

  ItemContainer *GetInventoryPointer() const;
  ItemContainer *GetToolBarPointer() const;

  // --- Conversions / Helpers ---
  item::id ToolBarSelectionToItemID(int sel) const;
  tile::id ConvertItemToTileID(item::id itemID) const;
};

#endif // !ITEM_HANDLER_H
