#include "game/common_data.h"
#include "game/pad.h"

namespace rst {

void UpdatePadState() {
  using namespace game;

  auto& controller_mgr = pad::GetControllerMgr();
  auto& state = controller_mgr.state;
  auto& touchscreen_state = controller_mgr.touchscreen_state;

  const auto set_touch_btn = [&](pad::Button trigger, pad::TouchscreenButton btn,
                                 UsableButton btn_to_check) {
    const bool usable = GetCommonData().usable_btns[u8(btn_to_check)] != ButtonIsUsable::No;
    if (state.input.buttons.TestAndClear(trigger) && usable)
      touchscreen_state.buttons.Set(btn);
    if (state.input.new_buttons.TestAndClear(trigger) && usable)
      touchscreen_state.new_buttons.Set(btn);
  };

  set_touch_btn(pad::Button::ZL, pad::TouchscreenButton::PictographBox,
                UsableButton::PictographBox);

  if (state.input.buttons.IsSet(pad::Button::ZR)) {
    set_touch_btn(pad::Button::A, pad::TouchscreenButton::Ocarina, UsableButton::Ocarina);
    set_touch_btn(pad::Button::X, pad::TouchscreenButton::I, UsableButton::I);
    set_touch_btn(pad::Button::Y, pad::TouchscreenButton::II, UsableButton::II);
  }
}

}  // namespace rst

extern "C" {
RST_HOOK void rst_UpdatePadState() {
  rst::UpdatePadState();
}
}
