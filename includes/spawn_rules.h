#ifndef SPAWN_RULES_H
#define SPAWN_RULES_H

#include "resource.h"

namespace spawn_data_rsrc {

// ==========================================
//               Resource
// ==========================================
constexpr int RESCOURCE_DIVERSITY = 3;
constexpr int SPAWN_CHANCE_TREE = 5;
constexpr int SPAWN_CHANCE_ROCK = 5;

constexpr std::pair<rsrc::id, int> RSRC_NULL = {rsrc::NULL_ID, 0};

constexpr std::array<std::pair<rsrc::id, int>, RESCOURCE_DIVERSITY>
    RSRC_SPAWN_CHANCE_GRASS = {{{rsrc::TREE, 5}, RSRC_NULL, RSRC_NULL}};

constexpr std::array<std::pair<rsrc::id, int>, RESCOURCE_DIVERSITY>
    RSRC_SPAWN_CHANCE_WATER = {RSRC_NULL, RSRC_NULL, RSRC_NULL};
constexpr std::array<std::pair<rsrc::id, int>, RESCOURCE_DIVERSITY>
    RSRC_SPAWN_CHANCE_DIRT = {RSRC_NULL, RSRC_NULL, RSRC_NULL};

inline const std::map<tile::id,
                      std::array<std::pair<rsrc::id, int>, RESCOURCE_DIVERSITY>>
    rsrcLut = {{tile::GRASS, RSRC_SPAWN_CHANCE_GRASS},
               {tile::WATER, RSRC_SPAWN_CHANCE_WATER},
               {tile::DIRT, RSRC_SPAWN_CHANCE_DIRT}};

} // namespace spawn_data_rsrc

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
#endif // !SPAWN_RULES_H
