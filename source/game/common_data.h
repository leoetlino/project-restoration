#pragma once

#include <array>

#include "common/types.h"
#include "common/utils.h"
#include "game/items.h"
#include "game/player.h"

namespace game {

enum class SwordType : u16 {
  NoSword = 0,
  KokiriSword = 1,
  RazorSword = 2,
  GildedSword = 3,
};

enum class ShieldType : u16 {
  NoShield = 0,
  HeroShield = 1,
  MirrorShield = 2,
};

enum class BombBag : u32 {
  NoBag = 0,
  BombBag20 = 1,
  BombBag30 = 2,
  BombBag40 = 3,
};

enum class Quiver : u32 {
  NoQuiver = 0,
  Quiver30 = 1,
  Quiver40 = 2,
  Quiver50 = 3,
};

struct __attribute__((packed)) __attribute__((aligned(2))) PlayerData {
  u32 field_11C;
  u8 gap_120[2];
  u16 save_count_maybe;
  int anonymous_d;
  int anonymous_e;
  int anonymous_f;
  int anonymous_g;
  u8 anonymous_h[2];
  u16 health_max;
  u16 health_current;
  char magic_size_type;
  char magic;
  u16 rupee_count;
  u16 razor_sword_hp;
  u16 anonymous_k;
  char magic_stuff;
  s8 magic_num_upgrades;
  char anonymous_17;
  char anonymous_18;
  char anonymous_19;
  char anonymous_20;
  u16 owl_statue_flags;
  char field_2E;
  char field_2F;
  char field_30;
  char field_31;
};
static_assert(sizeof(PlayerData) == 0x32);
static_assert(rst::util::OffsetOf(&PlayerData::magic) == 0x1F);

union FormEquipmentData {
  std::array<ItemId, 5> item_btns;
  struct {
    ItemId item_btn_b;
    ItemId item_btn_y;
    ItemId item_btn_x;
    ItemId item_btn_i;
    ItemId item_btn_ii;
  };
};

struct EquipmentData {
  FormEquipmentData data[4];
  char field_14;
  ItemId item_on_y_readonly;
  ItemId item_on_x_readonly;
  ItemId item_on_i_readonly;
  ItemId item_on_ii_readonly;
  char field_19;
  char field_1A;
  char field_1B;
  char field_1C;
  char field_1D;
  char field_1E;
  char field_1F;
  char field_20;
  char field_21;
  char field_22;
  char field_23;
  char field_24;
  char field_25;
  char field_26;
  char field_27;
  union SwordShield {
    u16 raw;

    BitField<0, 4, SwordType> sword;
    BitField<4, 8, ShieldType> shield;
  };
  SwordShield sword_shield;
};

struct InventoryData {
  std::array<ItemId, 24> items;
  std::array<ItemId, 24> masks;
  std::array<u8, 24> item_counts;
  u8 field_48[24];
  u8 field_60[24];
  union InventoryCountRegister {
    u32 raw;

    BitField<0, 3, Quiver> quiver_upgrade;
    BitField<3, 3, BombBag> bomb_bag_upgrade;
    BitField<6, 6, u32> pad_1;
    BitField<12, 2, u32> wallet_upgrade;
    BitField<14, 3, u32> pad_2;
    BitField<17, 3, u32> stick_upgrades;
    BitField<20, 3, u32> nut_upgrade;
    BitField<23, 9, u32> pad_3;
  };
  InventoryCountRegister inventory_count_register;
  union CollectRegister {
    u32 raw;
    
