#pragma once

#include "common/flags.h"
#include "common/types.h"

namespace game {

namespace act {
struct Actor;
}  // namespace act
struct GlobalContext;

// Names might be incorrect for MM3D.
// https://wiki.cloudmodding.com/mm/Notes:Mini-Cutscenes#0x02
enum class CameraState : u16 {
  NONE = 0x00,
  /// Regular gameplay
  NORMAL0 = 0x01,
  /// Regular gameplay, in water?
  NORMAL3 = 0x02,
  /// Diving as human Link?
  CIRCLE5 = 0x03,
  HORSE0 = 0x04,
  ZORA0 = 0x05,
  PREREND0 = 0x06,
  PREREND1 = 0x07,
  /// After going through a sliding door
  DOORC = 0x08,
  DEMO0 = 0x09,
  /// Cutscene?
  FREE0 = 0x0A,
  FUKAN0 = 0x0B,
  NORMAL1 = 0x0C,
  NANAME = 0x0D,
  /// Curiosity Shop, in front of owner?
  CIRCLE0 = 0x0E,
  FIXED0 = 0x0F,
  SPIRAL = 0x10,
  /// Dungeon (e.g. Stone Tower, some rooms in STT)
  DUNGEON0 = 0x11,
  ITEM0 = 0x12,
  /// Ocarina
  ITEM1 = 0x13,
  ITEM2 = 0x14,
  ITEM3 = 0x15,
  /// Song of Soaring
  NAVI = 0x16,
  WARP0 = 0x17,
  DEATH = 0x18,
  REBIRTH = 0x19,
  TREASURE = 0x1A,
  /// Transformation cutscene
  TRANSFORM = 0x1B,
  ATTENTION = 0x1C,
  WARP1 = 0x1D,
  DUNGEON1 = 0x1E,
  /// Talking to shop owner?
  FIXED1 = 0x1F,
  FIXED2 = 0x20,
  MAZE = 0x21,
  REMOTEBOMB = 0x22,
  CIRCLE1 = 0x23,
  CIRCLE2 = 0x24,
  /// Curiosity Shop?
  CIRCLE3 = 0x25,
  CIRCLE4 = 0x26,
  FIXED3 = 0x27,
  TOWER0 = 0x28,
  PARALLEL0 = 0x29,
  NORMALD = 0x2A,
  SUBJECTD = 0x2B,
  /// Initial camera after entering a map (e.g. Trading Post / Stone Tower Temple)
  START0 = 0x2C,
  /// Initial camera after entering a map (e.g. Bomb Shop, Great Bay from lab)
  START2 = 0x2D,
  STOP0 = 0x2E,
  JCRUISING = 0x2F,
  CLIMEMAZE = 0x30,
  SIDED = 0x31,
  /// Dungeon (ledge?)
  DUNGEON2 = 0x32,
  /// Dungeon (Great Bay Temple)?
  BOSS_SHIGE = 0x33,
  KEEPBACK = 0x34,
  CIRCLE6 = 0x35,
  CIRCLE7 = 0x36,
  CHUBOSS = 0x37,
  RFIXED1 = 0x38,
  TRESURE1 = 0x39,
  BOMBBASKET = 0x3A,
  CIRCLE8 = 0x3B,
  FUKAN1 = 0x3C,
  /// Dungeon (e.g. Stone Tower Temple)
  DUNGEON3 = 0x3D,
  TELESCOPE = 0x3E,
  /// Trading Post / Bomb Shop / Swordsman's School?
  ROOM0 = 0x3F,
  RCIRC0 = 0x40,
  CIRCLE9 = 0x41,
  ONTHEPOLE = 0x42,
  INBUSH = 0x43,
  BOSS_LAST = 0x44,
  BOSS_INI = 0x45,
  BOSS_HAK = 0x46,
  BOSS_KON = 0x47,
  CONNECT0 = 0x48,
  MORAY = 0x49,
  /// Trading Post, in front of shop owner?
  NORMAL2 = 0x4A,
  BOMBBOWL = 0x4B,
  CIRCLEa = 0x4C,
  WHIRLPOOL = 0x4D,
  KOKKOGAME = 0x4E,
  GIANT = 0x4F,
  /// In loading zone
  SCENE0 = 0x50,
  ROOM1 = 0x51,
  /// In water (Great Bay Temple?)
  WATER2 = 0x52,
  SOKONASI = 0x53,
  FORCEKEEP = 0x54,
  PARALLEL1 = 0x55,
  START1 = 0x56,
  ROOM2 = 0x57,
  NORMAL4 = 0x58,
  SHELL = 0x59,
  DUNGEON4 = 0x5A,
};

// https://wiki.cloudmodding.com/mm/Notes:Mini-Cutscenes#MM_.28J.29_addresses:_Camera_assembly_stuff
enum class CameraMode : u16 {
  NORMAL = 0x00,
  /// Deku hopping?
  JUMP = 0x01,
  /// Zora fast swimming or Goron fast rolling
  GORONDASH = 0x02,
  /// Deku bubble
  NUTSSHOT = 0x03,
  /// Bow or hookshot or Deku bubble (Z-target)
  BOWARROWZ = 0x04,
  /// Flying (Deku)
  NUTSFLY = 0x05,
  /// First-person mode (Pictograph Box)
  SUBJECT = 0x06,
  /// Zora boomerang (shot)
  BOOKEEPON = 0x07,
  /// Zora boomerang (not shot yet)
  ZORAFIN = 0x08,
  /// Z-Target
  KEEPON = 0x09,
  /// Z-Target with no target
  PARALLEL = 0x0A,
  // Talking
  TALK = 0x0B,
  /// Bow or hookshot
  PACHINCO = 0x0C,
  BOWARROW = 0x0D,
  BATTLE = 0x0E,
  /// Burrowed in Deku flower
  NUTSHIDE = 0x0F,
  /// Shielding or attacking
  STILL = 0x10,
  CHARGE = 0x11,
  /// Climbing
  CLIMB = 0x12,
  /// Climbing (Z-Target)
  CLIMBZ = 0x13,
  FOOKSHOT = 0x14,
  /// [State 2] Dolphin jumping
  FREEFALL = 0x15,
  HANG = 0x16,
  HANGZ = 0x17,
  PUSHPULL = 0x18,
  NUTSFLYZ = 0x19,
  /// Jumping while Goron fast rolling
  GORONJUMP = 0x1A,
  BOOMERANG = 0x1B,
  CHARGEZ = 0x1C,
  ZORAFINZ = 0x1D,
  /// [MM3D]
  UNK_1E = 0x1E,
  /// [MM3D] Free camera mode
  FREECAMERA = 0x1F,
};
constexpr size_t NumCameraModes = 0x20;

struct Camera {
  u32 ChangeMode(CameraMode mode, u32 unknown = 0);

