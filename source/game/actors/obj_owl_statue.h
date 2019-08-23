#pragma once

#include "common/types.h"
#include "game/actor.h"
#include "game/collision.h"

namespace game {
struct GlobalContext;
}

namespace game::act {

struct ObjOwlStatue : Actor {
  enum class SaveStatus : u8 {
    NotInUse = 0,
    Saving = 1,
    Saved = 2,
  };

  u8 gap_1F8[8];
  u32 field_200;
  u8 gap_204[8];
  CollisionBodyCylinder col_body;
  bool talking;
  u8 field_265;
  u8 field_266;
  SaveStatus save_status;
  bool (*owl_calc)(ObjOwlStatue*, GlobalContext*);
  u32 field_26C;
  u32 field_270;
  int field_274;
  u16 field_278;
  u16 field_27A;
  u16 field_27C;
  u16 field_27E;
  int field_280;
  int field_284;
  int field_288;
  bool activating;
  // Extension: Ticks until talking to the actor is triggered to activate the warp point.
  s8 activation_timer;
  u16 field_28E;
};
static_assert(sizeof(ObjOwlStatue) == 0x290);

}  // namespace game::act