    BitField<0, 1, u32> odolwas_remains;
    BitField<1, 1, u32> gohts_remains;
    BitField<2, 1, u32> gyorgs_remains;
    BitField<3, 1, u32> twinmolds_remains;
    BitField<4, 2, u32> pad_1;
    BitField<6, 1, u32> sonata_of_awakening;
    BitField<7, 1, u32> goron_lullaby;
    BitField<8, 1, u32> new_wave_bossa_nova;
    BitField<9, 1, u32> elegy_of_emptiness;
    BitField<10, 1, u32> oath_to_order;
    BitField<11, 1, u32> sarias_song;
    BitField<12, 1, u32> song_of_time;
    BitField<13, 1, u32> song_of_healing;
    BitField<14, 1, u32> eponas_song;
    BitField<15, 1, u32> song_of_soaring;
    BitField<16, 1, u32> song_of_storms;
    BitField<17, 1, u32> suns_song;
    BitField<18, 1, u32> bombers_notebook;
    BitField<19, 5, u32> pad_2;
    BitField<24, 1, u32> lullaby_intro;
    BitField<25, 3, u32> pad_3;
    BitField<28, 4, u32> heart_container_pieces;
  };
  CollectRegister collect_register;
  char anonymous_33[1];
  char anonymous_34[3];
  u8 gap200[6];
  char anonymous_35[1];
  char anonymous_36;
  char anonymous_37;
  char anonymous_38;
  u8 gap20A[5];
  char anonymous_39;
  char anonymous_40;
  char anonymous_41;
  char anonymous_42;
  char anonymous_43;
  char gap98[60];
};
static_assert(sizeof(InventoryData) == 0xD4);
static_assert(offsetof(InventoryData, inventory_count_register) == 0x78);

struct SaveData {
  MaskId mask;
  bool has_completed_intro;
  char unused;
  // Possible cutscene ID?
  char anonymous_0;
  bool is_night;
  /// Number of extra time units to add per game tick (0 normally; -1 with ISoT)
  /// In Majora's Mask, ISoT used to set this to -2.
  int extra_time_speed;
  /// In-game day
  int day;
  int total_day;
  int cutscene_stuff;
  /// In-game time.
  /// 0x0000 is midnight, 0x4000 is 6am, 0x8000 is noon, 0xc000 is 6pm.
  u16 time;
  u16 anonymous_3;
  u16 anonymous_4;
  act::Player::Form player_form;
  char anonymous_5;
  char field_20;
  char anonymous_7;
  char anonymous_8;
  char anonymous_9;
  char anonymous_10;
  char anonymous_11;
  char anonymous_12;
  char anonymous_13;
  char anonymous_14;
  char anonymous_15;
  char anonymous_16;
  char gap33[205];
  char anonymous_a[24];
  char anonymous_b;
  u8 gap_115[7];
  PlayerData player;
  EquipmentData equipment;
  InventoryData inventory;
  char field_24C;
  u8 gap249[1235];
  int anonymous_44;
  u8 gap728[384];
  char anonymous_45;
  u8 gap8A9[1023];
  int anonymous_46;
  u8 gapCAC[1284];
  int anonymous_47;
  int anonymous_48;
  int anonymous_49;
  int anonymous_50;
  int anonymous_51;
  u8 gap11C4[12];
  int anonymous_52;
  int anonymous_53;
  int anonymous_54;
  int anonymous_55;
  int anonymous_56;
  int anonymous_57;
  int anonymous_58;
  u8 gap11EC[36];
  union SkulltulaRegister {
    int raw;

