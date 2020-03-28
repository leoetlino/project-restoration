#pragma once

#include "game/states/state.h"
#include "game/states/state_ending.h"

namespace game {

namespace ui {
class Layout;
class MessageWindow;
}  // namespace ui

// Extension: Reuses the ending game state to show a save prompt after going back in time.
struct StateSotSave : StateSimple<StateSotSave, StateEnding> {
  static constexpr auto Type = StateType::SotSave;
  StateSotSave();
  ~StateSotSave();
  void Calc();

  enum class Status {
    Initialising,
    WaitingForPackageLoad,
    Running,
  };
  Status sotsave_status = Status::Initialising;
};

}  // namespace game
