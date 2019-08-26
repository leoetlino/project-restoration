#pragma once

#include <string_view>

#include "common/flags.h"
#include "common/types.h"

namespace game {

namespace act {
class Actor;
}

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

// https://wiki.cloudmodding.com/oot/Collision (some structures have been changed, though)

struct CollisionBody;

struct CollisionInfo {
  enum class Flag0 : u8 {};
  enum class Flag1 : u8 {
    Collided = 2,
  };

  AttackType GetType() const { return AttackType(__builtin_ctz(attack_type.GetStorage(0))); }
  bool IsType(AttackType t) const { return attack_type.IsSet(t); }
  bool IsCollided() const { return flags1.IsSet(Flag1::Collided); }
  void ClearCollided() { flags1.Clear(Flag1::Collided); }

  rst::BitSet<32, u32, AttackType> attack_type;
  u8 gap_4[10];
  u16 field_E;
  u16 field_10;
  u16 field_12;
  u8 gap_14;
  rst::Flags<Flag0> flags0;
  rst::Flags<Flag1> flags1;
  u8 flags2;
  u32 field_18;
  CollisionBody* colliding_body;
  u32 field_20;
  CollisionInfo* colliding_info;
};
static_assert(sizeof(CollisionInfo) == 0x28);

struct CollisionInfoCylinder : CollisionInfo {
  u32 field_28;
  u32 field_2C;
  u32 field_30;
  u32 field_34;
  Vec3 field_38;
  u32 field_44;
  float scale;
  u8 field_4C;
};
static_assert(sizeof(CollisionInfoCylinder) == 0x50);

struct CollisionInfoTri : CollisionInfo {
  Vec3 a, b, c, normal;
  float normal_dist;
};
static_assert(sizeof(CollisionInfoTri) == 0x5C);

// Base class for CollisionBody* structs.
// Not to be confused with CollisionInfo structs.
struct CollisionBody {
  enum class Type : u8 {
    CylinderCollection = 0,
    Cylinder = 1,
    TriCollection = 2,
    Quad = 3,
    // New to MM3D?
    Type4 = 4,
  };

  act::Actor* actor;
  act::Actor* unk_actor_1;
  act::Actor* unk_actor_2;
  act::Actor* unk_actor_3;
  u8 collider_flags;
  u8 collide_flags;
  u8 mask_a;
  u8 mask_b;
  u8 field_14;
  Type type;
};
static_assert(sizeof(CollisionBody) == 0x18);

struct CollisionBodyCylinderCollection : CollisionBody {  // type 0
  using InfoType = CollisionInfoCylinder;
  int count;
  // Confusingly enough, this is quite different from CollisionBodyCylinder.
  CollisionInfoCylinder* list;
};
static_assert(sizeof(CollisionBodyCylinderCollection) == 0x20);

struct CollisionBodyCylinder : CollisionBody {  // type 1
  CollisionInfo info;
  // Note: these are now floats in the 3DS version.
  float radius;
  float height;
  float y_shift;
  Vec3 position;
};
static_assert(sizeof(CollisionBodyCylinder) == 0x58);

struct CollisionBodyTriCollection : CollisionBody {  // type 2
  using InfoType = CollisionInfoTri;
  int count;
  CollisionInfoTri* list;
};
static_assert(sizeof(CollisionBodyTriCollection) == 0x20);

struct CollisionBodyQuad : CollisionBody {
  CollisionInfo info;
  Vec3 a, b, c, d;
  // Note: these are still integers in the 3DS version.
  TVec3<s16> bounding_max;
  TVec3<s16> bounding_min;
};
static_assert(sizeof(CollisionBodyQuad) == 0x7C);

struct CollisionBodyType4 : CollisionBody {
  CollisionInfo info;
  u32 field_40;
  u32 field_44;
  u32 field_48;
  u32 field_4C;
  u32 field_50;
  u32 field_54;
  u32 field_58;
  u32 field_5C;
  float field_60;
  u8 field_64;
};
static_assert(sizeof(CollisionBodyType4) == 0x68);

// Helper class to make a common pattern in MM3D (storing body + info) less error prone.
template <typename Body, int N>
struct CollisionBodies {
  Body body;
  typename Body::InfoType info[N];

  auto& operator[](size_t idx) { return info[idx]; }
  const auto& operator[](size_t idx) const { return info[idx]; }
  auto* begin() { return info; }
  auto* begin() const { return info; }
  auto* end() { return info + N; }
  auto* end() const { return info + N; }
};

enum class DamageEffect : int {
  Damage = 0,
  NoDamage = 1,
  NoDamageYellow = 2,
};
void EmitDamageEffect(const CollisionInfo& info, DamageEffect effect);
void EmitDamageEffectForBossCycle(const CollisionInfo& info);
void EmitDamageFlash(const act::Actor& actor, int a, int b, int c, int d);

void PrintCollision(const CollisionInfo* info, size_t count, std::string_view description = "");

inline bool IsCollided(const CollisionInfo& info) {
  return info.flags1.IsSet(CollisionInfo::Flag1::Collided);
}

}  // namespace game