    BitField<0, 16, int> swamp_count;
    BitField<16, 16, int> ocean_count;
  };
  SkulltulaRegister skulltulas_collected;
  int anonymous_60;
  u8 gap1218[4];
  int anonymous_61;
  int anonymous_62;
  int anonymous_63;
  u8 gap1228[8];
  int anonymous_64;
  u8 gap1234[8];
  int anonymous_65;
  int anonymous_66;
  int anonymous_67;
  int anonymous_68;
  u8 gap124C[5];
  char anonymous_69;
  char anonymous_70;
  u8 gap1253[4];
  char anonymous_71;
  char anonymous_72;
  char anonymous_73;
  char anonymous_74;
  char anonymous_75;
  char anonymous_76;
  char anonymous_77;
  u8 flag_8_for_no_magic_use;
  char anonymous_78;
  char anonymous_79;
  char anonymous_80;
  char anonymous_81;
  char anonymous_82;
  char anonymous_83;
  char anonymous_84;
  char anonymous_85;
  char anonymous_86;
  char anonymous_87;
  char anonymous_88;
  char anonymous_89;
  char anonymous_90;
  char anonymous_91;
  char anonymous_92;
  char anonymous_93;
  char anonymous_94;
  char anonymous_95;
  char anonymous_96;
  char anonymous_97;
  char anonymous_98;
  char anonymous_99;
  char anonymous_100;
  char anonymous_101;
  char anonymous_102;
  char anonymous_103;
  char anonymous_104;
  u8 gap127A[8];
  char anonymous_105;
  char anonymous_106;
  char anonymous_107;
  char anonymous_108;
  char anonymous_109;
  char anonymous_110;
  char anonymous_111;
  char anonymous_112;
  char anonymous_113;
  char anonymous_114;
  char anonymous_115;
  char anonymous_116;
  char anonymous_117;
  char anonymous_118;
  char anonymous_119;
  char anonymous_120;
  u8 gap1292[7];
  char anonymous_121;
  char anonymous_122;
  char anonymous_123;
  char anonymous_124;
  char anonymous_125;
  char anonymous_126;
  char anonymous_127;
  char anonymous_128;
  char anonymous_129;
  char anonymous_130;
  char anonymous_131;
  char anonymous_132;
  char anonymous_133;
  char anonymous_134;
  char anonymous_135;
  char anonymous_136;
  char anonymous_137;
  char anonymous_138;
  char anonymous_139;
  char anonymous_140;
  char anonymous_141;
  u8 gap12AE[6];
  char anonymous_142;
  char anonymous_143;
  char anonymous_144;
  u8 gap12B7[3];
  char anonymous_145;
  char anonymous_146;
  u8 gap12BC[13];
  char anonymous_147;
  char anonymous_148[6];
  char anonymous_149;
  char anonymous_150;
  char anonymous_151;
  char anonymous_152;
  char anonymous_153;
  char anonymous_154;
  char anonymous_155;
  char anonymous_156;
  char anonymous_157;
  char anonymous_158;
  char anonymous_159;
  char anonymous_160;
  u8 gap12DC[20];
  int anonymous_161;
  int anonymous_162;
  u8 gap12F8;
  char anonymous_163;
  u8 gap12FA[128];
  char anonymous_164;
  char anonymous_165;
  char anonymous_166;
  char anonymous_167;
  char anonymous_168;
  char anonymous_169;
  char anonymous_170;
  char anonymous_171;
  char anonymous_172;
  char anonymous_173;
  char anonymous_174;
  char anonymous_175;
  char anonymous_176;
  char anonymous_177;
  char anonymous_178;
  char anonymous_179;
  u8 gap138A[5];
  char anonymous_180;
  char anonymous_181;
  char anonymous_182;
  char anonymous_183;
  char anonymous_184;
  char anonymous_185;
  char anonymous_186[3];
  u16 anonymous_187;
  u16 anonymous_188;
  u16 anonymous_189;
  u16 anonymous_190;
  u16 anonymous_191;
  u8 gap13A2[326];
  char anonymous_192;
  char anonymous_193;
  char anonymous_194;
  char anonymous_195;
  char anonymous_196;
  char anonymous_197;
  u16 anonymous_198;
  u8 gap_14E8[136];
  u32 field_1570;
  u8 gap_1574[1020];
  u32 field_1970;
  u8 gap_1974[176];
  char anonymous_199;
  char anonymous_200;
  u16 anonymous_201;
  u8 gap1A30[20];
  char anonymous_202;
  char anonymous_203;
  char anonymous_204;
  char anonymous_205;
  char anonymous_206;
  char anonymous_207;
  char anonymous_208;
  char anonymous_209;
  char anonymous_210;
  char anonymous_211;
  char anonymous_212;
  char anonymous_213;
  char anonymous_214;
  char anonymous_215;
  char anonymous_216;
  char anonymous_217;
  char anonymous_218;
  char anonymous_219;
  char anonymous_220;
  char anonymous_221;
  char anonymous_222;
  char anonymous_223;
  char anonymous_224;
  char anonymous_225;
  u8 gap1A5C[8];
  u16 anonymous_226;
  char anonymous_227[2];
  u8 gap_1A60[16];
  u32 field_1A70;
  u8 gap_1A74[12];
  u16 anonymous_228;
  u16 anonymous_229;
  int field_1A84;
};
static_assert(sizeof(SaveData) == 0x1A88);

struct CommonDataSub1 {
  int entrance;
  u8 field_4;
  u8 field_5;
  u8 field_6;
  u8 field_7;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
};

struct CommonDataSub3 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
};

struct CommonDataSub4 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
};

struct CommonDataSub5 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
};

struct CommonDataSub6 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
};

struct CommonDataSub7 {
  u64 field_0;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
};

struct CommonDataSub8 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
};

struct CommonDataSub9 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
};

struct CommonDataSub10 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
};

struct CommonDataSub11 {
  u64 field_0;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
};

struct CommonDataSub12 {
  u32 field_0;
  int field_4;
  int field_8;
  int field_C;
  int field_10;
  int field_14;
};

