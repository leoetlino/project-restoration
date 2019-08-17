#include "game/random.h"

#include "common/utils.h"

namespace game {

u32 RandomInt() {
  return rst::util::GetPointer<u32()>(0x14F55C)();
}

float RandomFloat() {
  return rst::util::GetPointer<float()>(0x20A8F8)();
}

}  // namespace game
