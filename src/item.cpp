#include "item.h"
#include "defines.h"
#include "enums.h"

// --- Item Data Base ---
ItemDataBase::ItemDataBase() {
  propeties.resize(ITEM_ID_SIZE);
  propeties[ITEM_NULL] = {
      .name = "Null", .maxStack = 0, .value = 0, .TA_X = 0, .TA_Y = 0};
  propeties[ITEM_SET_GRASS] = {.name = "Set Grass",
                               .maxStack = 32,
                               .value = 5,
                               .TA_X = Conf::TA_ITEM_X_OFFSET,
                               .TA_Y = 0};
  propeties[ITEM_SET_WATER] = {
      .name = "Set Water", .maxStack = 32, .value = 8, .TA_X = 0, .TA_Y = 0};
}

ItemProperties ItemDataBase::GetItemProperties(ItemID id) {
  return propeties[id];
}

// --- Item Handler ---
ItemHandler::ItemHandler() {}

void ItemHandler::Init() {
  Item itemNull = {.id = ITEM_NULL, .count = 0};
  inventory.assign(Conf::ITEM_STACK_MAX_INVENTORY, itemNull);
  toolBar.assign(Conf::ITEM_STACK_MAX_TOOL_BAR, itemNull);

  Item grass = {.id = ITEM_SET_GRASS, .count = 3};
  Item water = {.id = ITEM_SET_GRASS, .count = 3};
  toolBar[0] = grass;
  toolBar[1] = water;
  toolBar[5] = grass;
  ItemProperties a = this->itemDataBase.GetItemProperties(ITEM_SET_WATER);
  ItemProperties b = this->itemDataBase.GetItemProperties(ITEM_NULL);
  toolBar[3] = grass;
}
