#pragma once

#include <cstddef>

#include "common/types.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/pad.h"

namespace game {

namespace act {
class Player;
}

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
  u8 gap_0[0xe];
  u8 num_actors;
  std::array<ActorList, 12> lists;
};
static_assert(sizeof(ActorLists) == 0xa0);

// Likely incomplete.
// The "global context" is actually a game state, and the start of the structure
// is common to all game states. But I haven't bothered looking at the other ones...
struct GlobalContext {
  bool IsUiMenuActive() const {
    return ui_menu_state != game::UiMenuState::Closed;
  }

  act::Actor* FindActorWithId(act::Id id, act::Type type) const;
  template <typename T>
  T* FindActorWithId(act::Id id, act::Type type) const {
    return static_cast<T*>(FindActorWithId(id, type));
  }

  act::Player* GetPlayerActor() const;

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
  /// Nnumber of frames since the game state was initialised.
  u32 frame_counter;
  u8 field_13C;
  GameStateType type;
  u16 field_13E;
  u8 gap_140[212];
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
  u8 gap_2418[624];
  u32 field_2688;
  u8 gap_268C[22932];
  u32 field_8020;
  u8 gap_8024[570];
  u16 field_825E;
  u8 gap_8260[226];
  u16 field_8342;
  u8 gap_8344[34];
  u16 field_8366;
  u8 gap_8368[192];
  u32 field_8428;
  u8 gap_842C[468];
  u32 field_8600;
  u8 gap_8604[68];
  u16 field_8648;
  u8 gap_864A[12];
  u16 field_8656;
  u8 gap_8658[52];
  char field_868C;
  __attribute__((aligned(4))) u8 gap_8690[20];
  bool field_86A4_involved_in_form_check;
  __attribute__((aligned(4))) u8 gap_86A8[4];
  u16 field_86AC;
  __attribute__((aligned(4))) u8 gap_86B0[272];
  u32 field_87C0;
  u8 gap_87C4[3188];
  u32 field_9438;
  u8 gap_943C[11204];
  u32 field_C000;
  u8 gap_C004[604];
  char field_C260;
  char field_C261;
  char field_C262;
  char field_C263;
  u8 gap_C264[445];
  __attribute__((packed)) __attribute__((aligned(1))) u32 field_C421;
  u8 gap_C425[163];
  char field_C4C8[4];
  u16 field_C4CC;
  u8 gap_C4CE[91];
  u8 field_C529;
  u8 gap_C52A[8];
  char field_C532;
  u8 gap_C533[5];
  int field_C538;
};
static_assert(rst::util::OffsetOf(&GlobalContext::field_C000) == 0xc000);

}  // namespace game