struct RespawnData {
  Vec3 pos; 
  s16 yaw;
  s16 player_params;
  s16 entrance_index;
  u8 room_index;
  s8 data;
  u32 temp_swch_flags_maybe;
  u8 field_1;
  u8 btn_y_can_use_item;
  u8 btn_x_can_use_item;
  u8 btn_i_can_use_item;
  //u32 stored_mask_id_maybe;
  u32 temp_collect_flags_maybe;
}; 
static_assert(sizeof(RespawnData) == 0x20);

enum class UsableButton : u8 {
  B = 0,
  Y = 1,
  X = 2,
  I = 3,
  II = 4,
  Unknown5 = 5,
  Ocarina = 6,
  PictographBox = 7,
  Unknown8 = 8,
};

enum class ButtonIsUsable : u8 {
  Yes = 0,
  No = 0xff,
  // Other values are possible and can be seen when riding Epona.
};

/// Common gameplay data, also known as the Save Context (unofficially).
struct CommonData {
  FormEquipmentData& GetFormEquipment() {
    const auto idx = save.player_form == act::Player::Form::Human ? 0 : u8(save.player_form);
    return save.equipment.data[idx];
  }
  const FormEquipmentData& GetFormEquipment() const {
    const auto idx = save.player_form == act::Player::Form::Human ? 0 : u8(save.player_form);
    return save.equipment.data[idx];
  }

  int start;
  int scene;
  SaveData save;
  SaveData save_backup;
  CommonDataSub1 sub1;
  u64 unknown_1;
  u64 unknown_2;
  u64 unknown_3;
  CommonDataSub3 sub3;
  CommonDataSub4 sub4;
  CommonDataSub5 sub5;
  CommonDataSub6 sub6;
  CommonDataSub7 sub7;
  CommonDataSub8 sub8;
  CommonDataSub9 sub9;
  CommonDataSub10 sub10;
  CommonDataSub11 sub11;
  CommonDataSub12 sub12;
  u8 gap_3668[14];
  __attribute__((packed)) __attribute__((aligned(1))) int field_3676;
  __attribute__((packed)) __attribute__((aligned(1))) int field_367A;
  __attribute__((packed)) __attribute__((aligned(1))) int field_367E;
  u16 field_3682;
  u16 field_3684;
  u16 field_3686;
  u16 field_3688;
  s16 magic_max;
  u16 field_368C;
  s16 magic_cost;
  u16 field_3690;
  u16 field_3692;
  u16 field_3694;
  u16 field_3696;
  u16 field_3698;
  u16 field_369A;
  u16 field_369C;
  u16 field_369E;
  u16 time_copy_2;
  // Used for scheduling NPCs?
  u16 time_copy;
  u8 field_36A4[32];
  u32 field_36C4;
  u8 gap_36C8[4];
  // MPO data.
  u8 pictograph_data[65356];

  // Data below isn't read from or written to save files.

  u16 save_idx;
  u8 gap_1361A[2];
  int setup;
  int setup2;
  int field_13624;
  RespawnData sub13s[8];
  u32 field_13728;
  int field_1372C;
  char field_13730;
  char field_13731;
  u16 field_13732;
  u16 field_13734;
  u16 field_13736;
  int field_13738;
  // 0-19
  int sound_env_index;

  /// see also UsableButton and ButtonIsUsable
  ButtonIsUsable usable_btns[9];
  u8 field_13749;
  u16 alpha_type;
  u16 alpha_type_2;
  u16 alpha_type_flag;
  u16 field_13750;
  u16 field_13752;
  u16 field_13754;
  u16 field_13756;

  int field_13758;
  char field_1375C;
  char field_1375D;
  char field_1375E;
  char field_1375F;
  u16 time_copy_3;
  char field_13762;
  char field_13763;
  u8 gap_13764[2204];
  u32 field_14000;
  u8 gap_14004[192];
  int field_140C4;
  int field_140C8;
  int field_140CC;
  int field_140D0;
  int field_140D4;
  u32 field_140D8;
  int field_140DC;
  int field_140E0;
  int field_140E4;
  int field_140E8;
  int field_140EC;
  char field_140F0;
  u16 field_140F2;
  int field_140F4;
};
static_assert(sizeof(CommonData) == 0x140F8);

CommonData& GetCommonData();

}  // namespace game
