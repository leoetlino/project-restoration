#include "rst/link.h"

#include <algorithm>

#include "common/context.h"
#include "common/debug.h"
#include "common/utils.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/items.h"
#include "game/pad.h"
#include "game/player.h"
#include "game/ui.h"
#include "game/ui/layouts/item_icon.h"
#include "game/ui/layouts/play_hud.h"
#include "game/ui/screens/main_screen.h"

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
  bool usable_in_water;
  const char* name;
  bool require_zr = false;
};
static constexpr TransformAction s_actions[] = {
    {game::pad::Button::Left, game::ItemId::ZoraMask, game::Action::ZoraMask, true, "Zora"},
    {game::pad::Button::Up, game::ItemId::GoronMask, game::Action::GoronMask, false, "Goron"},
    {game::pad::Button::Down, game::ItemId::DekuMask, game::Action::DekuMask, false, "Deku"},
    // Second-level mappings (make sure require_zr=true!)
    {game::pad::Button::Up, game::ItemId::FierceDeityMask, game::Action::FierceDeityMask, false,
     "Fierce Deity", true},
};
}  // namespace

void HandleFastTransform() {
  const game::GlobalContext* gctx = GetContext().gctx;

  game::act::Player* player = gctx->GetPlayerActor();
  if (!player)
    return;

  const bool in_water = player->flags1.IsSet(game::act::Player::Flag1::InWater);
  const auto it =
      std::find_if(std::begin(s_actions), std::end(s_actions), [&](const TransformAction& action) {
        return player->controller_info.state->input.buttons.IsSet(game::pad::Button::ZR) ==
                   action.require_zr &&
               player->controller_info.state->input.new_buttons.IsSet(action.trigger_btn) &&
               (!in_water || action.usable_in_water);
      });
  if (it == std::end(s_actions))
    return;

  if (player->active_mask_id == game::MaskId::GiantMask) {
    util::Print("%s: wearing Giant's Mask, skipping", __func__);
    return;
  }

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
  // Store the transform action in case the transformation cannot be done immediately.
  // This allows the Mask Storage technique to work with the fast transform shortcuts as well.
  player->transform_mask_action = it->action;
}

bool ShouldUseZoraFastSwim() {
  const auto& input = GetContext().gctx->pad_state.input;

  if (!input.buttons.IsSet(game::pad::Button::A))
    return false;

  // Toggle fast swim with D-Pad Up/Down or ZL
  if (input.new_buttons.IsOneSet(game::pad::Button::Up, game::pad::Button::Down,
                                 game::pad::Button::ZL)) {
    GetContext().use_fast_swim ^= true;
  }

  // Overrides
  if (input.buttons.IsSet(game::pad::Button::R))
    return true;
  if (input.buttons.IsSet(game::pad::Button::ZR))
    return false;

  return GetContext().use_fast_swim;
}

bool SwitchToZoraFastSwim(game::GlobalContext* gctx, game::act::Player* player, bool check_magic) {
  const auto do_switch = util::GetPointer<decltype(SwitchToZoraFastSwim)>(0x220EA0);
  return (player->zora_swim_a_press_duration >= 7 ||
          player->flags1.IsSet(game::act::Player::Flag1::IsUsingZoraBarrier)) &&
         do_switch(gctx, player, check_magic);
}

bool ShouldEndGoronRoll(game::act::Player* player) {
  return player->controller_info.state->input.new_buttons.IsSet(game::pad::Button::A) ||
         (player->controller_info.state->input.released_buttons.IsSet(game::pad::Button::A) &&
          GetContext().a_press_duration >= 15);
}

struct FastArrowState {
  std::optional<game::Action> override_action;
  int magic_cost_update_timer = -1;
};

static FastArrowState s_fast_arrow_state{};

struct ArrowTypeInfo {
  const char* name;
  int magic_cost;
  game::ItemId required_item;
};

static constexpr ArrowTypeInfo s_arrow_types[] = {
    {"Arrow", 0, game::ItemId::Arrow},
    {"Fire Arrow", 4, game::ItemId::FireArrow},
    {"Ice Arrow", 4, game::ItemId::IceArrow},
    {"Light Arrow", 8, game::ItemId::LightArrow},
};

static void SpawnArrowActor(game::GlobalContext* gctx, game::act::Player* player) {
  game::CommonData& cdata = game::GetCommonData();

  const auto info = player->GetArrowInfo(gctx);
  if (!info.can_use) {
    util::Print("%s: cannot use arrow", __func__);
    return;
  }

  u16 param = info.actor_param;
  int type = int(param) - 2;
  if (type < 0 || size_t(type) > std::size(s_arrow_types))
    return;

  if (cdata.save.player.magic < s_arrow_types[type].magic_cost) {
    util::Print("%s: not enough magic for %s (%d < %d) -- falling back to normal arrow", __func__,
                s_arrow_types[type].name, cdata.save.player.magic, s_arrow_types[type].magic_cost);
    type = 0;
    param = 2;
  }

  util::Print("%s: spawning %s (param=%u)", __func__, s_arrow_types[type].name, param);

  auto* arrow =
      gctx->SpawnActor(player, game::act::Id::Arrow, 0, player->angle, 0, param, player->pos.pos);
  player->projectile_actor = arrow;
  cdata.magic_cost = 0;
  // For some reason, updating the magic cost immediately doesn't work,
  // so delay the update by 2 frames.
  if (type != 0)
    s_fast_arrow_state.magic_cost_update_timer = 2;
}

