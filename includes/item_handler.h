#ifndef ITEM_HANDLER_H
#define ITEM_HANDLER_H

#include "enums.h"
#include "frame_context.h"
#include <string>
#include <vector>

// --- Structs ---
struct ItemProperties {
  std::string name;
  int maxStack;
  int value;
  bool placeableTile;
};

struct ItemStack {
  item::id itemID;
  int count;
};

// --- Item Database ---
class ItemDataBase {
private:
  std::vector<ItemProperties> properties;

public:
  ItemDataBase();
  const ItemProperties &GetItemProperties(item::id itemID) const;
};

// --- Item Handler ---
class ItemHandler {
private:
  std::vector<ItemStack> inventory;
  std::vector<ItemStack> toolBar;
  ItemDataBase itemDataBase;
  const frame::Context *frameContext;

  void Init();

public:
  ItemHandler();

  // Actions
  bool TakeItemFromToolBar(ItemStack *itemStack, int amount);
  bool AddItem(item::id itemID, int count);

  // Setters
  void SetFrameContext(const frame::Context *curFrameContext);

  // Getters
  ItemStack *GetToolBarItemPointer(int pos);
  item::id GetToolBarItemType(int pos) const;
  const char *GetSelectedItemType() const;

  // Conversion / Helpers
  item::id ToolBarSelectionToItemID(int sel) const;
  tile::id ConvertItemToTileID(item::id itemID) const;
};

#endif // !ITEM_HANDLER_H
