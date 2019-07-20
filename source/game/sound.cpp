#include "game/sound.h"

#include "common/utils.h"

namespace game::sound {

bool PlayEffect(u32 id) {
  return rst::util::GetPointer<bool(u32)>(0x2006E4)(id);
}

}
