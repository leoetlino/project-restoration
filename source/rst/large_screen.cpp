#include <algorithm>
#include <cmath>
#include <string_view>

#include "common/debug.h"
#include "common/utils.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/items.h"
#include "game/player.h"
#include "game/ui.h"
#include "game/ui/layouts/item_icon.h"
#include "game/ui/layouts/num_all.h"
#include "game/ui/layouts/play_action_icon.h"
#include "game/ui/layouts/play_hud.h"
#include "game/ui/screens/main_screen.h"
#include "rst/link.h"

namespace rst {

namespace ui = game::ui;

namespace {

// Second HUD screen on the top screen.
//
// This works by deriving from MainScreen in order to reuse most of MainScreen's logic
// and by having a custom Draw implementation that only draws the HUD layout to the top screen.
class Hud2 : public ui::MainScreen {
public:
  void Init(ui::ScreenContext& ctx) override {
    // Reuse the MainScreen init function, but with our own HUD layout.
    auto* const touch_panel = ui::GetCommonLayouts().touch_panel;
    ui::GetCommonLayouts().touch_panel =
        ui::LayoutMgr::Instance().MakeLayout("Joker.Main.New.Hud2");
    MainScreen::Init(ctx);
    ui::GetCommonLayouts().touch_panel = touch_panel;

    ui::PlayHud* hud = ui::GetCommonLayouts().hud;

    item_btn_group = playTouchPanel->GetWidget("item_button_g");
    action_btn_group = hud->GetWidget("action_button_g");
    non_essential_group = playTouchPanel->GetWidget("non_essential_g");
    heart_group = playTouchPanel->GetWidget("heart_g");
    timer_group = hud->GetWidget("timer_g");

    auto* item_icon_set = playTouchPanel->GetWidget("item_icon_set")->GetLayout();
    icon_1 = item_icon_set->GetWidget("icon_item_1_g");
    icon_2 = item_icon_set->GetWidget("icon_item_2_g");
    icon_x = item_icon_set->GetWidget("icon_item_x_g");
    icon_y = item_icon_set->GetWidget("icon_item_y_g");
    icon_oca = playTouchPanel->GetWidget("icon_item_oca_g");
    icon_a = hud->GetWidget("a_action_button");
    icon_b = hud->GetWidget("b_action_button");

    dpad = item_btn_group->GetWidget("hud_dpad");
    auto* dpad_l = dpad->GetLayout();
    dpad_icon_l = dpad_l->GetWidget("icon_l");
    dpad_icon_r = dpad_l->GetWidget("icon_r");
    dpad_icon_u = dpad_l->GetWidget("icon_u");
    dpad_icon_d = dpad_l->GetWidget("icon_d");

    for (auto* group :
         {item_btn_group, action_btn_group, non_essential_group, heart_group, timer_group})
      group->GetPos().SetOpacity(0.0f);
  }

  void Calc(ui::ScreenContext& ctx) override {
    auto* gctx = ctx.GetState<game::GlobalContext>();
    UpdateHudVisibility(gctx);
    UpdateActionButtons(gctx);
    UpdateDPad(gctx);
    if (gctx) {
      UpdateKeyVisibility(*gctx);
      UpdateCarrot(*gctx);
    }
    UpdateButtons(ctx);
    UpdateRupee();
    UpdateKey(*ctx.ctx);
    UpdateMagic();
    UpdateHeart();

    FixCounterTextColors();

    playTouchPanel->calc();
  }

  void Draw(ui::ScreenContext&) override {
    ui::LayoutDrawMgr::Instance().ControlLayout(playTouchPanel, 2, 10);
  }

  void PrepareClose(ui::ScreenContext&) override {
    transition->Play(playTouchPanel->GetAnim("out_menu_top_anim"));
  }

  void UpdateButtonUsability(game::HudState& hud_state);

private:
  void UpdateHudVisibility(const game::GlobalContext* gctx) {
    bool hide_hud = !gctx || gctx->dim_overlay_alpha >= 0.50f;
    if (auto* player = gctx->GetPlayerActor()) {
      hide_hud |= player->flags1.IsOneSet(game::act::Player::Flag1::FreezeLink,
                                          game::act::Player::Flag1::Locked);
      hide_hud |= player->action_type == game::act::Player::ActionType::Type2;
    }

    constexpr float fade_step = 0.125f;
    for (auto* widget :
         {item_btn_group, action_btn_group, non_essential_group, heart_group, timer_group}) {
      bool should_hide = hide_hud;

      if (widget == heart_group)
        should_hide &= !gctx || !gctx->undim_heart_status;

      if (widget == item_btn_group || widget == action_btn_group)
        should_hide |= !gctx || gctx->hide_hud;

      auto& pos = widget->GetPos();
      pos.AddOpacity((should_hide ? -1 : 1) * fade_step);
    }
  }

