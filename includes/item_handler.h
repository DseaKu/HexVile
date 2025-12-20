#ifndef ITEM_HANDLER_H
#define ITEM_HANDLER_H

#include "enums.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

// --- Structs ---
struct ItemProperties {
  std::string name;
  int maxStack;
  int value;
  bool placeableTile;
};

struct Item {
  ItemID id;
  int count;
};

struct Chest {
  std::vector<Item> contents;
};

// Global chest map (Consider moving this to a dedicated World/ChestHandler later)
extern std::map<std::pair<int, int>, Chest> gridChests;

// --- Item Database ---
class ItemDataBase {
private:
  std::vector<ItemProperties> properties;

public:
  ItemDataBase();
  const ItemProperties &GetItemProperties(ItemID id) const;
};

// --- Item Handler ---
class ItemHandler {
private:
  std::vector<Item> inventory;
  std::vector<Item> toolBar;
  ItemDataBase itemDataBase;
  int selectedToolBarSlot;

  void Init();

public:
  ItemHandler();

  // Actions
  bool TakeItemFromToolBar(Item *item, int amount);

  // Setters
  void SetItemSelection(int pos);

  // Getters
  Item *GetToolBarItemPointer(int pos);
  int GetSelectionToolBar() const;
  ItemID GetToolBarItemType(int pos) const;
  const char *GetSelectedItemType() const;

  // Conversion / Helpers
  ItemID ToolBarSelectionToItemId(int sel) const;
  TileID ConvertItemToTileID(ItemID item_id) const;
};

#endif // !ITEM_HANDLER_H