  u32 field_0;
  u8 gap_4[76];
  Vec3 pos1;
  Vec3 pos2;
  Vec3 field_68;
  Vec3 field_74;
  float field_80;
  float field_84;
  float field_88;
  float field_8C;
  float field_90;
  u8 gap_94[4];
  float field_98;
  float field_9C;
  GlobalContext* gctx;
  act::Actor* target_actor;
  Vec3 target_actor_pos;
  int angle;
  u16 field_B8;
  u16 field_BA;
  int field_BC;
  u8 gap_C0[20];
  float field_D4;
  float field_D8;
  float field_DC;
  float field_E0;
  float field_E4;
  float field_E8;
  int field_EC;
  float field_F0;
  float field_F4;
  float field_F8;
  u32 field_FC;
  u16 field_100;
  u16 field_102;
  Vec3 distance_to_target;
  float field_110;
  float field_114;
  float field_118;
  Vec3 field_11C;
  int field_128;
  int field_12C;
  u32 field_130;
  u16 field_134;
  u16 field_136;
  u16 field_138;
  u16 field_13A;
  u16 field_13C;
  u16 field_13E;
  u16 field_140;
  u16 field_142;
  u16 field_144;
  u16 field_146;
  u16 field_148;
  u16 field_14A;
  u16 field_14C;
  __attribute__((packed)) __attribute__((aligned(1))) int field_14E;
  u16 field_152;
  u16 field_154;
  CameraState state;
  CameraMode mode;
  u16 field_15A;
  u16 field_15C;
  u16 field_15E;
  u16 field_160;
  u16 field_162;
  u16 field_164;
  u16 field_166;
  u16 field_168;
  u16 field_16A;
  u16 field_16C;
  u16 field_16E;
  u16 field_170;
  u16 field_172;
  u16 field_174;
  u16 field_176;
  u16 field_178;
  u16 field_17A;
  u16 field_17C;
  u16 field_17E;
  float field_180;
  float field_184;
  float field_188;
  float field_18C;
  float field_190;
  u16 field_194;
  u16 field_196;
};
static_assert(sizeof(Camera) == 0x198);

struct CameraModeInfo {
  u16 handler_fn_idx;
  u16 field_2;
  uintptr_t field_4;
};
static_assert(sizeof(CameraModeInfo) == 0x8);

struct CameraStateInfo {
  rst::BitSet<NumCameraModes, u32, CameraMode> allowed_modes;
  u32 flags;
  std::array<CameraModeInfo, NumCameraModes>* modes;
};
static_assert(sizeof(CameraStateInfo) == 0xc);

}  // namespace game
