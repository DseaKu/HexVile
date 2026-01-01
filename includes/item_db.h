#ifndef ITEM_DB_H
#define ITEM_DB_H

#include "enums.h"
#include <string>

// --- Item Database ---
namespace item_db {
;
struct ItemProperties {
  std::string name;
  int maxStack;
  int value;
  bool placeableTile;
};

static const std::map<item::id, ItemProperties> DB = {
    {item::NULL_ID,
     {.name = "Null", .maxStack = 0, .value = 0, .placeableTile = false}},
    {item::SET_GRASS,
     {.name = "Set Grass", .maxStack = 32, .value = 5, .placeableTile = true}},
    {item::SET_WATER,
     {.name = "Set Water", .maxStack = 32, .value = 8, .placeableTile = true}},
    {item::SET_DIRT,
     {.name = "Set Dirt", .maxStack = 32, .value = 8, .placeableTile = true}},
    {item::AXE,
     {.name = "Axe", .maxStack = 1, .value = 10, .placeableTile = false}},
    {item::WOOD,
     {.name = "Wood", .maxStack = 64, .value = 2, .placeableTile = false}},
};
} // namespace item_db
#endif // !ITEM_DB_H
