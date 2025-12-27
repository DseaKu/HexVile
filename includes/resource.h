#ifndef RESCOURCE_H
#define RESCOURCE_H

#include "enums.h"
#include "raylib.h"
#include "texture_atlas.h"
namespace rsrc {

constexpr Vector2 POS_NULL = {0, 0};

enum ID : int {
  UNINITIALIZED = -2,
  ID_NULL = -1,
  ID_TREE = 0,
  ID_ROCK,
};
struct Object {
  Vector2 tilePos;
  TA_Pos taPos;
  int hp;
  ID id;
  int spawn_chance;
};

constexpr Object RSRC_NULL = {POS_NULL, tex_atlas::POS_NULL, 0, ID_NULL, 0};

// --- Grass ---
constexpr Object TREE = {POS_NULL, tex_atlas::RSRC_TREE, 100, ID_TREE, 5};

// --- Water ---
// --- Dirt ---
constexpr Object ROCK = {POS_NULL, tex_atlas::RSRC_STONE, 100, ID_ROCK, 5};

inline const std::map<tile::id, Object> TILE_LUT = {
    {tile::GRASS, TREE},
    {tile::WATER, RSRC_NULL},
    {tile::DIRT, ROCK},
};
} // namespace rsrc

#endif // !RESCOURCE_H
