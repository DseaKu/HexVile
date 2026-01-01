#include "item_handler.h"
#include "defines.h"
#include "enums.h"
#include "item_db.h"

// --- Constructors ---
ItemHandler::ItemHandler() { Init(); }

// --- Logic / Actions ---
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

bool ItemHandler::AddItem(item::id itemID, int count) {
  // Try to stack in toolbar first
  for (auto &stack : toolBar) {
    if (stack.itemID == itemID) {
      if (stack.count + count <= item_db::DB.at(itemID).maxStack) {
        stack.count += count;
        return true;
      }
    }
  }

  // Try to find empty slot in toolbar
  for (auto &stack : toolBar) {
    if (stack.itemID == item::NULL_ID) {
      stack.itemID = itemID;
      stack.count = count;
      return true;
    }
  }

  // Try to stack in inventory (not fully implemented in UI but data exists)
  for (auto &stack : inventory) {
    if (stack.itemID == itemID) {
      if (stack.count + count <= item_db::DB.at(itemID).maxStack) {
        stack.count += count;
        return true;
      }
    }
  }

  // Try to find empty slot in inventory
  for (auto &stack : inventory) {
    if (stack.itemID == item::NULL_ID) {
      stack.itemID = itemID;
      stack.count = count;
      return true;
    }
  }
  return false;
}

// --- Setters ---
void ItemHandler::SetFrameContext(const frame::Context *curFrameContext) {
  this->frameContext = curFrameContext;
}

// --- Getters ---
ItemStack *ItemHandler::GetInventoryItemPointer(int pos) {
  if (pos >= 0 && pos < inventory.size()) {
    return &inventory[pos];
  }
  return nullptr;
}

ItemStack *ItemHandler::GetToolBarItemPointer(int pos) { return &toolBar[pos]; }

item::id ItemHandler::GetToolBarItemType(int pos) const {
  return toolBar[pos].itemID;
}

const char *ItemHandler::GetSelectedItemType() const {
  int slot = this->frameContext->selToolBarSlot;
  if (slot < 0 || slot >= toolBar.size()) {
    slot = 0;
  }
  item::id id = toolBar[slot].itemID;
  return item_db::DB.at(id).name.c_str();
}

ItemContainer *ItemHandler::GetInventoryPointer() { return &this->inventory; };

ItemContainer *ItemHandler::GetToolBarPointer() { return &this->toolBar; };

// --- Conversions / Helpers ---
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

// --- Private Methods ---
void ItemHandler::Init() {
  ItemStack itemNull = {.itemID = item::NULL_ID, .count = 0};
  inventory.assign(conf::ITEM_SLOT_PER_ROW * conf::INVENTORY_ROW, itemNull);
  toolBar.assign(conf::ITEM_SLOT_PER_ROW, itemNull);

  ItemStack grass = {.itemID = item::SET_GRASS, .count = 32};
  ItemStack water = {.itemID = item::SET_WATER, .count = 51};
  ItemStack dirt = {.itemID = item::SET_DIRT, .count = 8};
  ItemStack dirt2 = {.itemID = item::SET_DIRT, .count = 24};
  ItemStack axe = {.itemID = item::AXE, .count = 1};

  toolBar[0] = axe;
  toolBar[1] = water;
  toolBar[2] = dirt2;
  toolBar[3] = grass;
  toolBar[4] = dirt;
  toolBar[5] = grass;

  inventory[0] = grass;
  inventory[2] = water;
  inventory[6] = water;
  inventory[12] = grass;
  inventory[17] = grass;
  inventory[15] = dirt;
}
