#pragma once

#include <string_view>

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
  u8 gap_0[14];
  u16 field_E;
  u16 field_10;
  u16 field_12;
  u8 gap_14;
  u8 flags0;
  u8 flags1;
  u8 field_17;
  u8 gap_18[4];
  CollisionBody* body;
  u8 gap_20[4];
  u32 field_24;
  u8 gap_28[0x20];
  float field_48;
  u8 gap_4C[4];
};
static_assert(sizeof(Collision) == 0x50);

void PrintCollision(const Collision* col, size_t count, std::string_view description = "");

}  // namespace game
