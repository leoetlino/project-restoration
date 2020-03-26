#include <algorithm>
#include <cmath>
#include <string_view>

#include "common/debug.h"
#include "common/utils.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/player.h"
#include "game/ui.h"
#include "game/ui/layouts/item_icon.h"
#include "game/ui/layouts/num_all.h"
#include "game/ui/layouts/play_action_icon.h"
#include "game/ui/screens/main_screen.h"

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

    non_essential_group = playTouchPanel->GetWidget("non_essential_g");
    heart_group = playTouchPanel->GetWidget("heart_g");

    for (auto* group : {non_essential_group, heart_group})
      group->GetPos().SetOpacity(0.0f);
  }

  void Calc(ui::ScreenContext& ctx) override {
    auto* gctx = ctx.GetState<game::GlobalContext>();
    UpdateHudVisibility(gctx);
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

private:
  void UpdateHudVisibility(const game::GlobalContext* gctx) {
    const bool hide_hud = !gctx || gctx->dim_overlay_alpha >= 0.50f ||
                          (gctx->GetPlayerActor() && gctx->GetPlayerActor()->flags1.IsOneSet(
                                                         game::act::Player::Flag1::FreezeLink,
                                                         game::act::Player::Flag1::Locked));
    constexpr float fade_step = 0.125f;
    for (auto* group : {non_essential_group, heart_group}) {
      auto& pos = group->GetPos();
      if (gctx && gctx->undim_heart_status && group == heart_group)
        pos.SetOpacity(std::clamp(pos.color(3) + fade_step, 0.0f, 1.0f));
      else
        pos.SetOpacity(std::clamp(pos.color(3) + (hide_hud ? -1 : 1) * fade_step, 0.0f, 1.0f));
    }
  }

  void FixCounterTextColors() {
    for (auto* num : {rank_a_l, rank_ten_l, rank_hundred_l, num_key_l, num_key_rank_ten_l}) {
      if (num->GetColor() == 2)  // grey
        num->SetColor(0);        // white
    }
  }

  ui::Widget* non_essential_group;
  ui::Widget* heart_group;
};

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

RST_HOOK void rst_MainScreen_UpdateButtonUsability(ui::MainScreen&, void* hud_state) {
  ScopedHookGuard guard;
  if (!guard.AlreadyInHook())
    util::GetPointer<decltype(rst_MainScreen_UpdateButtonUsability)>(0x191570)(s_hud, hud_state);
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
