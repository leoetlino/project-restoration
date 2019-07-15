#include "rst/link.h"

#include <algorithm>

#include "common/context.h"
#include "common/debug.h"
#include "common/utils.h"
#include "game/context.h"
#include "game/items.h"
#include "game/pad.h"
#include "game/player.h"

namespace rst::link {

void Init() {
  // This reverts some of the MM3D changes to form-specific parameters.

  // Fix Deku Link's walk acceleration value
  auto& deku_param = game::act::GetFormParam(game::act::FormParamIndex::Deku);
  deku_param.run_accel = 200;

  // Make Giant Link less painfully slow
  auto& giant_param = game::act::GetFormParam(game::act::FormParamIndex::Giant);
  giant_param.run_accel = 100;
  giant_param.walk_speed = 350;
}

namespace {
struct TransformAction {
  game::pad::Button trigger_btn;
  game::ItemId required_mask;
  game::Action action;
  const char* name;
};
static constexpr TransformAction s_actions[] = {
    {game::pad::Button::Left, game::ItemId::ZoraMask, game::Action::ZoraMask, "Zora"},
    {game::pad::Button::Up, game::ItemId::GoronMask, game::Action::GoronMask, "Goron"},
    {game::pad::Button::Down, game::ItemId::DekuMask, game::Action::DekuMask, "Deku"},
};

bool CanUseFastAction(game::act::Player* player) {
  if (GetContext().gctx->IsUiMenuActive()) {
    util::Print("%s: player is in a menu, skipping", __func__);
    return false;
  }

  if (player->flags1.IsSet(game::act::Player::Flag1::FreezeLink)) {
    util::Print("%s: Flag1::FreezeLink is set, skipping", __func__);
    return false;
  }

  if (player->active_mask_id == game::MaskId::GiantMask) {
    util::Print("%s: wearing Giant's Mask, skipping", __func__);
    return false;
  }

  return true;
}
}  // namespace

void HandleFastTransform() {
  game::GlobalContext* gctx = GetContext().gctx;

  game::act::Player* player = gctx->GetPlayerActor();
  if (!player)
    return;

  const auto it =
      std::find_if(std::begin(s_actions), std::end(s_actions), [&](const TransformAction& action) {
        return gctx->pad_state.input.new_buttons.IsSet(action.trigger_btn);
      });
  if (it == std::end(s_actions))
    return;

  if (!CanUseFastAction(player))
    return;

  if (!game::HasMask(it->required_mask)) {
    util::Print("%s: player does not have the %s Mask, skipping", __func__, it->name);
    return;
  }

  if (!game::CanUseItem(it->required_mask)) {
    util::Print("%s: CanUseItem returned false, skipping", __func__);
    return;
  }

  util::Print("%s: transforming (%s)", __func__, it->name);

  player->action = it->action;
  player->action_type = game::act::Player::ActionType::OcarinaOrTransformation;
}

void HandleFastOcarina() {
  game::GlobalContext* gctx = GetContext().gctx;
  game::act::Player* player = gctx->GetPlayerActor();
  if (!player)
    return;

  if (!gctx->pad_state.input.new_buttons.IsSet(game::pad::Button::ZR))
    return;

  if (!CanUseFastAction(player))
    return;

  if (!game::HasOcarina())
    return;

  if (!game::CanUseItem(game::ItemId::Ocarina)) {
    util::Print("%s: CanUseItem returned false, skipping", __func__);
    return;
  }

  player->action = game::Action::Ocarina;
  player->action_type = game::act::Player::ActionType::OcarinaOrTransformation;
}

bool ShouldUseZoraFastSwim() {
  const auto& input = GetContext().gctx->pad_state.input;

  if (!input.buttons.IsSet(game::pad::Button::A))
    return false;

  // Toggle fast swim with D-Pad Up/Down
  if (input.new_buttons.IsOneSet(game::pad::Button::Up, game::pad::Button::Down))
    GetContext().use_fast_swim ^= true;

  // Overrides
  if (input.buttons.IsSet(game::pad::Button::R))
    return true;
  if (input.buttons.IsSet(game::pad::Button::ZL))
    return false;

  return GetContext().use_fast_swim;
}

}  // namespace rst::link

extern "C" {
RST_HOOK bool rst_link_ShouldUseZoraFastSwim() {
  return rst::link::ShouldUseZoraFastSwim();
}
}
