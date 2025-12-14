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
  int selectionToolBar;

public:
  ItemHandler();
  void Init();

  // --- Conversion ---
  ItemID ToolBarSelctionToItemId(int sel);
  const char *ToolBarSelctionToString(int sel);
  const char *ItemToString(ItemID id);
  //
  // --- Get/Set---
  Item *GetToolBarItemPointer(int pos);
  ItemID GetToolBarItemType(int pos);
  int GetSelectionToolBar();
  void SetItemSelection(int pos);
};

#endif // !ITEM_H
