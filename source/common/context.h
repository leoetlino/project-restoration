#pragma once

#include "common/types.h"

namespace game {
struct GlobalContext;
}

namespace rst {

struct Context {
  game::GlobalContext* gctx;
  bool has_initialised = false;

  bool use_fast_swim = true;
};

Context& GetContext();

}  // namespace rst
