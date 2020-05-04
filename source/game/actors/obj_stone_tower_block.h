#pragma once

#include "game/actor.h"

namespace game::act {

struct ObjStoneTowerBlock : DynaPolyActor {
  enum class MoveDirection : int {
    X = 0,
    Y = 1,
    Z = 2,
    NegX = 3,
    NegY = 4,
    NegZ = 5,
    None = 6,
  };

  void* field_20C;
  int field_210;
  int field_214;
  MoveDirection move_direction;
  void* object_handle;
  ActionFunc<ObjStoneTowerBlock>* action_fn;
  u16 field_224;
  u16 timer;
};
static_assert(sizeof(ObjStoneTowerBlock) == 0x228);

}  // namespace game::act
