#pragma once

#include "common/types.h"
#include "game/actor.h"

namespace game {
struct Collision;
struct GlobalContext;
}  // namespace game

namespace game::act {

struct Player;

struct BossGyorg : Actor {
  void (*gyorg_calc)(BossGyorg* self, GlobalContext* gctx);
  u32 field_1FC;
  u8 gap_200[60];
  float field_23C;
  u8 gap_240[8];
  u32 field_248;
  u32 field_24C;
  u8 gap_250[4];
  float field_254;
  u8 gap_258[52];
  u32 field_28C;
  u8 gap_290[932];
  u32 field_634;
  u8 gap_638[456];
  u32 field_800;
  u8 gap_804[472];
  u32 field_9DC;
  u32 field_9E0;
  u32 field_9E4;
  u32 field_9E8;
  u32 field_9EC;
  u32 field_9F0;
  u32 field_9F4;
  u32 field_9F8;
  u32 field_9FC;
  u32 field_A00;
  u32 field_A04;
  u32 field_A08;
  float field_A0C;
  u16 field_A10;
  u16 field_A12;
  u8 gap_A14[2];
  u16 field_A16;
  u8 field_A18;
  u8 field_A19;
  u8 gap_A1A[3];
  u8 field_A1D;
  u8 cycle_damage;
  char field_A1F[1];
  float field_A20;
  u8 gap_A24;
  u8 downcount0;
  u16 downcount1;
  u8 gap_A28[4];
  Vec3 target_player_position;
  Vec3 field_A38;
  Vec3 field_A44;
  u16 field_A50;
  u16 field_A52;
  u8 gap_A54[8];
  u16 downcount2;
  u8 gap_A5E[2];
  u32 field_A60;
  u32 field_A64;
  u8 gap_A68[2];
  u8 eyeball_flags;
  u8 field_A6B;
  Vec3 eyeball_scale;
  Vec3 player_next_pos;
  int field_A84;
  u8 field_A88;
  u8 gap_A89[7];
  float field_A90;
  Vec3 field_A94;
  u8 gapAA0[8];
  u16 downcount3;
  u16 field_AAA;
  u16 field_AAC;
  u16 field_AAE;
  u8 gap_AB0[12];
  u8 downcount4;
  u8 gap_ABD[3];
  u32 field_AC0;
  u8 gap_AC4[20];
  u32 collision_eye_num_entries;
  Collision* collision_eye;
  int field_AE0;
  u8 gap_AE4[76];
  u32 field_B30;
  u8 gap_B34[8];
  Player* player;
  u8 gap_B40[8];
  u32 collision_armor_num_entries;
  Collision* collision_armor;
  int field_B50;
  u8 gap_B54[156];
  u32 field_BF0;
  u8 gap_BF4[12];
  u32 field_C00;
  u8 gap_C04[12];
  int field_C10;
  u8 gap_C14[316];
  u32 field_D50;
  u8 gap_D54[16];
  u8 field_D64;
  u8 gap_D65[3];
  u32 collision_num_entries;
  Collision* collision;
  int field_D70;
  u8 gap_D74[140];
  u32 field_E00;
  u8 gap_E04[172];
  float field_EB0;
  u8 field_EB4;
  u8 field_EB5;
  u16 field_EB6;
  u8 gap_EB8[2];
  u16 field_EBA;
  u8 gap_EBC[8];
  Vec3 field_EC4;
  Vec3 field_ED0;
  u8 gap_EDC[40];
  Vec3 field_F04;
  int field_F10;
  u32 field_F14;
  u32 field_F18;
  int field_F1C;
  int field_F20;
  int field_F24;
};
static_assert(sizeof(BossGyorg) == 0xF28);

}  // namespace game::act
