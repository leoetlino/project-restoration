#pragma once

#include "common/flags.h"
#include "common/types.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/context.h"
#include "game/items.h"
#include "game/pad.h"

namespace game::act {

enum class FormParamIndex : u8 {
  FierceDeity = 0,
  Human = 1,
  Giant = 2,
  Deku = 3,
  Zora = 4,
  ZoraDiving = 5,
  Goron = 6,
  Unknown = 7,
};

struct FormParam {
  u16 run_accel;
  u16 run_decel;
  u16 field_4;
  u16 field_6;
  u16 field_8;
  u16 field_A;
  u16 field_C;
  u16 field_E;
  u16 field_10;
  u16 field_12;
  u16 roll_decel_maybe;
  u16 walk_speed;
  u16 field_18;
  u16 field_1A;
  u16 field_1C;
  u16 field_1E;
  u16 field_20;
  u16 field_22;
  u16 field_24;
  u16 field_26;
};

FormParam& GetFormParam(FormParamIndex idx);

// XXX: Very incomplete.
struct Player : public Actor {
  enum class Form : u8 {
    FierceDeity = 0,
    Goron = 1,
    Zora = 2,
    Deku = 3,
    Human = 4,
  };

  enum class ActionType : u8 {
    Type1 = 1,
    Type2 = 2,
    Type3 = 3,
    Type4 = 4,
    OcarinaOrTransformation = 5,
  };

  enum class Flag1 : u32 {
    Locked = 0x1,
    Unk2 = 0x2,
    Unk4 = 0x4,
    Unk8 = 0x8,
    IsUsingZoraBarrier = 0x10,
    Unk20 = 0x20,
    Unk40 = 0x40,
    Dead = 0x80,
    Unk100 = 0x100,
    Unk200 = 0x200,
    Unk400 = 0x400,
    Unk800 = 0x800,
    ChargingSpinAttack = 0x1000,
    Unk2000 = 0x2000,
    Unk4000 = 0x4000,
    ZTargeting = 0x8000,
    ZTargetingWithTarget = 0x10000,
    ZTargetingWithoutTarget = 0x20000,
    InAir = 0x40000,
    Unk80000 = 0x80000,
    FirstPersonMode = 0x100000,
    Unk200000 = 0x200000,
    Shielding = 0x400000,
    Riding = 0x800000,
    LargeZoraFins = 0x1000000,
    ZoraFinsShot = 0x2000000,
    Unk4000000 = 0x4000000,
    InWater = 0x8000000,
    FreezeWorld = 0x10000000,
    FreezeLink = 0x20000000,
    Unk40000000 = 0x40000000,
    Unk80000000 = 0x80000000,
  };

  enum class Flag2 : u32 {
    Moving = 0x20,
    // Set when in the "held" state.
    Unk40 = 0x40,
    // Set when the player can stay in the hold state.
    Held = 0x80,
    Underwater = 0x400,
    TatlFlying = 0x0010'0000,
    PlayingIdleAnimation = 0x1000'0000,
  };

  enum class Flag3 : u32 {
    Unk1 = 0x1,
    Unk2 = 0x2,
    Unk4 = 0x4,
    AttackingB = 0x8,
    Unk10 = 0x10,
    Unk20 = 0x20,
    HasProjectileActor = 0x40,
    Unk80 = 0x80,
    DekuInFlower = 0x100,
    DekuLaunching = 0x200,
    DekuStuffMaybe = 0x400,
    Unk800 = 0x800,
    GoronRolling = 0x1000,
    DekuFlyingCamera = 0x2000,
    Unk4000 = 0x4000,
    ZoraFastSwimming = 0x8000,
    Unk10000 = 0x10000,
    AfterChangeMask = 0x20000,
    Unk40000 = 0x40000,
    GoronRollingFast = 0x80000,
    DekuSpin = 0x100000,
    Unk200000 = 0x200000,
    Unk400000 = 0x400000,
    AfterUseBoomerang = 0x800000,
    DekuNutsOnB = 0x1000000,
    Unk2000000 = 0x2000000,
    Unk4000000 = 0x4000000,
    Unk8000000 = 0x8000000,
    Unk10000000 = 0x10000000,
    Unk20000000 = 0x20000000,
    Unk40000000 = 0x40000000,
    Unk80000000 = 0x80000000,
  };

  enum class ProjectileType : s16 {
    Arrow = 1,
    DekuBubble = 2,
    Hookshot = 3,
  };

  struct ArrowInfo {
    bool can_use;
    u16 actor_param;
    ItemId item_id;
  };
  ArrowInfo GetArrowInfo(GlobalContext* gctx) const;

  bool SetTalkActor(u8 new_fn1_idx, Actor* actor);

