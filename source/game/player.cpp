#include "game/player.h"

#include "common/utils.h"

namespace game::act {

FormParam& GetFormParam(FormParamIndex idx) {
  return rst::util::GetPointer<FormParam>(0x7AE9E8, 0x7AF9E8)[u8(idx) % 8];
}

}  // namespace game::act
