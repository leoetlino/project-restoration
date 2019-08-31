#pragma once

#include <optional>

#include "game/items.h"

namespace rst::link {

void Init();

void Calc();

std::optional<game::Action> GetFastArrowAction();

}  // namespace rst::link
