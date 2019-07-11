#pragma once

#include "common/types.h"
#include "game/actor.h"

namespace game::act {

struct NpcBombers : Actor {
  enum class State : u16 {
    StoppedLookingAround = 0,
    Walking = 1,
    ChasingLink = 2,
    StoppedExcited = 3,
    Frozen = 4,
  };

  int field_1F8;
  u8 gap_1FC[133];
  u8 field_281;
  u8 gap_282[2];
  u32 field_284;
  u8 gap_288[880];
  u32 field_5F8;
  u8 gap_5FC[880];
  float field_96C;
  int field_970;
  void* notebook_data;
  int field_978;
  u8 gap_97C[388];
  u32 field_B00;
  u8 gap_B04[100];
  void (*bombers_fn)(NpcBombers* self, GlobalContext* gctx);
  u8 gap_B6C[6];
  u16 field_B72;
  u16 field_B74;
  u8 gap_B76[6];
  u16 field_B7C;
  u8 gap_B7E[14];
  State bombers_state;
  u8 gap_B8E[6];
  u16 field_B94;
  u16 field_B96;
  u8 gap_B98[4];
  float field_B9C;
  u32 field_BA0;
  u8 gap_BA4[4];
  u16 field_BA8;
  /// Bombers number. Extracted from Actor::params & 0xf
  u16 number;
  u16 field_BAC;
  u8 gap_BAE[2];
  u32 field_BB0;
  u32 field_BB4;
  u8 gap_BB8[60];
  float field_BF4;
  float field_BF8;
  int field_BFC;
  int field_C00;
  int field_C04;
  int field_C08;
};
static_assert(sizeof(NpcBombers) == 0xc0c);

}  // namespace game::act
