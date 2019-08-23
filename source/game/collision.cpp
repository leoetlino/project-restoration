#include "game/collision.h"

#include <string>

#include "common/context.h"
#include "common/debug.h"
#include "common/utils.h"

namespace game {

struct GlobalContext;

void EmitDamageEffect(const CollisionInfo& info, DamageEffect effect) {
  rst::util::GetPointer<void(GlobalContext*, const CollisionInfo&, DamageEffect)>(0x51297C)(
      rst::GetContext().gctx, info, effect);
}

// Similar to EmitDamageEffect, but additionally emits a graphical hit effect that looks similar
// to a light arrow hit.
void EmitDamageEffectForBossCycle(const CollisionInfo& info) {
  rst::util::GetPointer<void(GlobalContext*, const CollisionInfo&)>(0x5149B0)(
      rst::GetContext().gctx, info);
}

void EmitDamageFlash(const act::Actor& actor, int a, int b, int c, int d) {
  rst::util::GetPointer<void(const act::Actor&, int, int, int, int)>(0x51E214)(actor, a, b, c, d);
}

void PrintCollision(const CollisionInfo* info, size_t count, std::string_view description) {
  std::string s{description};
  s += ' ';
  for (size_t i = 0; i < count; ++i) {
    s += rst::util::StringFromFormat("%zu=%02x ", i, info[i].flags1.flags);
  }
  rst::util::Print(s);
}

}  // namespace game
