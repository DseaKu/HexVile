#include "item_handler.h"
#include "defines.h"
#include "enums.h"

// --- Item Data Base ---
ItemDataBase::ItemDataBase() {
  properties.resize(item::SIZE);
  properties[item::NULL_ID] = {
      .name = "Null", .maxStack = 0, .value = 0, .placeableTile = false};
  properties[item::SET_GRASS] = {
      .name = "Set Grass", .maxStack = 32, .value = 5, .placeableTile = true};
  properties[item::SET_WATER] = {
      .name = "Set Water", .maxStack = 32, .value = 8, .placeableTile = true};
  properties[item::SET_DIRT] = {
      .name = "Set Dirt", .maxStack = 32, .value = 8, .placeableTile = true};
}

const ItemProperties &ItemDataBase::GetItemProperties(item::id itemid) const {
  return properties[itemid];
}

// --- Item Handler ---
ItemHandler::ItemHandler() {
  selectedToolBarSlot = 0;
  Init();
}

void ItemHandler::Init() {
  ItemStack itemNull = {.itemID = item::NULL_ID, .count = 0};
  inventory.assign(Conf::INVENTORY_SLOTS, itemNull);
  toolBar.assign(Conf::TOOLBAR_SLOTS, itemNull);

  ItemStack grass = {.itemID = item::SET_GRASS, .count = 32};
  ItemStack water = {.itemID = item::SET_WATER, .count = 51};
  ItemStack dirt = {.itemID = item::SET_DIRT, .count = 8};
  ItemStack dirt2 = {.itemID = item::SET_DIRT, .count = 24};
  toolBar[0] = dirt;
  toolBar[1] = water;
  toolBar[2] = dirt2;
  toolBar[3] = grass;
  toolBar[5] = grass;
}

bool ItemHandler::TakeItemFromToolBar(ItemStack *itemStack, int amount) {
  if (itemStack->count >= amount) {
    itemStack->count -= amount;
    if (itemStack->count == 0) {
      itemStack->itemID = item::NULL_ID;
    }
    return true;
  }
  return false;
}

// --- Conversion ---
item::id ItemHandler::ToolBarSelectionToItemID(int sel) const {
  return toolBar[sel].itemID;
};

tile::id ItemHandler::ConvertItemToTileID(item::id itemid) const {
  auto it = item_to_tile_map.find(itemid);
  if (it != item_to_tile_map.end()) {
    return it->second;
  }
  return tile::NULL_ID;
}

// --- Get ---
int ItemHandler::GetSelectionToolBar() const { return selectedToolBarSlot; }

ItemStack *ItemHandler::GetToolBarItemPointer(int pos) { return &toolBar[pos]; }

item::id ItemHandler::GetToolBarItemType(int pos) const {
  return toolBar[pos].itemID;
}

const char *ItemHandler::GetSelectedItemType() const {
  item::id id = toolBar[selectedToolBarSlot].itemID;
  return itemDataBase.GetItemProperties(id).name.c_str();
}

// --- Set ---
void ItemHandler::SetItemSelection(int pos) { selectedToolBarSlot = pos; }
