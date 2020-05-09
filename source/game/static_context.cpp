#include "game/static_context.h"

#include "common/utils.h"

namespace game {

StaticContext& GetStaticContext() {
  return *rst::util::GetPointer<StaticContext>(0x7892D0);
}

}  // namespace game
