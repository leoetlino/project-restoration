#pragma once

#include <cstddef>

#include "common/types.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/camera.h"
#include "game/pad.h"
#include "game/scene.h"
#include "game/states/state.h"

namespace game {

namespace act {
class BossTwinmold;
class ObjElegyStatue;
class Player;
}  // namespace act

namespace ui {
class PlayHud;
}

enum class PauseFlag : u16 {
  PauseCalc = 1 << 0,
  PauseDraw = 1 << 1,
  PauseAll = PauseCalc | PauseDraw,
};

// Keeps track of spawned actors.
struct ActorList {
  u32 num_actors;
  act::Actor* first;
  /// Set this after changing an actor's type to update the linked lists.
  bool need_to_update_lists;
};
static_assert(sizeof(ActorList) == 0xc);

struct ActorLists {
  ActorList& GetList(act::Type type) { return lists[u8(type)]; }
  const ActorList& GetList(act::Type type) const { return lists[u8(type)]; }
  u8 gap_0[4];
  u8 field_4;
  u8 field_5;
  u8 gap_6[6];
  u8 num_actors;
  std::array<ActorList, 12> lists;
};
static_assert(sizeof(ActorLists) == 0xa0);

enum class OcarinaState : u16 {
  NotOpened = 0,
  Playing = 1,
  PlayingAndReplayDone = 3,
  StoppedPlaying = 4,
  PlayedSongOfTime = 0x12,
  PlayedInvertedSongOfTime = 0x13,
  GoingBackInTime = 0x16,
  RestoringTimeToNormalSpeed = 0x18,
  SlowingTime = 0x19,
  JumpingForwardInTime = 0x1a,
  WarpingToGreatBayCoast = 0x1c,
  WarpingToZoraCape = 0x1d,
  WarpingToSnowhead = 0x1e,
  WarpingToMountainVillage = 0x1f,
  WarpingToClockTown = 0x20,
  WarpingToMilkRoad = 0x21,
  WarpingToWoodfall = 0x22,
  WarpingToSouthernSwamp = 0x23,
  WarpingToIkanaCanyon = 0x24,
  WarpingToStoneTower = 0x25,
};

// Incomplete
enum class OcarinaSong : u16 {
  SonataOfAwakening = 0,
  GoronLullaby = 1,
  NewWaveBossaNova = 2,
  ElegyOfEmptiness = 3,
  OathToOrder = 4,
  SongOfTime = 6,
  SongOfHealing = 7,
  EponaSong = 8,
  SongOfSoaring = 9,
  SongOfStorms = 10,
  InvertedSongOfTime = 12,
  SongOfDoubleTime = 13,

  InvalidDetecting = 0xfe,
  Invalid = 0xff,
};

struct HudState {
  u32 field_0;
  u8 gap_4[86];
  u16 field_5A;
  u8 gap_5C[380];
  u32 field_1D8;
  u8 gap_1DC[36];
  u32 field_200;
  u8 gap_204[28];
  u16 field_220;
  s16 a_btn_num;
  s16 field_224;
  u8 gap_226[2];
  float field_228;
  u8 gap_22C[4];
  u16 field_230;
  u8 gap_232[10];
  s16 field_23C;
  u8 num_carrots;
  u8 gap_23F[5];
  u16 field_244;
  u16 field_246;
  u16 field_248;

  // From 0 (invisible) to 0xFF (fully visible).
  u16 a_btn_opacity;
  u16 b_btn_opacity;
  u16 item_btn_opacity[5];
  u16 camera_opacity;
  u16 field_25A;

  u8 gap_25C[6];
  u16 field_262;
  u16 field_264;
  char field_266;
  char field_267;
  u16 field_268;
  u8 gap_26A[2];
  char field_26C;
  char field_26D;
  u16 needLoadPerfectLayout;
  u8 field_270;
  u8 field_271;
  u8 field_272;
  bool prohibit_gear_or_bottles;
  int field_274;
  u8 field_278;
  bool prohibit_masks;
  bool prohibit_ocarina;
  bool prohibit_regular_items;
  u8 field_27C;
};
static_assert(sizeof(HudState) == 0x280);

// Likely incomplete.
struct GlobalContext : State {
  bool IsPaused() const { return pause_flags.IsOneSet(PauseFlag::PauseCalc, PauseFlag::PauseDraw); }

