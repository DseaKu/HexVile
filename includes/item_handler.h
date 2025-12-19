#ifndef ITEM_HANDLER_H
#define ITEM_HANDLER_H

#include "enums.h"
#include <map>
#include <utility>
#include <vector>
#include <string>

typedef struct ItemProperties {
  std::string name;
  int maxStack;
  int value;
  bool placeableTile;
} ItemProperties;

typedef struct Item {
  ItemID id;
  int count;
} Item;

struct Chest {
  // std::string name;
  std::vector<Item> contents;
};

extern std::map<std::pair<int, int>, Chest> gridChests;

// --- Item Data Base ---
class ItemDataBase {
private:
  std::vector<ItemProperties> propeties;

public:
  ItemDataBase();
  const ItemProperties& GetItemProperties(ItemID id);
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
  bool TakeItemFromToolBar(Item *item, int amount);

  // --- Conversion ---
  ItemID ToolBarSelctionToItemId(int sel);
  TileID ConvertItemToTileID(ItemID item_id);

  // --- Get ---
  Item *GetToolBarItemPointer(int pos);
  int GetSelectionToolBar();
  ItemID GetToolBarItemType(int pos);
  const char *GetSelectedItemType();

  // --- Set ---
  void SetItemSelection(int pos);
};

#endif // !ITEM_HANDLER_H
