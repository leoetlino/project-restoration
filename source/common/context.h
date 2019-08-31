#pragma once

#include "common/types.h"

namespace game {
struct GlobalContext;
}

namespace rst {

struct Context {
  game::GlobalContext* gctx;
  bool has_initialised = false;

  bool going_back_in_time = false;

  bool use_fast_swim = true;
  u32 a_press_duration = 0;
};

Context& GetContext();

}  // namespace rst
