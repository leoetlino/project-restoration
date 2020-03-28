#include "game/states/state_sot_save.h"

#include "common/context.h"
#include "common/utils.h"
#include "game/message.h"
#include "game/sound.h"
#include "game/ui.h"
#include "game/ui/message_text.h"
#include "game/ui/layouts/message_window.h"

namespace game {

StateSotSave::StateSotSave() {
  const auto actual_calc_fn = calc_fn;
  StateEnding::Init();
  calc_fn = actual_calc_fn;

  if (!rst::GetContext().going_back_in_time) {
    ChangeState(StateType::DayTelop);
    return;
  }
  rst::GetContext().going_back_in_time = false;
}

StateSotSave::~StateSotSave() = default;

void StateSotSave::Calc() {
  switch (sotsave_status) {
  case Status::Initialising:
    sound::ControlEnv(18);
    message_window =
        ui::LayoutMgr::Instance().MakeLayout<ui::MessageWindow>("Joker.Main.MessageWindow");
    ending_status = 4;
    sotsave_status = Status::WaitingForPackageLoad;
    break;
  case Status::WaitingForPackageLoad:
    if (ui::Project::Instance().IsLoading())
      return;
    layout = ui::LayoutMgr::Instance().MakeLayout("Joker.Main.Ending.Ending");
    press_start_layout = ui::LayoutMgr::Instance().MakeLayout("Joker.Main.Ending.EndingPressStart");
    sotsave_status = Status::Running;
    break;
  case Status::Running: {
    // Show "do you want to save" and default to "do not save", which is the non-destructive option.
    if (ending_status == 4) {
      ShowMessage(0x575E);
      timer = 0.3f;
      ui::MessageHandleChoice(&message_choice, 1, *text_state, *branch_arrow, false);
      ++ending_status;
    }

    StateEnding::Calc();

    // Show the regular "Your progress has been saved." message,
    // rather than the credits save message
    if (ending_status == 0xA) {
      ShowMessage(0xE9B);
    }

    // Switch to the telop state after the message window has disappeared.
    if (ending_status == 0xC) {
      ChangeState(StateType::DayTelop);
    }

    break;
  }
  }
}

}  // namespace game
