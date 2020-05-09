#include "common/context.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/pad.h"
#include "game/player.h"
#include "game/ui.h"

namespace rst {

/// Modified touchscreen state, ignoring item usability checks.
static game::pad::TouchscreenState s_touchscreen_state;

RST_HOOK void UpdatePadState() {
  using namespace game;

  auto& controller_mgr = pad::GetControllerMgr();
  auto& state = controller_mgr.state;
  s_touchscreen_state = controller_mgr.touchscreen_state;

  const auto set_touch_btn = [&](pad::Button trigger, pad::TouchscreenButton btn) {
    if (state.input.buttons.TestAndClear(trigger))
      s_touchscreen_state.buttons.Set(btn);
    if (state.input.new_buttons.TestAndClear(trigger))
      s_touchscreen_state.new_buttons.Set(btn);
  };
  const auto set_touch_btn_without_clear = [&](pad::Button trigger, pad::TouchscreenButton btn) {
    if (state.input.buttons.IsSet(trigger))
      s_touchscreen_state.buttons.Set(btn);
    if (state.input.new_buttons.IsSet(trigger))
      s_touchscreen_state.new_buttons.Set(btn);
  };

  set_touch_btn_without_clear(pad::Button::ZL, pad::TouchscreenButton::PictographBox);

  if (state.input.buttons.IsSet(pad::Button::ZR)) {
    // XXX: This shouldn't be here...
    auto* gctx = rst::GetContext().gctx;
    auto* player = gctx ? gctx->GetPlayerActor() : nullptr;
    if (player && player->flags1.IsSet(game::act::Player::Flag1::InWater) &&
        !player->flags_94.IsSet(game::act::Actor::Flag94::Grounded)) {
      // If Link is swimming (as Zora Link most likely but that doesn't matter),
      // do not unset the A button.
      set_touch_btn_without_clear(pad::Button::A, pad::TouchscreenButton::Ocarina);
    } else {
      set_touch_btn(pad::Button::A, pad::TouchscreenButton::Ocarina);
    }

    set_touch_btn(pad::Button::X, pad::TouchscreenButton::I);
    set_touch_btn(pad::Button::Y, pad::TouchscreenButton::II);
  }

  controller_mgr.touchscreen_state = s_touchscreen_state;

  // Unset unusable buttons.
  const auto unset_if_not_usable = [&](pad::TouchscreenButton btn, UsableButton btn_to_check) {
    const bool usable = GetCommonData().usable_btns[u8(btn_to_check)] != ButtonIsUsable::No;
    if (!usable) {
      controller_mgr.touchscreen_state.buttons.Clear(btn);
      controller_mgr.touchscreen_state.new_buttons.Clear(btn);
    }
  };
  unset_if_not_usable(pad::TouchscreenButton::I, UsableButton::I);
  unset_if_not_usable(pad::TouchscreenButton::II, UsableButton::II);
  unset_if_not_usable(pad::TouchscreenButton::Ocarina, UsableButton::Ocarina);
  unset_if_not_usable(pad::TouchscreenButton::PictographBox, UsableButton::PictographBox);
}

RST_HOOK void UpdatePadStateForOcarina() {
  using namespace game;
  auto& controller_mgr = pad::GetControllerMgr();
  auto& state = controller_mgr.state;

  // Merge ZL and L, ZR and R for the Ocarina screen
  if (!game::ui::CheckCurrentScreen(game::ui::ScreenType::Ocarina))
    return;

  auto map = [&state](pad::Button source, pad::Button target) {
    if (state.input.buttons.IsSet(source))
      state.input.buttons.Set(target);
    if (state.input.new_buttons.IsSet(source))
      state.input.new_buttons.Set(target);
    if (state.input.released_buttons.IsSet(source))
      state.input.released_buttons.Set(target);
  };
  map(pad::Button::ZL, pad::Button::L);
  map(pad::Button::ZR, pad::Button::R);
}

namespace ui::items {

bool IsItemAssignRequested() {
  using namespace game::pad;
  const auto& mgr = GetControllerMgr();
  return mgr.state.input.new_buttons.IsOneSet(Button::X, Button::Y) ||
         s_touchscreen_state.new_buttons.IsOneSet(TouchscreenButton::I, TouchscreenButton::II);
}

int GetItemAssignIndex() {
  using namespace game::pad;
  const auto& mgr = GetControllerMgr();
  if (mgr.state.input.new_buttons.IsSet(Button::X))
    return 1;
  if (mgr.state.input.new_buttons.IsSet(Button::Y))
    return 2;
  if (s_touchscreen_state.new_buttons.IsSet(TouchscreenButton::I))
    return 0;
  if (s_touchscreen_state.new_buttons.IsSet(TouchscreenButton::II))
    return 3;
  return 1;  // should be unreachable
}

}  // namespace ui::items

}  // namespace rst

extern "C" {
RST_HOOK bool rst_ui_items_IsItemAssignRequested() {
  return rst::ui::items::IsItemAssignRequested();
}
RST_HOOK int rst_ui_items_GetItemAssignIndex() {
  return rst::ui::items::GetItemAssignIndex();
}
}
