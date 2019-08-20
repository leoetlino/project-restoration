#include "game/collision.h"

#include <string>

#include "common/context.h"
#include "common/debug.h"
#include "common/utils.h"

namespace game {

struct GlobalContext;

void EmitDamageEffect(const Collision& col, CollisionResponse response) {
  rst::util::GetPointer<void(GlobalContext*, const Collision&, CollisionResponse)>(0x51297C)(
      rst::GetContext().gctx, col, response);
}

// Similar to EmitDamageEffect, but additionally emits a graphical hit effect that looks similar
// to a light arrow hit.
void EmitDamageEffectForBossCycle(const Collision& col) {
  rst::util::GetPointer<void(GlobalContext*, const Collision&)>(0x5149B0)(rst::GetContext().gctx,
                                                                          col);
}

void EmitDamageFlash(const act::Actor& actor, int a, int b, int c, int d) {
  rst::util::GetPointer<void(const act::Actor&, int, int, int, int)>(0x51E214)(actor, a, b, c, d);
}

void PrintCollision(const Collision* col, size_t count, std::string_view description) {
  std::string s{description};
  s += ' ';
  for (size_t i = 0; i < count; ++i) {
    s += rst::util::StringFromFormat("%zu=%02x ", i, col[i].flags1.flags);
  }
  rst::util::Print(s);
}

}  // namespace game
