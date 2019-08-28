#pragma once

#include "common/types.h"
#include "game/pad.h"

namespace game {

enum class StateType : u8 {
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

struct State {
  int field_0;
  u8 gap_4[36];
  pad::State pad_state;
  u8 gap_94[108];
  u32 field_100;
  void (*calc_fn)(State*);
  void (*exit_fn)(State*);
  void (*next_state_init_fn)(State*);
  u32 field_110;
  u32 field_114;
  u8 gap_118[12];
  u32 field_124;
  u8 gap_128[16];
  /// Number of frames since the game state was initialised.
  u32 frame_counter;
  u8 field_13C;
  StateType type;
  u16 field_13E;
  u32 field_140;
  u32 field_144;
};
static_assert(sizeof(State) == 0x148);

}
