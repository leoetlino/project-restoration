#include "game/pad.h"

#include "common/utils.h"

namespace game::pad {

ControllerMgr& GetControllerMgr() {
  return *rst::util::GetPointer<ControllerMgr>(0x729000);
}

}  // namespace game::pad
