#include "rst/fixes.h"

#include <optional>

#include "common/context.h"
#include "common/debug.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/actors/npc_bombers.h"
#include "game/actors/obj_owl_statue.h"
#include "game/common_data.h"
#include "game/context.h"
#include "game/player.h"

namespace rst {

void FixIceArrows() {
  const auto* gctx = GetContext().gctx;
  game::act::Actor* actor = gctx->actors.lists[u8(game::act::Type::Prop)].first;
  while (actor) {
    // Hide the new sparkle actors by moving them.
    if (actor->id == game::act::Id::BgSeaFreezablePoint) {
      actor->pos.pos.y = -100000.0;
    }
    actor = actor->next;
  }

  // The main fix is a code patch that removes the entrance restriction for ice arrows.
}

void FixDekuMovingPlatforms() {
  // The moving platforms in Deku Palace / Woodfall Temple don't actually move
  // until the player stands on them.
  // That pretty much negates the point of having moving platforms...

  const auto& type_1_actors = GetContext().gctx->actors.lists[1];
  for (auto* actor = type_1_actors.first; actor; actor = actor->next) {
    if (actor->id != game::act::Id::ObjRailLift)
      continue;

    const auto calc_fn = util::BitCastPtr<uintptr_t>(actor, 0x20C);
    constexpr uintptr_t waiting_for_player_fn = 0x3113C8;
    constexpr uintptr_t moving_fn = 0x58F1F4;
    // Make any paused platform move automatically.
    if (calc_fn == waiting_for_player_fn)
      *(uintptr_t*)((u8*)actor + 0x20C) = moving_fn;
  }
}

// Fix bombers being too enthusiastic and distracting the player even during cutscenes.
void FixBombers() {
  static std::array<void*, 5> s_original_data_ptrs{};

  const game::GlobalContext* gctx = GetContext().gctx;

  const auto* npc_rosa_sister =
      gctx->FindActorWithId(game::act::Id::NpcRosaSisters, game::act::Type::Npc);
  const bool is_player_around_rosa_sisters =
      npc_rosa_sister &&
      gctx->GetPlayerActor()->pos.pos.Distance(npc_rosa_sister->pos.pos) <= 200;

  const auto& npcs = gctx->actors.GetList(game::act::Type::Npc);

  bool has_at_least_one_bombers_actor = false;

  for (auto* actor = npcs.first; actor; actor = actor->next) {
    if (actor->id != game::act::Id::NpcBombers)
      continue;

    has_at_least_one_bombers_actor = true;
    auto* npc = static_cast<game::act::NpcBombers*>(actor);

    if (is_player_around_rosa_sisters) {
      // Clear the notebook data pointer, which will make the NPC not chase the player
      // or provide any hints.
      if (!s_original_data_ptrs[npc->number]) {
        util::Print("%s: disabling hints for Bombers %u", __func__, npc->number);
        std::swap(npc->notebook_data, s_original_data_ptrs[npc->number]);
      }

    } else if (s_original_data_ptrs[npc->number] && !npc->notebook_data) {
      // Restore the original pointer.
      util::Print("%s: re-enabling hints for Bombers %u", __func__, npc->number);
      std::swap(npc->notebook_data, s_original_data_ptrs[npc->number]);
    }
  }

  if (!has_at_least_one_bombers_actor)
    s_original_data_ptrs.fill(nullptr);
}

void FixHintStone() {
  namespace act = game::act;
  const game::GlobalContext* gctx = GetContext().gctx;
  act::Actor* hintstone = gctx->FindActorWithId(act::Id::NpcHintStone, act::Type::Prop);
  if (!hintstone)
    return;

  // Disable the "new hint" glowing animation until the player has the Deku Mask
  // to prevent the hint stone from distracting them during important cutscenes
  // in the Clock Tower.
  if (!game::HasMask(game::ItemId::DekuMask)) {
    // moving animation?
    util::Write<float>(hintstone, 0x4AC, 0.0);
    // color animation?
    util::Write<float>(hintstone, 0x4C8, 0.0);
    // color animation?
    util::Write<float>(hintstone, 0x4CC, 0.0);
  }
}

void FixFreeCameraReset() {
  using namespace game;
  auto* gctx = GetContext().gctx;
  auto* player = gctx->GetPlayerActor();

  // Only reset free camera when Z-targeting and when free camera is active
  if (!player)
    return;
  if (gctx->main_camera.mode != CameraMode::FREECAMERA)
    return;
  if (!gctx->pad_state.input.buttons.IsSet(pad::Button::L))
    return;

  if (player->flags3.IsSet(act::Player::Flag3::ZoraFastSwimming)) {
    util::Print("%s: resetting camera mode (Zora swimming)", __func__);
    const bool in_water = player->flags1.IsSet(act::Player::Flag1::InWater);
    gctx->main_camera.ChangeMode(in_water ? CameraMode::GORONDASH : CameraMode::FREEFALL);
  } else if (player->flags3.IsSet(act::Player::Flag3::GoronRolling)) {
    util::Print("%s: resetting camera mode (Goron rolling)", __func__);
    const bool on_ground = player->flags_94.IsSet(act::Actor::Flag94::Grounded);
    gctx->main_camera.ChangeMode(on_ground ? CameraMode::GORONDASH : CameraMode::GORONJUMP);
  }
}

void FixOwlStatueActivationTrigger() {
  using namespace game;
  auto& cdata = GetCommonData();
  auto* gctx = GetContext().gctx;
  auto* owl = gctx->FindActorWithId<act::ObjOwlStatue>(act::Id::ObjOwlStatue, act::Type::Item);
  if (!owl)
    return;

  if (owl->activation_timer > 1) {
    --owl->activation_timer;
  }

  const bool activated = cdata.save.player.owl_statue_flags & (1 << (owl->params & 0xF));
  const bool hit = owl->col_body.info.IsCollided();
  owl->col_body.info.ClearCollided();
  if (!activated && hit && owl->activation_timer == 0) {
    util::Print("%s: scheduling owl statue activation (%04x)", __func__, owl->params);
    owl->activation_timer = 5;
  }
  if (owl->activation_timer == 1) {
    util::Print("%s: activating owl statue (%04x)", __func__, owl->params);
    gctx->Talk(owl);
    owl->activation_timer = -1;
  }
}

}  // namespace rst
