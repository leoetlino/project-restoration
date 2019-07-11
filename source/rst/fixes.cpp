#include "rst/fixes.h"

#include <optional>

#include "common/context.h"
#include "common/debug.h"
#include "common/utils.h"
#include "game/actor.h"
#include "game/actors/boss_twinmold.h"
#include "game/actors/npc_bombers.h"
#include "game/common_data.h"
#include "game/context.h"

namespace rst {

void FixTime() {
  game::CommonData& cdata = game::GetCommonData();

  // Restore the effectiveness of the Inverted Song of Time.
  //
  // In MM, the normal time speed is +3 and the ISoT sets the extra time speed to -2, resulting
  // in a +1 effective time speed (which means 1/3 time speed).
  //
  // In MM3D, the normal speed is +2 and the ISoT only sets the extra speed to -1, which still
  // gives the player a +1 effective speed, but only 1/2 time speed.
  //
  // A quick fix is to skip incrementing the in-game time every 6 frames,
  // giving us the desired ratio of 2/6.

  if (cdata.save.extra_time_speed == -2)
    cdata.save.extra_time_speed = -1;
  if (cdata.save.extra_time_speed == -1 && GetContext().gctx->frame_counter % 6 == 0)
    cdata.save.extra_time_speed = -2;
}

struct TwinmoldFixState {
  s8 blue_prev_life;
  s8 red_prev_life;
  game::act::BossTwinmold::Status red_prev_status;
  u16 red_prev_hit_counter;
  bool is_hit_counter_sane;
};

void FixTwinmold() {
  static std::optional<TwinmoldFixState> state{};
  const game::GlobalContext* gctx = GetContext().gctx;

  auto* red_twinmold = gctx->FindActorWithId<game::act::BossTwinmold>(game::act::Id::BossTwinmold,
                                                                      game::act::Type::Boss);
  if (!red_twinmold) {
    state.reset();
    return;
  }

  auto* blue_twinmold = red_twinmold->other_twinmold_actor;

  if (state) {
    // Red Twinmold has 12 HP (after killing their blue friend).
    //
    // Spinning it deals 3-5 damage points based on lin_vel_xxx:
    //     boss->life -= 3 + (5 - 2) * player->lin_vel_xxx;
    // It is possible to deal 5 damage to the boss by spinning the main stick,
    // but that's not obvious at all...
    //
    // If the player spins the main stick (which is not an obvious thing to do...),
    // killing Red Twinmold takes 3 identical cycles.
    // If not, 4 cycles (!) are required.
    //
    // Let's make that less tedious and less boring by reducing the number of required cycles
    // (1 if the player touches the stick, 2 otherwise).
    if (state->red_prev_life > red_twinmold->life) {
      util::Print("%s: dealing more damage to Red Twinmold", __func__);
      red_twinmold->life -= 8;
    }

    // Only update the hit counter if it is sane. One way of ensuring that condition is satisfied
    // is to only consider the counter to be sane after the player has hit Twinmold once.
    if (red_twinmold->hit_counter == 8)
      state->is_hit_counter_sane = true;

    // 10 hits are required to stun Red or Blue Twinmold. This would have been acceptable
    // if it weren't for the fact that Red Twinmold regularly burrows back into sand during phase 2
    // and the hit counter is reset every time that happens.
    // This makes for a confusing experience the first time the player fights Twinmold,
    // as there is nothing in the game that indicates that the hit counter resets every time
    // (and it's still frustrating on subsequent playthroughs).
    //
    // Fix that by restoring the previous hit counter after it's been reset by the game.
    const bool was_reset = red_twinmold->hit_counter == 9 && state->red_prev_hit_counter != 9;
    const bool is_legit_reset = red_twinmold->status == game::act::BossTwinmold::Status::Stunned;
    if (state->is_hit_counter_sane && was_reset && !is_legit_reset) {
      util::Print("%s: restoring hit counter (%u)", __func__, state->red_prev_hit_counter);
      red_twinmold->hit_counter = state->red_prev_hit_counter;
    }
  } else {
    util::Print("%s: initialising state", __func__);
    state.emplace();
    state->is_hit_counter_sane = false;
  }

  state->blue_prev_life = blue_twinmold->life;
  state->red_prev_life = red_twinmold->life;
  state->red_prev_status = red_twinmold->status;
  if (state->is_hit_counter_sane)
    state->red_prev_hit_counter = red_twinmold->hit_counter;
}

void FixIceArrows() {
  const auto* gctx = GetContext().gctx;
  game::act::Actor* actor = gctx->actors.lists[u8(game::act::Type::Prop)].first;
  while (actor) {
    // Hide the new sparkle actors by moving them.
    if (actor->id == game::act::Id::BgSeaFreezablePoint) {
      actor->position.y = -100000.0;
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
      gctx->GetPlayerActor()->position.Distance(npc_rosa_sister->position) <= 200;

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

}  // namespace rst