  void UpdateActionButtons(const game::GlobalContext* gctx) {
    const bool is_second_level = gctx && gctx->pad_state.input.buttons.IsSet(game::pad::Button::ZR);
    for (auto icon : {icon_1, icon_2})
      icon->GetPos().SetVisible(is_second_level);
    for (auto icon : {icon_x, icon_y})
      icon->GetPos().SetVisible(!is_second_level);

    const auto* player = gctx ? gctx->GetPlayerActor() : nullptr;
    const bool show_oca =
        is_second_level && !(player && player->flags1.IsSet(game::act::Player::Flag1::InWater) &&
                             !player->flags_94.IsSet(game::act::Actor::Flag94::Grounded));
    icon_oca->GetPos().SetVisible(show_oca);
    icon_a->GetPos().SetVisible(!show_oca);
  }

  void UpdateDPad(const game::GlobalContext* gctx) {
    struct DPadButton {
      ui::Widget* widget;
      game::pad::Button btn;
      game::ItemId mask;
      /// Mask to use for the second level mapping (when ZR is held).
      game::ItemId mask2 = game::ItemId::None;
    };
    const DPadButton dpad_buttons[] = {
        {dpad_icon_l, game::pad::Button::Left, game::ItemId::ZoraMask},
        {dpad_icon_u, game::pad::Button::Up, game::ItemId::GoronMask,
         game::HasMask(game::ItemId::FierceDeityMask) ? game::ItemId::FierceDeityMask :
                                                        game::ItemId::None},
        {dpad_icon_d, game::pad::Button::Down, game::ItemId::DekuMask},
    };

    const bool is_second_level = gctx && gctx->pad_state.input.buttons.IsSet(game::pad::Button::ZR);
    const auto update_opacity = [](ui::WidgetPos& pos, bool can_use) {
      pos.SetOpacity(std::clamp(pos.color(3) + (can_use ? 1 : -1) * 0.25f, 0.3f, 1.0f));
    };
    bool fade_out_dpad = true;

    for (const DPadButton& btn : dpad_buttons) {
      const auto mask = (btn.mask2 != game::ItemId::None && is_second_level) ? btn.mask2 : btn.mask;
      bool can_use = game::CanUseItem(mask);
      if (is_second_level && btn.mask2 == game::ItemId::None)
        can_use = false;

      auto* icon = btn.widget->AsLayout<ui::ItemIcon>();
      const auto previous_icon = icon->GetIcon();
      icon->SetCountVisible(false);
      icon->SetIconForItem(int(mask));
      const bool icon_changed = icon->GetIcon() != previous_icon;

      ui::WidgetPos& pos = btn.widget->GetPos();
      pos.SetVisible(game::HasMask(mask));
      if (icon_changed)
        pos.SetOpacity(can_use ? 1.0f : 0.3f);
      else
        update_opacity(pos, can_use);

      fade_out_dpad &= !pos.IsVisible();
    }

    // If the Tatl prompt is visible, show the D-Pad icon and hide the ocarina.
    if (ui::GetCommonLayouts().hud->tatl_state != ui::TatlHudState::Hidden) {
      dpad_icon_r->GetPos().SetOpacity(0.0f);
      fade_out_dpad = false;
    } else if (gctx && game::HasOcarina()) {
      // Otherwise, set the D-Pad right button icon and opacity.
      update_opacity(dpad_icon_r->GetPos(), game::CanUseItem(game::ItemId::Ocarina));
      dpad_icon_r->AsLayout<ui::ItemIcon>()->SetIcon(icon_c_btn02->GetIcon());
      dpad_icon_r->AsLayout<ui::ItemIcon>()->SetCountVisible(false);
      fade_out_dpad = false;
    } else {
      dpad_icon_r->GetPos().AddOpacity(-0.25f);
    }

    dpad->GetPos().AddOpacity((fade_out_dpad ? -1 : 1) * 0.25f);
  }

  void FixCounterTextColors() {
    for (auto* num : {rank_a_l, rank_ten_l, rank_hundred_l, num_key_l, num_key_rank_ten_l}) {
      if (num->GetColor() == 2)  // grey
        num->SetColor(0);        // white
    }
  }

  /// X, Y, I, II, Ocarina
  ui::Widget* item_btn_group;
  /// A, B
  ui::Widget* action_btn_group;
  ui::Widget* non_essential_group;
  ui::Widget* heart_group;
  ui::Widget* timer_group;

  ui::Widget* icon_1;
  ui::Widget* icon_2;
  ui::Widget* icon_x;
  ui::Widget* icon_y;
  ui::Widget* icon_oca;
  ui::Widget* icon_a;
  ui::Widget* icon_b;

