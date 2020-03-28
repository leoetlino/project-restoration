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
  /// Sets several variables before changing to state 5 (PrepareTitleScreen2).
  PrepareTitleScreen1 = 3,
  /// Main game. An instance of this type is unofficially called the "global context".
  Play = 4,
  /// Initialises common data for the title screen cutscene.
  /// Responsible for setting time to 0x5555.
  PrepareTitleScreen2 = 5,
  /// File Select
  FileSelect = 6,
  /// 72/48/24 Hours Remaining / Dawn of a New Day
  DayTelop = 7,
  /// Majora's Mask 3D video hints
  JokerHintMovie = 8,
  /// Majora's Mask 3D credits (including THE END and save prompt)
  JokerEnding = 9,

  /// Extension: Save prompt for the Song of Time before telop.
  SotSave = 10,
};

struct State {
  enum class Status : u8 {
    /// Triggers a game state change. Make sure next_state_init_fn is set to a valid function
    /// before setting status to Changing.
    Changing = 0,
    Initialising = 1,
    Running = 2,
  };

  // "gamealloc" structures
  struct SimpleHeap {
    void* field_0;
    void* field_4;
    void* field_8;
    void* field_C;
    void* field_10;
  };
  struct SimpleAllocator {
    void* Alloc(u32 size) {
      bottom = (void*)(((uintptr_t(bottom) & ~0xF) - size) & ~0xF);
      return bottom;
    }

    u32 size;
    void* buffer;
    void* top;
    void* bottom;
  };

  void ChangeState(StateType new_type);

  int field_0;
  u8 gap_4[36];
  pad::State pad_state;
  u8 gap_94[108];
  u32 field_100;
  void (*calc_fn)(State*);
  void (*exit_fn)(State*);
  void (*next_state_init_fn)(State*);
  u32 next_state_instance_size;
  SimpleAllocator allocator;
  SimpleHeap heap;
  /// Number of frames since the game state was initialised.
  u32 frame_counter;
  Status status;
  StateType type;
  u16 field_13E;
  u32 field_140;
  u32 field_144;
};
static_assert(sizeof(State) == 0x148);

template <typename T, typename Base = State>
struct StateSimple : Base {
  StateSimple() {
    State::calc_fn = [](auto* state) { static_cast<T*>(state)->Calc(); };
    State::exit_fn = [](auto* state) { static_cast<T*>(state)->~T(); };
    State::status = State::Status::Running;
  }
};

struct StateInfo {
  u32 unused_0;
  void (*init_fn)(State*);
  void (*exit_fn)(State*);
  u32 unused_1, unused_2, unused_3;
  u32 instance_size;
  StateType type;
};
static_assert(sizeof(StateInfo) == 0x20);

const StateInfo* FindStateInfoByInit(void (*init_fn)(State*));
const StateInfo* FindStateInfoByType(StateType type);

class FileEntity;

struct SaveFile {
  void Open();
  bool WriteAndFree();

  FileEntity* file;
  u16 field_4;
  u16 field_6;
  void* data;
  u16 field_C;
  u16 field_E;
  u16 status;
};
static_assert(sizeof(SaveFile) == 0x14);

}  // namespace game
