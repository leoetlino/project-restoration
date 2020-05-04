#include "game/actors/obj_stone_tower_block.h"
#include "common/types.h"

namespace rst {

using namespace game;

RST_HOOK void ObjStoneTowerBlock_MoveAfterDelay2(act::ObjStoneTowerBlock* self, GlobalContext*) {
  --self->timer;
  if (self->timer == 0) {
    auto calc_move = util::GetPointer<act::ActionFunc<act::ObjStoneTowerBlock>>(0x2AC52C);
    self->action_fn = calc_move;
  }
}

RST_HOOK void ObjStoneTowerBlock_MoveAfterDelay1(act::ObjStoneTowerBlock* self, GlobalContext*) {
  self->timer = 6;
  self->action_fn = ObjStoneTowerBlock_MoveAfterDelay2;
}

}  // namespace rst