  ui::Widget* dpad;
  ui::Widget* dpad_icon_l;
  ui::Widget* dpad_icon_r;
  ui::Widget* dpad_icon_u;
  ui::Widget* dpad_icon_d;
};

void Hud2::UpdateButtonUsability(game::HudState& hud_state) {
  ui::Widget* const btns[] = {icon_y, icon_x, icon_1, icon_2, icon_oca};

  for (size_t i = 0; i < std::size(btns); ++i) {
    if (hud_state.a_btn_opacity >= 0x80 && hud_state.b_btn_opacity >= 0x80) {
      btns[i]->GetPos().SetOpacity(std::max(hud_state.item_btn_opacity[i] / 255.0f, 0.5f));
    } else {
      btns[i]->GetPos().SetOpacity(hud_state.item_btn_opacity[i] / 255.0f);
    }
  }

  for (auto* widget : {camChangePane, camPicturePane, camReturn})
    widget->GetPos().SetOpacity(hud_state.camera_opacity / 255.0f);
}

Hud2 s_hud;

void UpdateBIconForHumanLink(ui::PlayBActionIcon& b_btn, game::GlobalContext* gctx) {
  if (b_btn.text_num != 0xA || b_btn.a_action_icon->GetPos().IsVisible()) {
    // Something is already being shown in the button.
    return;
  }

  // Unfortunately, there is no Fierce Deity Sword icon in MM3D,
  // so we can only show an icon for Human Link.
  auto* player = gctx->GetPlayerActor();
  if (!player || player->active_form != game::act::Player::Form::Human)
    return;

  b_btn.a_action_text->GetPos().SetVisible(false);
  b_btn.a_action_num->GetPos().SetVisible(false);
  b_btn.a_action_icon->GetPos().SetVisible(true);

  auto item_id = game::GetCommonData().save.equipment.data[0].item_btn_b;
  b_btn.a_action_icon_l->SetIconForItem(int(item_id));
  b_btn.a_action_icon_l->SetCountVisible(false);
}

bool s_is_in_main_screen_hook = false;

struct ScopedHookGuard {
  ScopedHookGuard() {
    already_in_hook = s_is_in_main_screen_hook;
    s_is_in_main_screen_hook = true;
  }
  ~ScopedHookGuard() { s_is_in_main_screen_hook = false; }
  bool AlreadyInHook() const { return already_in_hook; }

private:
  bool already_in_hook = false;
};

}  // namespace

RST_HOOK void MainScreen_Init(ui::MainScreen&, ui::ScreenContext& ctx) {
  ScopedHookGuard guard;
  if (!guard.AlreadyInHook())
    s_hud.Init(ctx);
}

RST_HOOK void MainScreen_Open(ui::MainScreen&, ui::ScreenContext& ctx) {
  ScopedHookGuard guard;
  if (!guard.AlreadyInHook())
    s_hud.Open(ctx);
}

RST_HOOK void MainScreen_Calc(ui::MainScreen&, ui::ScreenContext& ctx) {
  ScopedHookGuard guard;
  if (!guard.AlreadyInHook())
    s_hud.Calc(ctx);
}

RST_HOOK void MainScreen_Draw(ui::MainScreen&, ui::ScreenContext& ctx) {
  ScopedHookGuard guard;
  if (!guard.AlreadyInHook())
    s_hud.Draw(ctx);
}

RST_HOOK void MainScreen_PrepareClose(ui::MainScreen&, ui::ScreenContext& ctx) {
  ScopedHookGuard guard;
  if (!guard.AlreadyInHook())
    s_hud.PrepareClose(ctx);
}

RST_HOOK void MainScreen_CalcClose(ui::MainScreen&, ui::ScreenContext& ctx) {
  ScopedHookGuard guard;
  if (!guard.AlreadyInHook())
    s_hud.CalcClose(ctx);
}

RST_HOOK void MainScreen_UpdateButtonUsability(ui::MainScreen&, game::HudState& hud_state) {
  s_hud.UpdateButtonUsability(hud_state);
}

RST_HOOK void MainScreen_MagicPlayAndHideYellowGauge(ui::MainScreen&) {
  s_hud.MagicPlayAndHideYellowGauge();
}

RST_HOOK void MainScreen_MagicHideYellowGauge(ui::MainScreen&) {
  s_hud.MagicHideYellowGauge();
}

RST_HOOK void MainScreen_MagicStartUseAnim(ui::MainScreen&) {
  s_hud.MagicStartUseAnim();
}

RST_HOOK void MainScreen_MagicUpdateCost(ui::MainScreen&) {
  s_hud.MagicUpdateCost();
}

RST_HOOK void BActionIconUpdate(ui::PlayBActionIcon& icon, game::GlobalContext* gctx) {
  icon.Update(gctx);
  UpdateBIconForHumanLink(icon, gctx);
}

RST_HOOK void LoadPackages() {
  auto& project = ui::Project::Instance();
  // Used to store new textures, since texture IDs in all packages must be contiguous...
  project.LoadPackage(project.GetPackageId("Joker.Main.New.gar"));
}

}  // namespace rst
