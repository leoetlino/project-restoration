#include "game/as.h"

#include "common/utils.h"

namespace game::as {

void ActorUtil::PlayAnim(u32 id, float x) {
  rst::util::GetPointer<void(ActorUtil*, u32, float)>(0x51689C)(this, id, x);
}

void ActorUtil::PlayAnimFull(u32 id, float x) {
  rst::util::GetPointer<void(ActorUtil*, u32, float)>(0x5153C8)(this, id, x);
}

int ActorUtil::GetAnimDuration(u32 id) {
  return rst::util::GetPointer<int(ActorUtil*, u32)>(0x240DD8)(this, id);
}

}
