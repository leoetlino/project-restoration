#include "game/player.h"

#include "common/utils.h"
#include "game/actors/obj_elegy_statue.h"
#include "game/common_data.h"
#include "rst/link.h"

namespace game::act {

namespace {
static void PlayerChangeStateToStill(Player* player, GlobalContext* gctx) {
  constexpr float speed_maybe = -6.0;
  rst::util::GetPointer<void(Player*, GlobalContext*, float)>(0x1E6500)(player, gctx, speed_maybe);
}
}  // namespace

FormParam& GetFormParam(FormParamIndex idx) {
  return rst::util::GetPointer<FormParam>(0x7AE9E8)[u8(idx) % 8];
}

Player::ArrowInfo Player::GetArrowInfo(GlobalContext* gctx) const {
  ArrowInfo info;

  if (current_action == Action::DekuBubble) {
    info.item_id = ItemId::DekuNuts;
    info.actor_param = active_form == Player::Form::Deku ? 7 : 6;
  } else {
    info.item_id = ItemId::Arrow;
    // Extension: Nintendo directly uses current_action here.
    info.actor_param = u8(rst::link::GetFastArrowAction().value_or(current_action)) - 7;
    if (flags1.IsSet(Player::Flag1::Riding))
      info.actor_param = 1;
  }

  const CommonData& cdata = GetCommonData();

  if (active_form == Player::Form::Deku) {
    // Playing the Honey and Darling shooting minigame as Deku Link.
    info.can_use = cdata.save.player.magic >= 2 ||
                   ((cdata.save.anonymous_72 & 1) && gctx->scene == SceneId::HoneyAndDarling);
  } else {
    info.can_use = flags3.IsSet(Flag3::DekuStuffMaybe) ||
                   (cdata.field_3696 == 1 && gctx->hud_state.field_244) || gctx->field_C531 ||
                   rst::util::GetPointer<bool(ItemId)>(0x224EF8)(info.item_id);
  }

  return info;
}

bool Player::SetTalkActor(u8 new_fn1_idx, Actor* actor) {
  if (fn1_idx == 5)
    return false;
  if (new_fn1_idx == 6 && fn1_idx == 0)
    return false;
  fn1_idx = new_fn1_idx;
  talk_actor = actor;
  talking = 1;
  return true;
}

bool PlayerUpdateMagicCost(game::GlobalContext* gctx, int cost, int mode,
                           AllowExistingMagicUsage allow_existing_usage) {
  return rst::util::GetPointer<bool(game::GlobalContext*, int, int, bool)>(0x2264CC)(
      gctx, cost, mode, allow_existing_usage == AllowExistingMagicUsage::Yes);
}

RST_HOOK void PlayerStateSpawningElegyStatue(Player* player, GlobalContext* gctx) {
  auto& pad = gctx->pad_state;
  player->controller_info.state = &pad;

  ++player->timer;

  // Spawn the statue as soon as possible.
  if (player->timer == 1) {
    auto spawn_elegy_statue = rst::util::GetPointer<void(GlobalContext*, Player*)>(0x1F0758);
    spawn_elegy_statue(gctx, player);
    auto* statue = gctx->elegy_statues[u8(player->active_form)];
    statue->timer = 0;
  } else if (player->timer > 5) {
    auto* statue = gctx->elegy_statues[u8(player->active_form)];
    const bool statue_ready = !statue || statue->pos.pos == player->pos.pos;
    if (player->timer > 135 || statue_ready) {
      gctx->ocarina_state = OcarinaState::StoppedPlaying;
      PlayerChangeStateToStill(player, gctx);
    } else if (statue && !statue_ready) {
      // Speed up the statue fadeout. (0x18 + 8 = 0x20 per game tick)
      statue->opacity = std::max(int(statue->opacity) - 0x18, 0);
    }
  }
}

RST_HOOK bool PlayerGetArrowInfo(GlobalContext* gctx, Player* player, ItemId* item_id,
                                 int* actor_param) {
  const Player::ArrowInfo info = player->GetArrowInfo(gctx);
  *item_id = info.item_id;
  *actor_param = info.actor_param;
  return info.can_use;
}

}  // namespace game::act
