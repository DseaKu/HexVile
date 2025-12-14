#include "item.h"
#include "defines.h"
#include "enums.h"

ItemHandler::ItemHandler() {}

void ItemHandler::Init() {
  Item itemNull;
  inventory.assign(Conf::ITEM_STACK_MAX_INVENTORY, itemNull);
  toolBar.assign(Conf::ITEM_STACK_MAX_TOOL_BAR, itemNull);

  Item grass = {.id = ITEM_SET_TILE_GRASS, 3};
}
