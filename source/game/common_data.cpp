#include "game/common_data.h"

#include "common/utils.h"

namespace game {

CommonData& GetCommonData() {
  // Right before the static context in .bss.
  return *rst::util::GetPointer<CommonData>(0x7751D8);
}

}  // namespace game