  char field_1F8;
  FormParamIndex form_param_idx;
  u8 item_btn_slot;
  Action current_action;
  ItemId held_item;
  FormParamIndex form_param_idx2;
  Action action;
  Form active_form;
  Action transform_mask_action;
  char field_201;
  char field_202;
  u8 gap_203[4];
  u8 field_207;
  u8 gap208[3];
  MaskId active_mask_id;
  char field_20C;
  MaskId previous_mask_id;
  char field_20E;
  u8 gap_20F[293];
  u32 field_334;
  u8 gap_338[48];
  u32 field_368;
  u8 gap_36C[4];
  u32 field_370;
  float field_374;
  u8 gap_378[4];
  float field_37C;
  float field_380;
  u8 gap_384[4];
  float field_388;
  u8 gap_38C[6];
  __attribute__((packed)) __attribute__((aligned(1))) u32 field_392;
  u8 gap_396[30];
  u32 field_3B4;
  u32 field_3B8;
  u8 gap_3BC;
  u8 gap_3BD[19];
  u32 field_3D0;
  u8 gap_3D4[128];
  u32 field_454;
  u8 gap_458[24];
  u32 field_470;
  u8 gap_474[652];
  u32 field_700;
  u8 gap_704[12];
  u32 field_710;
  u8 gap_714[196];
  u32 field_7D8;
  u8 gap_7DC[136];
  int field_864;
  int field_868;
  u8 gap_86C[116];
  u32 field_8E0;
  Actor* projectile_actor;
  u8 gap_8E8[8];
  float field_8F0;
  u8 gap_8F4[8];
  u32 field_8FC;
  u32 field_900;
  u8 gap_904[16];
  char field_914[12];
  int field_920;
  u8 gap_924[4];
  u32 field_928;
  u8 fn1_idx;
  u8 fn2_idx;
  char field_92E;
  char field_92F;
  Actor* talk_actor2;
  u8 gap_934[4];
  Vec3 field_938;
  u32 field_944;
  u8 gap_948[10];
  u16 talking;
  s16 field_954;
  u8 gap_956[22];
  char field_96C;
  __attribute__((aligned(2))) u8 gap_96E[50];
  u32 field_9A0;
  u32 field_9A4;
  u8 gap_9A8[20];
  u32 field_9BC;
  u8 gap_9C0[12];
  char field_9CC;
  char field_9CD;
  char field_9CE;
  char field_9CF;
  u8 gap_9D0[4];
  int field_9D4;
  u8 gap_9D8[4];
  float field_9DC;
  u8 gap_9E0[28];
  float field_9FC;
  float field_A00;
  float field_A04;
  u8 gapA08[1];
  u8 gapA09[11];
  u16 field_A14;
  u8 gap_A16[126];
  u16 field_A94;
  u8 gap_A96[126];
  u16 field_B14;
  u8 gap_B16[126];
  u16 field_B94;
  u8 gap_B96[126];
  u16 field_C14;
  u8 gap_C16[126];
  u32 field_C94;
  u8 gap_C98[13];
  char field_CA5[7];
  u32 field_CAC;
  u8 gap_CB0[17];
  char field_CC1[1];
  char field_CC2[2];
  u32 field_CC4;
  u8 gap_CC8[4];
  u32 field_CCC;
  u8 gap_CD0[4];
  float field_CD4;
  float field_CD8;
  float field_CDC;
  u8 gap_CE0[12];
  u16 field_CEC;
  u8 gap_CEE[270];
  /// Z-target actor.
  Actor* target_actor;
  u8 gap_E00[4];
  u32 field_E04;
  u32 field_E08;
  u32 field_E0C;
  u32 field_E10;
  void (*state_handler_fn)(Player*, GlobalContext*);
  u8 gap_E18[4763];
  u8 field_20B3;
  u8 gap_20B4[61260];