  act::Actor* FindActorWithId(act::Id id, act::Type type) const;
  template <typename T>
  T* FindActorWithId(act::Id id, act::Type type) const {
    return static_cast<T*>(FindActorWithId(id, type));
  }
  bool IsActorVisible(act::Id id, act::Type type) const {
    const auto* actor = FindActorWithId(id, type);
    return actor && actor->draw_fn;
  }

  act::Player* GetPlayerActor() const;

  act::Actor* SpawnActor(act::Actor* actor, act::Id id, u16 rx, u16 ry, u16 rz, u16 param,
                         Vec3 pos);
  act::Actor* SpawnActor(act::Id id, u16 rx, u16 ry, u16 rz, u16 param, Vec3 pos);
  void ChangeActorType(act::Actor& actor, act::Type type);

  void ShowMessage(u16 msgid, int unknown = 0);
  void Talk(act::Actor* actor, int a = 0);

  void EmitLastDamageSound(const act::Actor& actor);

  SceneId scene;
  u16 field_14A;
  u8 gap_14C[200];
  float field_214;
  float field_218;
  float field_21C;
  u8 gap_220[132];
  float field_2A4;
  u8 gap_2A8[228];
  u8 field_38C;
  u8 gap_38D[123];
  Camera main_camera;
  Camera other_cameras[3];
  Camera* cameras[4];
  u16 new_camera_idx;
  u16 camera_idx;
  u8 gap_A7C[12];
  u32 field_A88;
  u8 gap_A8C[32];
  rst::Flags<PauseFlag> pause_flags;
  u32 field_AB0;
  u8 gap_AB4[76];
  int field_B00;
  u8 gap_B04[5372];
  u32 field_2000;
  u8 gap_2004[172];
  ActorLists actors;
  u8 gap_2150[128];
  Vec3 field_21D0;
  u8 gap_21DC[0x2394 - 0x21DC];
  std::array<act::ObjElegyStatue*, 5> elegy_statues;
  char field_23A8;
  u8 gap_23A9[3];
  pad::State pad_state_copy;
  u8 gap_2418[16];
  u32 field_2428;
  u8 gap_242C[604];
  u32 field_2688;
  u8 field_268C;
  u8 gap_268D[5999];
  u16 field_3DFC;
  u8 gap_3DFE[16898];
  u32 field_8000;
  u8 gap_8004[28];
  u32 notebook_stuff;
  u8 gap_8024[570];
  u8 hide_hud;
  u8 field_825F;
  u8 gap_8260[160];
  u32 field_8300;
  u8 gap_8304[96];
  OcarinaSong ocarina_song2;
  OcarinaState ocarina_state;
  OcarinaState ocarina_state2;
  OcarinaSong ocarina_song;
  u16 field_836C;
  char field_836E;
  char field_836F;
  int field_8370;
  int field_8374;
  int field_8378;
  int field_837C;
  u16 field_8380;
  u16 field_8382;
  u8 gap_8384[88];
  u32 field_83DC;
  u8 gap_83E0[72];
  HudState hud_state;
  u8 gap_86A8[4];
  u16 field_86AC;
  __attribute__((aligned(4))) u8 gap_86B0[272];
  u32 field_87C0;
  u8 gap_87C4[59];
  u8 field_87FF;
  u8 gap_8800[58];
  u8 field_883A;
  __attribute__((aligned(2))) u8 gap_883C[2];
  u8 field_883E;
  __attribute__((aligned(2))) u8 gap_8840[3064];
  u32 actors_spawn_stuff;
  u8 gap_943C[11204];
  u32 field_C000;
  u8 gap_C004[604];
  u8 room_number;
  char field_C261;
  char field_C262;
  char field_C263;
  u8 gap_C264[88];
  u16 field_C2BC;
  u8 gap_C2BE[355];
  __attribute__((packed)) __attribute__((aligned(1))) u32 field_C421;
  u8 gap_C425[7];
  void (*player_init)(act::Player*, GlobalContext*, int);
  void (*player_update)(act::Player*, GlobalContext*, pad::State*, pad::TouchscreenState*);
  void (*player_switch_to_held_state_0)(GlobalContext*, act::Player*);
  void (*player_use_fishing_rod)(GlobalContext*);
  bool (*player_switch_to_held_state)(GlobalContext*, act::Player*, bool is_gyorg);
  void* player_fn_x4;
  void* player_switch_to_still;
  void* player_fn_x5;
  void* player_talk;
  void* player_fn_x6;
  void* player_fn_x7;
  void* player_fn_x8;
  void* player_fn_x9;
  u8 gap_C460[0x68];
  char field_C4C8[4];
  u16 field_C4CC;
  u8 gap_C4CE[46];
  u32 field_C4FC;
  u8 gap_C500[40];
  u8 field_C528;
  u8 field_C529_one_to_clear_input;
  u8 gap_C52A[4];
  u16 next_entrance;
  u8 field_C530;
  u8 field_C531;
  u8 field_C532;
  u8 gap_C533[5];
  int field_C538;
  u8 gap_C53C[798];
  u8 field_C85A;
  u8 gap_C85B[5];
  void* some_ptr;
  u8 gap_C864[572];
  void (*some_fn1)(void*, u32);
  u32 field_CAA4;
  bool (*some_fn2)(void*);
  u8 gap_CAAC;
  u8 field_CAAD;
  u8 gap_CAAE[2];
  u32 field_CAB0;
  u8 gap_CAB4[16];
  u32 field_CAC4;
  u8 gap_CAC8[480];
  char field_CCA8;
  char field_CCA9;
  char field_CCAA;
  char field_CCAB;

