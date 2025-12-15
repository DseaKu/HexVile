#include "item_handler.h"
#include "defines.h"
#include "enums.h"

// --- Item Data Base ---
ItemDataBase::ItemDataBase() {
  propeties.resize(ITEM_ID_SIZE);
  propeties[ITEM_NULL] = {
      .name = "Null", .maxStack = 0, .value = 0, .placeableTile = false};
  propeties[ITEM_SET_GRASS] = {
      .name = "Set Grass", .maxStack = 32, .value = 5, .placeableTile = true};
  propeties[ITEM_SET_WATER] = {
      .name = "Set Water", .maxStack = 32, .value = 8, .placeableTile = true};
  propeties[ITEM_SET_DIRT] = {
      .name = "Set Dirt", .maxStack = 32, .value = 8, .placeableTile = true};
}

ItemProperties ItemDataBase::GetItemProperties(ItemID id) {
  return propeties[id];
}

// --- Item Handler ---
ItemHandler::ItemHandler() {
  selectedToolBarSlot = 0;
  Init();
}

void ItemHandler::Init() {
  Item itemNull = {.id = ITEM_NULL, .count = 0};
  inventory.assign(Conf::ITEM_STACK_MAX_INVENTORY, itemNull);
  toolBar.assign(Conf::ITEM_STACK_MAX_TOOL_BAR, itemNull);

  Item grass = {.id = ITEM_SET_GRASS, .count = 32};
  Item water = {.id = ITEM_SET_WATER, .count = 51};
  Item dirt = {.id = ITEM_SET_DIRT, .count = 8};
  Item dirt2 = {.id = ITEM_SET_DIRT, .count = 24};
  toolBar[0] = dirt;
  toolBar[1] = water;
  toolBar[2] = dirt2;
  toolBar[3] = grass;
  toolBar[5] = grass;
}

bool ItemHandler::TakeItemFromToolBar(Item *item, int amount) {
  if (item->count >= amount) {
    item->count -= amount;
    if (item->count == 0) {
      item->id = ITEM_NULL;
    }
    return true;
  }
  return false;
}

// --- Conversion ---
ItemID ItemHandler::ToolBarSelctionToItemId(int sel) {
  return toolBar[sel].id;
};

TileID ItemHandler::ConvertItemToTileID(ItemID item_id) {
  auto it = item_to_tile_map.find(item_id);
  if (it != item_to_tile_map.end()) {
    return it->second;
  }
  return TILE_NULL;
}

// --- Get ---
int ItemHandler::GetSelectionToolBar() { return selectedToolBarSlot; }

Item *ItemHandler::GetToolBarItemPointer(int pos) { return &toolBar[pos]; }

ItemID ItemHandler::GetToolBarItemType(int pos) { return toolBar[pos].id; }

const char *ItemHandler::GetSelectedItemType() {
  ItemID id = toolBar[selectedToolBarSlot].id;
  std::string str = itemDataBase.GetItemProperties(id).name;
  const char *txt = str.c_str();
  return txt;
}

// --- Set ---
void ItemHandler::SetItemSelection(int pos) { selectedToolBarSlot = pos; }
