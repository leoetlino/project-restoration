#pragma once

#include "common/types.h"

namespace game {

struct Collision {
  u8 gap_0[14];
  u16 field_E;
  u16 field_10;
  u16 field_12;
  u8 gap_14;
  u8 flags0;
  u8 flags1;
  u8 field_17;
  u8 gap_18[12];
  u32 field_24;
  u8 gap_28[40];
};

}  // namespace game
