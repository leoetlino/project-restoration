#include "common/context.h"

namespace rst {

Context& GetContext() {
  static Context s_context{};
  return s_context;
}

}  // namespace rst
