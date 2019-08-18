#pragma once

#include <string_view>

#include "common/flags.h"
#include "common/types.h"

namespace game {

namespace act {
class Actor;
}

// FIXME: Incomplete.
struct CollisionBody {
  act::Actor* actor;
};

// https://wiki.cloudmodding.com/mm/Damage_Charts
enum class AttackType : u32 {
  DekuNut = 0,
  DekuStick = 1,
  Epona = 2,
  Bomb = 3,
  ZoraFins = 4,
  Arrow = 5,
  Mirror = 6,
  Hookshot = 7,
  GoronPunch = 8,
  Sword = 9,
  GoronPound = 10,
  FireArrow = 11,
  IceArrow = 12,
  LightArrow = 13,
  GoronSpikes = 14,
  DekuSpin = 15,
  DekuBubble = 16,
  DekuFlower = 17,
  DekuNutDrop = 18,
  ZoraBarrier = 19,
  Type20 = 20,
  Type21 = 21,
  Type22 = 22,
  Zora = 23,
  GreatFairySword = 24,
  Beam = 25,
  Roll = 26,
  Type27 = 27,
  Type28 = 28,
  Type29 = 29,
  Type30 = 30,
  Keg = 31,
};

struct CollisionInfo {
  AttackType GetType() const { return AttackType(__builtin_ctz(flags.GetStorage(0))); }
  bool IsType(AttackType t) const { return flags.IsSet(t); }

  rst::BitSet<32, u32, AttackType> flags;
};

struct Collision {
  enum class Flag0 : u8 {};
  enum class Flag1 : u8 {
    Collided = 2,
  };
  u8 gap_0[14];
  u16 field_E;
  u16 field_10;
  u16 field_12;
  u8 gap_14;
  rst::Flags<Flag0> flags0;
  rst::Flags<Flag1> flags1;
  u8 field_17;
  u8 gap_18[4];
  CollisionBody* body;
  u8 gap_20[4];
  CollisionInfo* info;
  u8 gap_28[0x20];
  float field_48;
  u8 gap_4C[4];
};
static_assert(sizeof(Collision) == 0x50);

enum class CollisionResponse : int {
  Damage = 0,
  NoDamage = 1,
  NoDamageYellow = 2,
};
void HandleCollision(const Collision& col, CollisionResponse response);
void HandleCollisionForBossCycleLastDamage(const Collision& col);

void PrintCollision(const Collision* col, size_t count, std::string_view description = "");

inline bool IsCollided(const Collision& collision) {
  return collision.flags1.IsSet(Collision::Flag1::Collided);
}

}  // namespace game
