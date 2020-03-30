#pragma once

#include <optional>

#include "common/span.h"
#include "game/items.h"
#include "game/pad.h"

namespace rst::link {

struct TransformAction {
  game::pad::Button trigger_btn;
  game::ItemId required_mask;
  game::Action action;
  bool usable_in_water;
  const char* name;
  bool require_zr = false;
};

tcb::span<const TransformAction> GetTransformActions();

void Init();

void Calc();

std::optional<game::Action> GetFastArrowAction();

}  // namespace rst::link
