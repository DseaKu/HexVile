#ifndef ANIMATION_H
#define ANIMATION_H

#include "enums.h"
#include "texture_atlas.h"

namespace animation {
struct Object {

  tex_atlas::Coords texAtlas;
  int frameCount;
  float speed;
  bool isLooping;
};

constexpr Object Player_IDLE = {tex_atlas::PLAYER_IDLE, 12, 5.0f, true};
constexpr Object Player_WALK = {tex_atlas::PLAYER_WALK, 8, 9.0f, true};
constexpr Object Player_CHOP = {tex_atlas::PLAYER_ATTACK, 7, 4.0f, true};

inline const std::map<playerState::id, Object> playerLut = {
    {playerState::IDLE, Player_IDLE},
    {playerState::WALK, Player_WALK},
    {playerState::CHOP, Player_CHOP}};
}; // namespace animation
#endif // !ANIMATION_H
