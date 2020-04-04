#include <limits>

#include "common/utils.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/player.h"
#include "game/ui.h"
#include "game/ui/layouts/num_all.h"
#include "game/ui/screens/main_screen.h"

namespace game::ui {

// TODO: figure out how to link to the actual vtable properly...

void MainScreen::Init(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x599D20)(this, ctx);
}

void MainScreen::Open(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B4D4)(this, ctx);
}

void MainScreen::Calc(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B6A0)(this, ctx);
}

void MainScreen::Draw(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B27C)(this, ctx);
}

void MainScreen::PrepareClose(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59BC10)(this, ctx);
}

bool MainScreen::CalcClose(ScreenContext& ctx) {
  return rst::util::GetPointer<bool(MainScreen*, ScreenContext&)>(0x59B3CC)(this, ctx);
}

void MainScreen::Close(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59B388)(this, ctx);
}

void MainScreen::UpdateButtons(ScreenContext& ctx) {
  rst::util::GetPointer<void(MainScreen*, ScreenContext&)>(0x59A874)(this, ctx);
}

void MainScreen::UpdateRupee() {
  rst::util::GetPointer<void(MainScreen*)>(0x59AB38)(this);
}

void MainScreen::UpdateHeart() {
  rst::util::GetPointer<void(MainScreen*)>(0x59ACBC)(this);
}

void MainScreen::UpdateMagic() {
  rst::util::GetPointer<void(MainScreen*)>(0x59AED0)(this);
}

void MainScreen::UpdateKey(Context& ctx) {
  rst::util::GetPointer<void(MainScreen*, Context&)>(0x59B040)(this, ctx);
}

void MainScreen::MagicPlayAndHideYellowGauge() {
  if (magic_unlimited)
    return;
  magic_use->Play(magic_use_ani, std::numeric_limits<float>::infinity());
  gaugeYellow_g->GetPos().SetVisible(false);
}

void MainScreen::MagicHideYellowGauge() {
  if (!magic_unlimited)
    gaugeYellow_g->GetPos().SetVisible(false);
}

void MainScreen::MagicStartUseAnim() {
  if (!magic_unlimited && !magic_use->IsPlaying())
    magic_use->Play(magic_use_ani, 0.0, true);
}

void MainScreen::MagicUpdateCost() {
  if (magic_unlimited)
    return;
  const auto& data = game::GetCommonData();
  const float multiplier = data.save.player.magic_num_upgrades == 0 ? 0.5f : 1.0f;
  magic_gauge_position->SetFrame(multiplier * float(data.save.player.magic) / data.magic_max);
  magic_gauge_value->SetFrame(multiplier * float(data.magic_cost) / data.magic_max);
  gaugeYellow_g->GetPos().SetVisible(true);
}

void MainScreen::UpdateKeyVisibility(game::GlobalContext& gctx) {
  float key_change_frame = 0;
  switch (gctx.scene) {
  case SceneId::SnowheadTemple:
  case SceneId::StoneTowerTemple:
  case SceneId::StoneTowerTempleInverted:
  case SceneId::WoodfallTemple:
  case SceneId::GreatBayTemple:
    key_change_frame = 1.0f;
    break;
  case SceneId::SwampSpiderHouse:
  case SceneId::OceansideSpiderHouse:
    key_change_frame = 2.0f;
    break;
  default:
    break;
  }

  const bool visible = key_change_frame != 0.0f;

  if (key_change)
    key_change->SetFrame(key_change_frame - 1);

  if (iconKeyPane)
    iconKeyPane->GetPos().SetVisible(visible);

  if (num_key_l)
    num_key_l->GetRootWidget()->GetPos().SetVisible(visible);

  if (num_key_rank_ten_l)
    num_key_rank_ten_l->GetRootWidget()->GetPos().SetVisible(visible && numSmallKeys >= 10);
}

void MainScreen::UpdateCarrot(game::GlobalContext& gctx) {
  const auto* player = gctx.GetPlayerActor();
  if (!player)
    return;

  carrotStatus->GetPos().SetVisible(player->flags1.IsSet(game::act::Player::Flag1::Riding));

  for (u8 i = num_carrots; i < gctx.hud_state.num_carrots; ++i) {
    carrot_anims[i].player->Stop();
  }

  for (u8 i = gctx.hud_state.num_carrots; i < num_carrots; ++i) {
    carrot_anims[i].player->Play(carrot_anims[i].anim);
  }

  num_carrots = gctx.hud_state.num_carrots;
}

}  // namespace game::ui
