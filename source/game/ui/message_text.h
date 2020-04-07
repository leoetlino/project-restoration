#pragma once

#include "common/types.h"

namespace game::pad {
struct State;
}

namespace game::ui {

class Widget;

// XXX: Incomplete.
struct MessageTextState {
  int GetChoicePositionY(int choice) const;

  u32 field_0;
  u8 gap_4[36];
  int field_28;
  int field_2C;
  u8 gap_30[28];
  u32 field_4C;
  u8 gap_50[4];
  u32 field_54;
  u8 gap_58[192];
  u8 gap_118[444];
  int first_choice_slot;
  int num_choices;
  u8 gap_2DC[24];
  int field_2F4;
};

void MessageHandleChoice(int* current_choice, const pad::State& pad,
                         const MessageTextState& text_state, Widget& arrow, bool play_sound = true);
void MessageHandleChoice(int* current_choice, int new_choice, const MessageTextState& text_state,
                         Widget& arrow, bool play_sound = true);

}  // namespace game::ui