void HandleFastArrowSwitch(game::act::Player* player) {
  game::CommonData& cdata = game::GetCommonData();
  game::GlobalContext* gctx = GetContext().gctx;

  // Reset the override action if the player is not using a bow.
  constexpr u8 first = u8(game::Action::Arrow);
  constexpr u8 last = u8(game::Action::LightArrow);
  const bool is_using = player->action_type == game::act::Player::ActionType::Type3 ||
                        player->projectile_actor ||
                        player->flags1.IsSet(game::act::Player::Flag1::ZTargeting);
  if (first > u8(player->current_action) || u8(player->current_action) > last || !is_using) {
    s_fast_arrow_state = {};
    return;
  }

  if (s_fast_arrow_state.magic_cost_update_timer > 0)
    --s_fast_arrow_state.magic_cost_update_timer;

  if (s_fast_arrow_state.override_action && s_fast_arrow_state.magic_cost_update_timer == 0) {
    const u8 type = u8(*s_fast_arrow_state.override_action) - first;
    game::act::PlayerUpdateMagicCost(gctx, s_arrow_types[type].magic_cost, 0,
                                     game::act::AllowExistingMagicUsage::Yes);
    s_fast_arrow_state.magic_cost_update_timer = -1;
  }

  if (!s_fast_arrow_state.override_action) {
    s_fast_arrow_state.override_action = player->current_action;
    util::Print("%s: override_action is now %u", __func__, u8(*s_fast_arrow_state.override_action));
  }

  if (gctx->pad_state.input.new_buttons.IsSet(game::pad::Button::ZL)) {
    int idx = u8(*s_fast_arrow_state.override_action) - first;

    auto can_use_arrow = [&cdata](int idx) {
      return game::HasItem(s_arrow_types[idx].required_item) &&
             cdata.save.player.magic >= s_arrow_types[idx].magic_cost &&
             game::CanUseItem(s_arrow_types[idx].required_item);
    };

    // Ensure we don't enter an infinite loop if no other arrow type can be used.
    if (!can_use_arrow(idx))
      return;

    do {
      idx = (idx + 1) % std::size(s_arrow_types);
    } while (!can_use_arrow(idx));

    s_fast_arrow_state.override_action = static_cast<game::Action>(idx + first);
    player->current_action = player->action = *s_fast_arrow_state.override_action;
    cdata.save.equipment.data[0].item_btns[player->item_btn_slot] =
        s_arrow_types[idx].required_item;
    util::Print("%s: override_action is now %u (%s)", __func__,
                u8(*s_fast_arrow_state.override_action), s_arrow_types[idx].name);

    if (player->projectile_actor) {
      player->projectile_actor->Free();
      player->projectile_actor = nullptr;
      SpawnArrowActor(gctx, player);
    }
  }
}

static void HandleFastOcarina(game::GlobalContext* gctx) {
  // If the Tatl prompt is visible, do not enable the ocarina D-Pad button.
  if (game::ui::GetCommonLayouts().hud->tatl_state != game::ui::TatlHudState::Hidden)
    return;

  if (!game::HasOcarina())
    return;

  if (auto* player = gctx->GetPlayerActor();
      player && gctx->hud_state.item_btn_opacity[4] == 0xFF &&
      player->controller_info.state->input.new_buttons.IsSet(game::pad::Button::Right)) {
    player->action_type = game::act::Player::ActionType::OcarinaOrTransformation;
    player->action = game::Action::Ocarina;
  }
}

void Calc() {
  game::GlobalContext* gctx = GetContext().gctx;
  game::act::Player* player = gctx->GetPlayerActor();
  if (!player)
    return;
  if (gctx->IsPaused())
    return;

  if (player->controller_info.state) {
    ++GetContext().a_press_duration;
    if (!player->controller_info.state->input.buttons.IsSet(game::pad::Button::A))
      GetContext().a_press_duration = 0;
  }

  HandleFastArrowSwitch(player);
  HandleFastOcarina(gctx);
}

std::optional<game::Action> GetFastArrowAction() {
  return s_fast_arrow_state.override_action;
}

RST_HOOK void TriggerItemUseHook() {
  HandleFastTransform();
}

}  // namespace rst::link

extern "C" {
RST_HOOK bool rst_link_ShouldUseZoraFastSwim() {
  return rst::link::ShouldUseZoraFastSwim();
}

RST_HOOK bool rst_link_ShouldEndGoronRoll(game::act::Player* player) {
  return rst::link::ShouldEndGoronRoll(player);
}

RST_HOOK bool rst_link_SwitchToZoraFastSwim(game::GlobalContext* gctx, game::act::Player* player,
                                            bool check_magic) {
  return rst::link::SwitchToZoraFastSwim(gctx, player, check_magic);
}
}
