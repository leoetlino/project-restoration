#pragma once

#include "common/types.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/collision.h"

namespace game {
struct GlobalContext;
}

namespace game::act {

struct BossGoht : Actor {
  enum class Flag : u8 {
    ShowedEye = 0x2,
    Unk8 = 0x8,
    Unk10 = 0x10,
    Unk20 = 0x20,
    FinishedPlayingStunCutsceneOnce = 0x40,
  };

  u32 field_1F8;
  u32 field_1FC;
  u16 field_200;
  u8 gap_2[46];
  u32 field_30;
  u8 gap_34[20];
  float field_48;
  u8 gap_4C[60];
  void (*goht_calc)(BossGoht* self, GlobalContext* gctx);
  char field_28C[4];
  char field_290[2];
  char field_292[1];
  u8 field_293;
  char field_294[2];
  u8 field_296;
  char field_97;
  char field_98;
  char field_99;
  u16 field_29A;
  s16 downcount;
  u16 field_29E;
  u16 field_2A0;
  u16 field_A2;
  u16 field_A4;
  u16 field_A6;
  u8 gap_A8[2];
  s16 field_2AA;
  s16 camera_idx;
  u8 gap_2AE[2];
  unsigned int field_2B0;
  u32 field_2B4;
  float field_2B8;
  float field_2BC;
  float field_2C0;
  float field_C4;
  float field_2C8;
  float field_CC;
  float field_D0;
  float field_2D4;
  u8 gap_2D8[4];
  Vec3 field_2DC;
  u32 field_2E8;
  u8 gap_2EC[1504];
  u32 field_8CC;
  u8 gap_6D0[1328];
  u32 field_C00;
  u8 gap_C04[178];
  u16 field_EB6;
  u16 field_EB8;
  u16 field_EBA;
  Vec3 field_EBC;
  Vec3 field_EC8;
  int field_ED4;
  u8 gap_ED8[188];
  int field_F94;
  u8 gap_D98[4];
  float field_D9C;
  u8 gap_DA0[24];
  int field_FB8;
  char field_FBC[24];
  CollisionBodies<CollisionBodyCylinderCollection, 20> collision;
  CollisionBodyCylinder collision_2;
  u8 gap_168C[0x20];
  u32 field_16AC;
  u32 field_16B0;
  u32 field_16B4;
  u32 field_16B8;
  u32 field_16BC;
  u32 field_16C0;
  u32 field_16C4;
  u32 field_16C8;
  u32 field_16CC;
  u32 field_16D0;
  u32 field_16D4;
  u32 field_16D8;
  u8 gap_16DC[6436];
  u32 field_3000;
  u8 gap_3004[2044];
  u32 field_3800;
  u8 gap_3804[244];
  int field_38F8;
  int field_38FC;
  int field_3900;
  Vec3 field_3904;
  CollisionBodyType4 collision_1;
  char field_3978[4];
  u8 gap_397C[4];
  u16 cycle_life;
  u8 gap_3982[2];
  u16 field_3984;
  u8 gap_3986[8];
  u16 field_398E;
  rst::Flags<Flag> goht_flags;
  char field_3991;
  char field_3992;
  char cycle_damage;
  char field_3994;
  bool taking_real_damage;
  char field_3996;
  char field_3997;
  char field_3998;
  u8 gap_3999[3];
  u32 field_399C;
  u16 field_39A0;
  u8 gap_39A2[2];
  int field_39A4;
  int field_39A8;
  Vec3 eye_scale;
  int field_39B8;
  u8 gap_39BC[76];
  u32 field_3A08;
  u32 field_3A0C;
  u32 field_3A10;
  u32 field_3A14;
  u8 gap_3A18[24];
  int field_3A30;
  int field_3A34;
  u8 gap_3A38[4];
  float field_3A3C;
  u8 gap_3A40[96];
  u32 field_3AA0;
  int field_3AA4;
  int field_3AA8;
  int field_3AAC;
  int field_3AB0;
  int field_3AB4;
  int field_3AB8;
  int field_3ABC;
};
static_assert(sizeof(BossGoht) == 0x3AC0);

}  // namespace game::act
