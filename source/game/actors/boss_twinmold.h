#pragma once

#include "common/types.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/collision.h"

namespace game {
class GlobalContext;
}

namespace game::act {

struct BossTwinmold : Actor {
  // Probably incomplete.
  enum class Status : u16 {
    Buried = 0,
    BlueRisingOutOfSand = 1,
    // Also resets the hit counter.
    RedBurrowingIntoSand = 3,

    Flying = 4,
    Unk6 = 6,
    // The most commonly seen state for Blue Twinmold.
    FlyingAimlessly = 7,

    Unk8 = 8,
    Unk9 = 9,

    // These states are entered after receiving enough hits.
    BlueStunnedByShootingEyes = 11,
    BlueStunnedEyeOut = 12,
    BlueStunnedBurrowingIntoSand = 13,

    Stunned = 15,
    StunnedAndOnGround = 16,

    TauntingLink = 18,
    AfterTaunting = 19,

    // Only for Red Twinmold?
    TauntingAndAttacking = 21,
    AfterTauntingAndAttacking = 22,
    FlyingAndAttacking = 23,

    BeingGrabbedByLink = 24,
    BeingChokedByLink = 25,

    PreparingToRiseOutOfSand = 26,

    // ?
    AfterTaunting2 = 98,

    Inactive = 100,
    FirstTimeRisingOutOfSand = 102,

    DyingStart = 200,
    DyingExploding = 201,
    DyingFallingToGround = 202,
    DyingTouchedGround = 203,
  };

  void* resource;
  Status status;
  u16 some_status_change_countdown;
  u16 field_200;
  u16 field_202;
  u16 frame_counter;
  u16 field_206;
  u16 field_208;
  u16 field_20A;
  u16 field_20C;
  u8 gap_20E[14];
  u16 field_21C;
  u8 gap_21E[14];
  Vec3 field_22C;
  Vec3 field_238;
  u8 gap244[1];
  u8 field_245;
  u8 gap246[3];
  u8 gap_249[27];
  float field_264;
  float field_268;
  signed int field_26C;
  Vec3 field_270;
  u8 gap27C[15748];
  u32 field_4000;
  u8 gap_4004[3448];
  u32 field_4D7C;
  u32 field_4D80;
  u8 gap_4D84[56];
  u32 status_anim;
  u8 gap_4DC0[8];
  float field_4DC8;
  signed int field_4DCC;
  u8 gap_4DD0[560];
  u32 field_5000;
  u8 gap_5004[3448];
  Vec3 field_5D7C;
  Vec3 field_5D88;
  Vec3 field_5D94;
  Vec3 field_5DA0;
  u8 gap5DAC[120];
  void (*field_5E24)(BossTwinmold*, GlobalContext*);
  /// Points to Blue Twinmold for Red Twinmold, and vice versa.
  BossTwinmold* other_twinmold_actor;
  int field_5E2C;
  u8 gap_5E30[12];
  /// one for each segment of Twinmold's body,
  /// starting at the head (0) and ending at the tail (13)
  CollisionBodies<CollisionBodyCylinderCollection, 14> collision_1;
  CollisionBodies<CollisionBodyCylinderCollection, 14> collision_main;
  CollisionBodyCylinder collision_2;
  u8 gap_6794[8];
  u32 field_679C;
  u16 field_67A0;
  u8 gap_67A2[26];
  float field_67BC;
  u16 field_67C0;
  signed int field_67C4;
  u8 gap_67C8[44];
  float field_67F4;
  float field_67F8;
  u8 gap_67FC[8];
  int (*field_6804)(BossTwinmold*, GlobalContext*, act::Actor*);
  u8 gap_6808[6136];
  u32 field_8000;
  u8 gap_8004[784];
  char field_8314;
  char field_8315;
  char field_8316;
  char field_8317;
  u8 gap_8318[3304];
  u32 field_9000;
  u8 gap_9004[208];
  char field_90D4;
  __attribute__((aligned(4))) u8 field_90D8;
  u8 gap_90D9[11];
  u16 hit_counter;
  u16 field_90E6;
  u8 gap_90E8[12];
  u32 field_90F4;
  int field_90F8;
  u8 gap_90FC[16];
  u32 field_910C;
  u32 field_9110;
  u8 gap_9114[4];
  u32 field_9118;
  u8 gap_911C[8];
  u32 field_9124;
  u8 gap_9128[4];
  float eye_scale;
  float field_9130;
  u8 gap_9134[8];
  float field_913C;
  u8 gap_9140[24];
  Vec3 field_9158;
  Vec3 field_9164;
  u8 gap9170[36];
  CollisionBodies<CollisionBodyCylinderCollection, 1> collision_eye;
  CollisionBodies<CollisionBodyCylinderCollection, 1> collision_3;
  u32 field_9274;
  u8 gap_9278[308];
  u32 field_93AC;
  u8 gap_93B0[308];
  CollisionBodies<CollisionBodyCylinderCollection, 1> collision_4;
  CollisionBodies<CollisionBodyCylinderCollection, 1> collision_5;
  Vec3 field_95C4;
  u8 gap_95D0[8];
  int field_95D8;
  int field_95DC;
  int field_95E0;
  u8 gap_95E4[60];
  int field_9620;
  int field_9624;
};
static_assert(sizeof(BossTwinmold) == 0x9628);
static_assert(rst::util::OffsetOf(&BossTwinmold::other_twinmold_actor) == 0x5E28);
static_assert(rst::util::OffsetOf(&BossTwinmold::gap_93B0) == 0x93B0);

}  // namespace game::act
