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
  Vector2 worldPos;
  tex_atlas::Coords xyTexAtlas;
  int hp;
  ID id;
  int spawn_chance;
  float flashTimer;
};

constexpr Object OBJECT_NULL = {POS_NULL, tex_atlas::POS_NULL, 0, ID_NULL, 0,
                                0.0f};

// --- Grass ---
constexpr Object OBJECT_TREE = {POS_NULL, tex_atlas::RSRC_TREE, 100, ID_TREE,
                                1, 0.0f};

// --- Water ---
// --- Dirt ---
constexpr Object OBJECT_ROCK = {POS_NULL, tex_atlas::RSRC_STONE, 100, ID_ROCK,
                                5, 0.0f};

inline const std::map<tile::id, Object> TILE_LUT = {
    {tile::GRASS, OBJECT_TREE},
    {tile::WATER, OBJECT_NULL},
    {tile::DIRT, OBJECT_ROCK},
};
} // namespace rsrc

#endif // !RESCOURCE_H
