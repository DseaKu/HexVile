#ifndef RESCOURCE_H
#define RESCOURCE_H

#include "enums.h"
#include "texture_atlas.h"

namespace rsrc {

enum id : int {
  NULL_ID = -1,
  TREE_ID = 0,
  SIZE,
};
struct Props {
  TA_Pos taPos;
  int hp;
  rsrc::id id;
  int spawn_chance;
};

constexpr Props RSRC_NULL = {tex_atlas::POS_NULL, 0, NULL_ID, 0};

// --- Grass ---
constexpr Props TREE = {tex_atlas::RSRC_TREE, 100, TREE_ID, 5};

// --- Water ---
// --- Dirt ---
constexpr Props ROCK = {tex_atlas::RSRC_STONE, 100, TREE_ID, 5};

inline const std::map<tile::id, Props> TILE_LUT = {
    {tile::GRASS, TREE},
    {tile::WATER, RSRC_NULL},
    {tile::DIRT, ROCK},
};
} // namespace rsrc

#endif // !RESCOURCE_H
