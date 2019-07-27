#pragma once

#include <cstddef>

#include "common/types.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/pad.h"

namespace game {

namespace act {
class Player;
class BossTwinmold;
}  // namespace act

// based on a quick experiment - probably wrong
enum class UiMenuState : u16 {
  Closed = 0,
  Opening = 1,
  Opened = 3,
};

enum class GameStateType : u8 {
  /// Initial game state
  Initial = 0,
  /// Second game state after the initial game state
  /// Responsible for allocating a 0x2aa0 byte structure.
  FirstGame = 1,
  /// Remnant of N64 "ovl_title" state?
  Dummy = 2,
  /// Sets several variables before changing to state 5.
  PrepareTitleScreen = 3,
  /// Main game. An instance of this type is unofficially called the "global context".
  Play = 4,
  /// Initialises player data (notably save data).
  /// Responsible for setting time to 0x5555.
  InitPlayer = 5,
  /// File Select
  FileSelect = 6,
  /// 72/48/24 Hours Remaining / Dawn of a New Day
  DayTelop = 7,
  /// Majora's Mask 3D video hints
  JokerHintMovie = 8,
  /// Majora's Mask 3D credits (including THE END and save prompt)
  JokerEnding = 9,
};

// Keeps track of spawned actors.
struct ActorList {
  u32 num_actors;
  act::Actor* first;
  u32 unknown;
};
static_assert(sizeof(ActorList) == 0xc);

struct ActorLists {
  ActorList& GetList(act::Type type) { return lists[u8(type)]; }
  const ActorList& GetList(act::Type type) const { return lists[u8(type)]; }
  u8 gap_0[0xe];
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
};

// Likely incomplete.
// The "global context" is actually a game state, and the start of the structure
// is common to all game states. But I haven't bothered looking at the other ones...
struct GlobalContext {
  bool IsUiMenuActive() const { return ui_menu_state != game::UiMenuState::Closed; }

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
                         float pos_x, float pos_y, float pos_z);

  void ShowMessage(u16 msgid, int unknown = 0);

  int field_0;
  u8 gap_4[36];
  pad::State pad_state;
  u8 gap_94[108];
  u32 field_100;
  int (*calc_fn)(GlobalContext*);
  int (*exit_fn)(GlobalContext*);
  void (*next_game_state_init_fn)(GlobalContext*);
  u32 field_110;
  u32 field_114;
  u8 gap_118[12];
  u32 field_124;
  u8 gap_128[16];
  /// Number of frames since the game state was initialised.
  u32 frame_counter;
  u8 field_13C;
  GameStateType type;
  u16 field_13E;
  u32 field_140;
  u32 field_144;
  u16 map_maybe;
  u16 field_14A;
  u8 gap_14C[200];
  float field_214;
  u8 gap_218[2144];
  s16 field_A78;
  __attribute__((aligned(4))) u8 gap_A7C[48];
  UiMenuState ui_menu_state;
  u32 field_AB0;
  u8 gap_AB4[76];
  int field_B00;
  u8 gap_B04[5372];
  u32 field_2000;
  u8 gap_2004[172];
  ActorLists actors;
  u8 gap_2150[600];
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
  u16 field_825E;
  u8 gap_8260[160];
  u32 field_8300;
  u8 gap_8304[98];
  OcarinaState ocarina_state;
  u16 field_8368;
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
  u32 field_8428;
  u8 gap_842C[468];
  u32 field_8600;
  u8 gap_8604[68];
  u16 field_8648;
  u8 gap_864A[12];
  u16 field_8656;
  u8 gap_8658[16];
  u16 field_8668;
  u16 field_866A;
  u16 field_866C;
  u8 gap_866E[30];
  char field_868C;
  __attribute__((aligned(4))) u8 gap_8690[20];
  bool field_86A4_involved_in_form_check;
  __attribute__((aligned(4))) u8 gap_86A8[4];
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
  u8 gap_C425[163];
  char field_C4C8[4];
  u16 field_C4CC;
  u8 gap_C4CE[46];
  u32 field_C4FC;
  u8 gap_C500[40];
  u8 field_C528;
  u8 field_C529_one_to_clear_input;
  u8 gap_C52A[7];
  bool field_C531;
  char field_C532;
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
  bool field_CCAC;
  u32 field_CCB0;
  u32 field_CCB4;
  u8 gap_CCB8[8];
  u8 field_CCC0;
  u8 field_CCC1;
  u8 gap_CCC2[9236];
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
static_assert(rst::util::OffsetOf(&GlobalContext::field_C000) == 0xc000);
static_assert(rst::util::OffsetOf(&GlobalContext::ocarina_state) == 0x8366);
static_assert(rst::util::OffsetOf(&GlobalContext::ocarina_song) == 0x836A);
static_assert(rst::util::OffsetOf(&GlobalContext::field_836E) == 0x836E);
static_assert(sizeof(GlobalContext) == 0x11030);

enum class UiScreen {
  Black,
  Main,
  Schedule,
  Quest,
  Items,
  Map,
  Masks,
  /// "Play your instrument" screen
  Ocarina,
};
bool OpenUiScreen(UiScreen screen);

bool CheckCurrentUiScreen(UiScreen screen);

}  // namespace game
