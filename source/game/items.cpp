#include "game/items.h"

#include <algorithm>

#include "common/context.h"
#include "common/utils.h"
#include "game/actors/boss_twinmold.h"
#include "game/common_data.h"

namespace game {

Action GetActionForItem(ItemId item) {
  // 0x68F800 in Joker 1.0
  static constexpr Action table[] = {
      Action::Ocarina,   Action::Arrow,         Action::FireArrow,
      Action::IceArrow,  Action::LightArrow,    Action::PictographBox,
      Action(0xE),       Action(0x10),          Action(0x7),
      Action(0x12),      Action(0x30),          Action::PictographBox,
      Action(0xF),       Action::PictographBox, Action(0x54),
      Action(0xD),       Action(0x6),           Action::PictographBox,
      Action(0x15),      Action(0x23),          Action(0x25),
      Action(0x24),      Action(0x29),          Action(0x1A),
      Action(0x26),      Action(0x27),          Action(0x16),
      Action(0x20),      Action(0x20),          Action(0x21),
      Action(0x22),      Action(0x17),          Action(0x18),
      Action(0x19),      Action(0x1B),          Action(0x1E),
      Action(0x1D),      Action(0x28),          Action(0x2A),
      Action(0x2B),      Action(0x2C),          Action(0x2D),
      Action(0x31),      Action(0x32),          Action(0x33),
      Action(0x2E),      Action(0x35),          Action(0x2F),
      Action(0x38),      Action(0x3A),          Action::DekuMask,
      Action::GoronMask, Action::ZoraMask,      Action::FierceDeityMask,
      Action(0x3C),      Action(0x3D),          Action(0x3E),
      Action(0x3F),      Action(0x40),          Action(0x41),
      Action(0x42),      Action(0x43),          Action(0x44),
      Action(0x45),      Action(0x46),          Action(0x47),
      Action(0x48),      Action(0x49),          Action(0x4A),
      Action(0x4B),      Action(0x4C),          Action(0x4D),
      Action(0x4E),      Action::GiantMask,     Action::FireArrow,
      Action::IceArrow,  Action::LightArrow,    Action(0x3),
      Action(0x4),       Action(0x5),           Action(0x6),
      Action::None,      Action::None,          Action::None,
  };
  if (u8(item) >= std::size(table))
    return Action(0xffffffff);
  return table[u8(item)];
}

bool HasOcarina() {
  const auto& items = GetCommonData().save.inventory.items;
  return items[0] == ItemId::Ocarina;
}

bool HasItem(ItemId item_id) {
  const auto& items = GetCommonData().save.inventory.items;
  return std::any_of(items.begin(), items.end(), [&](ItemId id) { return item_id == id; });
}

bool HasMask(ItemId item_id) {
  const auto& masks = GetCommonData().save.inventory.masks;
  return std::any_of(masks.begin(), masks.end(), [&](ItemId id) { return item_id == id; });
}

bool CanUseItem(ItemId item_id) {
  if (item_id == ItemId::Ocarina) {
    return GetCommonData().usable_btns[u8(UsableButton::Ocarina)] != ButtonIsUsable::No;
  }
  if (item_id == ItemId::PictographBox) {
    return GetCommonData().usable_btns[u8(UsableButton::PictographBox)] != ButtonIsUsable::No;
  }

  auto* gctx = rst::GetContext().gctx;
  if (!gctx)
    return false;

  // If the player is using the pictobox, return false since most items (other than the ocarina)
  // cannot be used and because the game won't update item usability status properly.
  auto* player = gctx->GetPlayerActor();
  if (!player)
    return false;
  if (player->action_type == act::Player::ActionType::Type2)
    return false;

  // Work around a game bug that causes the player to be able to use items
  // before the Twinmold intro cutscene starts and softlock the game.
  const auto* twinmold = gctx->FindActorWithId<game::act::BossTwinmold>(game::act::Id::BossTwinmold,
                                                                        game::act::Type::Boss);
  if (twinmold && twinmold->status == game::act::BossTwinmold::Status::Inactive) {
    return false;
  }

  // This is a terrible hack to use the official item usability logic.
  // Assign the item to an action button, then update item usability by calling
  // a MM3D function, then restore the original assignment.

  FormEquipmentData& equipment = GetCommonData().save.equipment.data[0];
  const FormEquipmentData equipment_copy = equipment;

  equipment.item_btn_i = item_id;
  const auto update_usability = rst::util::GetPointer<void(GlobalContext*)>(0x1884B0);
  update_usability(gctx);

  const bool is_usable = GetCommonData().usable_btns[u8(UsableButton::I)] != ButtonIsUsable::No;

  equipment = equipment_copy;
  update_usability(gctx);
  return is_usable;
}

}  // namespace game
