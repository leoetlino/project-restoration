#include "game/ui/message_text.h"

#include <algorithm>

#include "common/utils.h"
#include "game/pad.h"
#include "game/sound.h"
#include "game/ui.h"

namespace game::ui {

int MessageTextState::GetChoicePositionY(int choice) const {
  return rst::util::GetPointer<int(const MessageTextState&, int)>(0x1C6AD4)(*this, choice);
}

void MessageHandleChoice(int* current_choice, const pad::State& pad,
                         const MessageTextState& text_state, Widget& arrow, bool play_sound) {
  int new_choice = *current_choice;
  if (pad.input.new_buttons.IsOneSet(pad::Button::Up, pad::Button::MainStickUp))
    new_choice -= 1;
  else if (pad.input.new_buttons.IsOneSet(pad::Button::Down, pad::Button::MainStickDown))
    new_choice += 1;

  MessageHandleChoice(current_choice, new_choice, text_state, arrow, play_sound);
}

void MessageHandleChoice(int* current_choice, int new_choice, const MessageTextState& text_state,
                         Widget& arrow, bool play_sound) {
  if (text_state.num_choices <= 0)
    return;

  new_choice = std::clamp(new_choice, 0, text_state.num_choices - 1);
  if (*current_choice == new_choice)
    return;

  if (play_sound)
    sound::PlayEffect(sound::EffectId::NA_SE_SY_CURSOR);

  arrow.GetPos().SetTranslateY(
      text_state.GetChoicePositionY(text_state.first_choice_slot + new_choice) + 8);
  *current_choice = new_choice;
}

}  // namespace game::ui