  char field_11000;
  u8 gap1[3071];
  u32 field_11C00;
  u8 gap_11C04[252];
  u32 field_11D00;
  u8 gap_11D04[168];
  u32 field_11DAC;
  rst::Flags<Flag1> flags1;
  rst::Flags<Flag2> flags2;
  rst::Flags<Flag3> flags3;
  u32 flags4;
  int field_11DC0;
  u32 field_11DC4;
  Actor* tatl;
  u16 tatl_msgid;
  char field_11DCE;
  Action pending_action;
  Actor* talk_actor;
  int field_11DD4;
  int field_11DD8;
  float field_11DDC;
  u8 gap_11DE0[4];
  int field_11DE4;
  float field_11DE8;
  float field_11DEC;
  u8 gap_11DF0;
  ActionType action_type;
  u16 field_11DF2;
  u16 field_11DF4;
  u16 field_11DF6;
  u16 field_11DF8;
  u8 gap_11DFA;
  u8 gap_11DFB;
  u16 field_11DFC;
  u16 field_11DFE;
  u16 field_11E00;
  __attribute__((packed)) __attribute__((aligned(1))) u32 field_11E02;
  u16 field_11E06;
  u16 field_11E08;
  u16 field_11E0A;
  u16 field_11E0C;
  u16 field_11E0E;
  u16 field_11E10;
  u8 gap_11E12[2];
  u32 field_11E14;
  float field_11E18;
  float field_11E1C;
  u32 field_11E20;
  u8 gap_11E24[8];
  u32 field_11E2C;
  float lin_vel;
  u16 player_angle;
  u16 field_11E36;
  u32 field_11E38;
  u8 gap_11E3C;
  char field_11E3D[1];
  char field_111E3E;
  u8 field_11E3F;
  char field_11E40;
  char field_11E41;
  u8 gap_11E42[10];
  char field_11E4C;
  char field_11E4D;
  /// General purpose timer.
  /// Reset to 0 on every state handler change.
  u16 timer;
  u8 gap_11E50[4];
  u16 field_11E54;
  u8 gap_11E56[22];
  float lin_vel_xxx;
  float lin_vel_xxx2;
  float field_11E74;
  float field_11E78;
  u32 field_11E7C;
  u32 field_11E80;
  u32 field_11E84;
  float field_E88;
  ProjectileType projectile_type;
  u16 field_11E8E;
  u8 gap_11E90;
  char field_11E91;
  char field_11E92;
  char field_11E93;
  float field_11E94;
  u32 field_11E98;
  u8 gap_11E9C[20];
  float lin_vel_max;
  int field_11EB4;
  float field_11EB8;
  float field_11EBC;
  u8 gap_11EC0[6];
  char field_11EC6;
  char field_11EC7;
  u16 field_11EC8;
  u16 zora_barrier_timer;
  u16 field_11ECC;
  char field_11ECE[1];
  char field_11ECF;
  u8 gapED0[3];
  u8 gap_11ED3;
  u32 field_11ED4;
  u8 gap_11ED8[4];
  u32 field_11EDC;
  u8 gap_11EE0;
  char field_11EE1[11];
  float field_11EEC;
  u16 field_11EF0;
  u16 is_zora_slow_swim;
  u16 field_EF4;
  u16 field_EF6;
  u16 field_EF8;
  u16 field_EFA;
  u16 zora_swim_a_press_duration;
  u16 zora_fast_swim_countdown;
  u16 field_F00;
  u16 field_F02;
  u16 field_F04;
  u16 field_F06;
  u32 field_11F08;
  u32 field_11F0C;
  u8 gap_11F10[20];
  u32 field_11F24;
  u8 gap_11F28[20];
  u32 field_11F3C;
  u8 gap_11F40[48];
  u32 field_11F70;
  u8 gap_11F74[140];
  u32 field_12000;
  u8 gap_12004[110];
  __attribute__((packed)) __attribute__((aligned(1))) int field_12072;
  u8 gap_1076[66];
  u32 field_10B8;
  u8 gap_10BC[172];
  u32 field_1168;
  u8 gap_116C[492];
  u32 field_1358;
  u8 gap_135C[160];
  u32 field_13FC;
  u8 gap_1400[160];
  u32 field_14A0;
  u8 gap_14A4[860];
  int field_12800;
  u8 gap_1804[436];
  u32 field_19B8;
  u32 field_129BC;
  u8 gap_129C0[12];

  pad::ControllerInfo controller_info;
  u8 gap_12A14[36];
  u32 field_12A38;
  u8 gap_12A3C[4];
  u32 field_12A40;
  u8 gap_12A44[12];
  u32 field_12A50;
  u8 gap_12A54[28];
  u32 field_12A70;
  u8 gap_12A74[40];
  s16 field_12A9C;
  s16 field_12A9E;
  u32 field_12AA0;
  u16 field_12AA4;
  u8 gap_12AA6[62];
  float field_12AE4;
  u8 gap_12AE8[4];
  bool field_12AEC;
  u8 field_12AED;
  u8 gap_12AEE[2];
  u8 gap_12AF0[292];
  float field_12C14;
  u8 gap_12C18[32];
  float field_12C38;
  u8 gap_12C3C[8];
  float field_12C44;
  u8 gap_12C48[134];
  s16 field_12CCE;
};
static_assert(rst::util::OffsetOf(&Player::transform_mask_action) == 0x200);
static_assert(rst::util::OffsetOf(&Player::field_12CCE) == 0x12CCE);
// TODO: complete the struct and add a size assertion.

enum class AllowExistingMagicUsage { No, Yes };
bool PlayerUpdateMagicCost(game::GlobalContext* gctx, int cost, int mode,
                           AllowExistingMagicUsage allow_existing_usage);

}  // namespace game::act
