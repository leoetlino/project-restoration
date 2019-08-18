#include "game/collision.h"

#include <string>

#include "common/debug.h"

namespace game {

void PrintCollision(const Collision* col, size_t count, std::string_view description) {
  std::string s{description};
  s += ' ';
  for (size_t i = 0; i < count; ++i) {
    s += rst::util::StringFromFormat("%zu=%02x ", i, col[i].flags1.flags);
  }
  rst::util::Print(s);
}

}  // namespace game
