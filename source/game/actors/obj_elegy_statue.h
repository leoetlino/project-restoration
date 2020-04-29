#pragma once

#include "game/actor.h"
#include "game/collision.h"

namespace game::act {

struct ObjElegyStatue : Actor {
  enum class State : u8 {
    PrepareSpawn = 0,
    WaitForLinkToGetAway = 1,
    Spawn = 2,
    PrepareDespawn = 3,
    Despawn = 4,
  };

  CollisionBodyCylinder collision;
  State state;
  /// Ticks before moving from state 0 to state 1. Counts down to 0.
  u8 timer;
  /// 0 (min) - 0xFF (max)
  u16 opacity;
  void* field_254;
};
static_assert(sizeof(ObjElegyStatue) == 0x258);

}  // namespace game::act