  /// If true, causes black borders (if configured) to be drawn on the top screen.
  /// This is used to implement the letterbox effect when targeting and during some cutscenes.
  bool enable_letterbox;

  /// Opacity of the dimming overlay for the bottom screen.
  /// This is automatically updated every frame depending on flags (see below).
  ///
  /// If this value is > 0.001 and dim_overlay_alpha == dim_heart_overlay_alpha, a semi-transparent
  /// rectangle is drawn over the entire bottom screen. Otherwise, if this value is only > 0.001,
  /// several rectangles are drawn over the screen to dim everything except the heart status widget.
  float dim_overlay_alpha;

  /// Opacity of the dimming overlay for the heart status widget on the bottom screen.
  /// This is automatically updated every frame depending on flags (see below).
  float dim_heart_overlay_alpha;

  /// Non-zero if the heart status widget should be undimmed. Typically set to 211 when non-zero.
  u8 undim_heart_status;
  /// Whether the bottom screen should be undimmed.
  u8 undim_bottom_screen;

  u8 gap_CCBC[4];
  u32 field_CCC0;
  ui::PlayHud* play_hud;
  u8 gap_CCC8[9230];
  u8 field_F0D6;
  u8 gap_F0D7;
  float field_F0D8;
  act::BossTwinmold* twinmold_actor_maybe;
  act::BossTwinmold* twinmold_2_actor_maybe;
  act::Actor* boss_actor_maybe;
  u8 gap_F0E8[4];
  act::Actor* blue_warp_actor;
  act::Actor* some_actor;
  u8 gap_F0F4[7996];
};
static_assert(rst::util::OffsetOf(&GlobalContext::main_camera) == 0x408);
static_assert(rst::util::OffsetOf(&GlobalContext::pause_flags) == 0xAAC);
static_assert(rst::util::OffsetOf(&GlobalContext::elegy_statues) == 0x2394);
static_assert(rst::util::OffsetOf(&GlobalContext::field_C000) == 0xc000);
static_assert(rst::util::OffsetOf(&GlobalContext::ocarina_state) == 0x8366);
static_assert(rst::util::OffsetOf(&GlobalContext::ocarina_song) == 0x836A);
static_assert(rst::util::OffsetOf(&GlobalContext::hide_hud) == 0x825E);
static_assert(rst::util::OffsetOf(&GlobalContext::field_836E) == 0x836E);
static_assert(rst::util::OffsetOf(&GlobalContext::field_C4C8) == 0xC4C8);
static_assert(sizeof(GlobalContext) == 0x11030);

}  // namespace game
