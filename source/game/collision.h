#pragma once

#include <string_view>

#include "common/flags.h"
#include "common/types.h"

namespace game {

namespace act {
class Actor;
}

// FIXME: Incomplete.
struct CollisionBody {
  act::Actor* actor;
};

struct Collision {
  enum class Flag0 : u8 {};
  enum class Flag1 : u8 {
    Collided = 2,
  };
  u8 gap_0[14];
  u16 field_E;
  u16 field_10;
  u16 field_12;
  u8 gap_14;
  rst::Flags<Flag0> flags0;
  rst::Flags<Flag1> flags1;
  u8 field_17;
  u8 gap_18[4];
  CollisionBody* body;
  u8 gap_20[4];
  void* field_24;
  u8 gap_28[0x20];
  float field_48;
  u8 gap_4C[4];
};
static_assert(sizeof(Collision) == 0x50);

void PrintCollision(const Collision* col, size_t count, std::string_view description = "");

inline bool IsCollided(const Collision& collision) {
  return collision.flags1.IsSet(Collision::Flag1::Collided);
}

}  // namespace game
