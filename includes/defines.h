#ifndef DEFINES_H
#define DEFINES_H
#include <cstdint>

// namespace Config
namespace Config {
constexpr int SCREEN_WIDTH = 1400;
constexpr int SCREEN_HEIGHT = 800;
constexpr int TILE_SIZE = 32;
constexpr int TILE_SIZE_HALF = TILE_SIZE / 2;

constexpr float SCALE = 2.0;
constexpr float SCALED_TILE_SIZE = TILE_SIZE * SCALE;
constexpr float SCALED_TILE_SIZE_HALF = TILE_SIZE_HALF * SCALE;

} // namespace Config

// --- Unsigned Integer Aliases ---
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// --- Signed Integer Aliases ---
using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

#endif // !DEFINES_H
