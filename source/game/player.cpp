#include "game/player.h"

#include "common/utils.h"
#include "game/common_data.h"
#include "rst/link.h"

namespace game::act {

FormParam& GetFormParam(FormParamIndex idx) {
  return rst::util::GetPointer<FormParam>(0x7AE9E8, 0x7AF9E8)[u8(idx) % 8];
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
    if (flags1.IsSet(Player::Flag1::Unk800000))
      info.actor_param = 1;
  }

  const CommonData& cdata = GetCommonData();

  if (active_form == Player::Form::Deku) {
    info.can_use =
        cdata.save.player.magic >= 2 || ((cdata.save.anonymous_72 & 1) && gctx->map_maybe == 0x11);
  } else {
    info.can_use = flags3.IsSet(Flag3::DekuStuffMaybe) ||
                   (cdata.field_3696 == 1 && gctx->field_866C) || gctx->field_C531 ||
                   rst::util::GetPointer<bool(ItemId)>(0x224EF8)(info.item_id);
  }

  return info;
}

bool PlayerUpdateMagicCost(game::GlobalContext* gctx, int cost, int mode,
                           AllowExistingMagicUsage allow_existing_usage) {
  return rst::util::GetPointer<bool(game::GlobalContext*, int, int, bool)>(0x2264CC)(
      gctx, cost, mode, allow_existing_usage == AllowExistingMagicUsage::Yes);
}

extern "C" {
RST_HOOK bool rst_PlayerGetArrowInfo(game::GlobalContext* gctx, Player* player, ItemId* item_id,
                                     int* actor_param) {
  const Player::ArrowInfo info = player->GetArrowInfo(gctx);
  *item_id = info.item_id;
  *actor_param = info.actor_param;
  return info.can_use;
}
}

}  // namespace game::act
