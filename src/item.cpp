#include "item.h"
#include "defines.h"
#include "enums.h"

// --- Item Data Base ---
ItemDataBase::ItemDataBase() { propeties.resize(ITEMS_ID_SIZE); }

// --- Item Handler ---
ItemHandler::ItemHandler() {}

void ItemHandler::Init() {
  Item itemNull = {.id = ITEM_NULL, .count = 0};
  inventory.assign(Conf::ITEM_STACK_MAX_INVENTORY, itemNull);
  toolBar.assign(Conf::ITEM_STACK_MAX_TOOL_BAR, itemNull);

  Item grass = {.id = ITEM_SET_TILE_GRASS, .count = 3};
  Item water = {.id = ITEM_SET_TILE_GRASS, .count = 3};
  toolBar[0] = grass;
  toolBar[1] = water;
  toolBar[5] = grass;
  toolBar[3] = grass;
}
