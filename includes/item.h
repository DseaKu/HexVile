#ifndef ITEM_H
#define ITEM_H

#include "enums.h"
#include <map>
#include <utility>
#include <vector>

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
  ItemProperties GetItemProperties(ItemID id);
};

// --- Item Handler ---
class ItemHandler {
private:
  std::vector<Item> inventory;
  std::vector<Item> toolBar;
  ItemDataBase itemDataBase;
  int selectedToolBarSlot;

public:
  ItemHandler();
  void Init();

  // --- Conversion ---
  ItemID ToolBarSelctionToItemId(int sel);

  // --- Get ---
  Item *GetToolBarItemPointer(int pos);
  int GetSelectionToolBar();
  ItemID GetToolBarItemType(int pos);
  const char *GetSelectedItemType();

  // --- Set ---
  void SetItemSelection(int pos);
};

#endif // !ITEM_H
