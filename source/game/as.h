#pragma once

// Animation sequence system.

#include "common/types.h"
#include "common/utils.h"

namespace game::as {

struct State {
  u32 id;
  float field_4;
  float field_8;
  float position;
  float speed;
  float start_position;
  float end_position;
  float duration_plus_one;
  u8 field_20[6];
  u8 field_26;
  u8 field_27;
};
static_assert(sizeof(State) == 0x28);

// Incomplete, and it's unclear what this is used for, so the name is temporary, too.
struct ActorUtil {
  void PlayAnim(u32 id, float x);
  void PlayAnimFull(u32 id, float x);
  int GetAnimDuration(u32 id);

  void* vtable;
  int field_4;
  int field_8;
  void* vtable_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  int field_24;
  int field_28;
  int field_2C;
  int field_30;
  int field_34;
  int field_38;
  State state;
  u8 gap_64[24];
  u8 field_7C;
  u8 field_7D;
  u8 field_7E;
  u8 field_7F;
  u8 gap_80[8];
  u8 field_88;
  u8 field_89;
  u8 field_8A;
  u8 field_8B;
};
static_assert(rst::util::OffsetOf(&ActorUtil::gap_64) == 0x64);
static_assert(sizeof(ActorUtil) == 0x8C);

}  // namespace game::as
