#pragma once

#include "common/utils.h"
#include "game/states/state.h"

namespace game {

namespace ui {
class Layout;
class MessageTextState;
class MessageWindow;
class Widget;
}  // namespace ui

struct StateEnding : State {
  static constexpr auto Type = StateType::JokerEnding;
  void Init() { rst::util::GetPointer<void(StateEnding&)>(0x15D20C)(*this); }
  void Calc() { rst::util::GetPointer<void(StateEnding&)>(0x16EC90)(*this); }
  void ShowMessage(int id) { rst::util::GetPointer<void(StateEnding&, int)>(0x18CD10)(*this, id); }

  SaveFile save_file;
  u8 gap_15C[0x10];
  ui::Layout* layout;
  ui::Layout* press_start_layout;
  ui::MessageWindow* message_window;
  ui::MessageTextState* text_state;
  ui::Widget* branch_arrow;
  int ending_status;
  int message_choice;
  float timer;
  float time_step;
};
static_assert(sizeof(StateEnding) == 0x190);

}  // namespace game
