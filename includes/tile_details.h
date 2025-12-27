#ifndef TILE_DETAILS_H
#define TILE_DETAILS_H

#include "enums.h"
#include <array>

namespace tile_det {

// ==========================================
//               Resource
// ==========================================
constexpr int RESCOURCE_DIVERSITY = 3;
constexpr int SPAWN_CHANCE_TREE = 5;
constexpr int SPAWN_CHANCE_ROCK = 5;

} // namespace tile_det

// ==========================================
//               Details
// ==========================================
namespace spawn_data_det {
// --- Details ---
constexpr int DETAIL_DIVERSITY = 6;
constexpr int TOTAL_WEIGHT_DET = 100;
constexpr std::array<int, DETAIL_DIVERSITY> DET_SPAWN_CHANCE_GRASS = {5, 4, 3,
                                                                      0, 0, 0};
constexpr std::array<int, DETAIL_DIVERSITY> DET_SPAWN_CHANCE_WATER = {3, 0, 0,
                                                                      0, 0, 0};
constexpr std::array<int, DETAIL_DIVERSITY> DET_SPAWN_CHANCE_DIRT = {3, 1, 0,
                                                                     0, 0, 0};

inline const std::map<tile::id, std::array<int, DETAIL_DIVERSITY>> detLut = {
    {tile::GRASS, DET_SPAWN_CHANCE_GRASS},
    {tile::WATER, DET_SPAWN_CHANCE_WATER},
    {tile::DIRT, DET_SPAWN_CHANCE_DIRT}};

} // namespace spawn_data_det
#endif // !TILE_DETAILS_H
