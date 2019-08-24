#include "game/actor.h"

#include "common/utils.h"

namespace game::act {

void Actor::Free() {
  draw_fn = nullptr;
  flags.Clear(Flag::Targetable);
  calc_fn = nullptr;
}

}  // namespace game::act
