#include "rst/fixes/boss.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>

#include "common/context.h"
#include "common/debug.h"
#include "common/types.h"
#include "game/actor.h"
#include "game/actors/boss_goht.h"
#include "game/actors/boss_gyorg.h"
#include "game/actors/boss_odolwa.h"
#include "game/actors/boss_twinmold.h"
#include "game/as.h"
#include "game/collision.h"
#include "game/context.h"
#include "game/random.h"
#include "game/sound.h"

namespace rst {

extern "C" RST_HOOK int rst_OdolwaGetWaitDuration(game::act::BossOdolwa* odolwa) {
  constexpr auto calc_intro = util::GetAddr(0x308484);
  if (uintptr_t(odolwa->odolwa_calc_prev) == calc_intro)
    return 90;
  const bool is_phase_1 = odolwa->call_bug_counter == 0;
  const int delay = game::RandomFloat() <= 0.25f ? 15 : 0;
  return delay + (is_phase_1 ? 60 : 15);
}

extern "C" RST_HOOK int rst_OdolwaGetChargeDuration(game::act::BossOdolwa* odolwa) {
  if (odolwa->command == u8(game::act::BossOdolwa::ChargeCommand::DanceThrust)) {
    const float probability_long_charge = odolwa->call_bug_counter == 0 ? 0.9 : 0.25;
    return game::RandomFloat() <= probability_long_charge ? 88 : 0;
  }
  return 2 * static_cast<int>(odolwa->anim_duration * 1.5f + 0.5f);
}

extern "C" RST_HOOK bool rst_OdolwaShouldMoveToPhase2(game::act::BossOdolwa* odolwa) {
  if (odolwa->call_bug_counter)
    return false;
  return odolwa->call_bug_timer > 1500 || odolwa->life <= 10;
}

extern "C" RST_HOOK bool rst_OdolwaCheckIsInvincible(game::act::BossOdolwa* odolwa) {
  // Nintendo uses an invincibility timer to ensure some attacks such as kicking
  // don't deal too much damage and send Link flying.
  // The problem is that this system also prevents landing attacks whenever Odolwa is kicking...
  // The workaround is to check whether an attack has landed in addition to checking the timer.
  static bool s_collided_last_time = false;
  if (odolwa->invincibility_timer == 0)
    return false;
  const bool collided = std::any_of(odolwa->collision, odolwa->collision + 11, game::IsCollided);
  s_collided_last_time = collided;
  return !collided && !s_collided_last_time;
}

void FixOdolwa() {
  auto* gctx = GetContext().gctx;
  auto* boss = gctx->FindActorWithId<game::act::BossOdolwa>(game::act::Id::BossOdolwa,
                                                            game::act::Type::Boss);
  if (!boss)
    return;

  if (boss->intro_state == 3) {
    // Re-implement the SFX and BGM triggers.
    if (boss->intro_timer == 0) {
      const auto play_boss_bgm = util::GetPointer<void(game::sound::StreamId)>(0x15D198);
      play_boss_bgm(game::sound::StreamId::NA_BGM_BOSS00);
    }
    // Skip the eye opening part of the intro animation.
    if (boss->intro_timer == 2) {
      boss->actor_util.state.position = 32.666653;
      boss->intro_timer = 49;
    }
    if (boss->intro_timer == 60 || boss->intro_timer == 114 || boss->intro_timer == 176)
      game::sound::PlayEffect(*boss, game::sound::EffectId::NA_SE_EN_MIBOSS_SWORD);
    return;
  }

  const auto calc_preparing = util::GetAddr(0x24FE24);
  const auto calc_waiting = util::GetAddr(0x5725E4);
  const auto current_calc = uintptr_t(boss->odolwa_calc);
  const bool preparing_charge =
      current_calc == calc_preparing &&
      boss->command == u8(game::act::BossOdolwa::ChargeCommand::DanceThrust);
  if (preparing_charge || current_calc == calc_waiting) {
    if (boss->distance_to_link <= 130.0f) {
      const auto jump_away =
          util::GetPointer<void(game::act::BossOdolwa*, game::GlobalContext*, int)>(0x2FE074);
      jump_away(boss, gctx, 2);
    }
  }
}

extern "C" RST_HOOK bool rst_IsGohtCollided(game::act::BossGoht* goht) {
  if (!goht->goht_flags.IsSet(game::act::BossGoht::Flag::FinishedPlayingStunCutsceneOnce) ||
      goht->field_3998) {
    return false;
  }
  return std::any_of(goht->collision, goht->collision + 0x12, game::IsCollided);
}

void FixGoht() {
  const game::GlobalContext* gctx = GetContext().gctx;
  auto* goht =
      gctx->FindActorWithId<game::act::BossGoht>(game::act::Id::BossGoht, game::act::Type::Boss);
  if (!goht)
    return;

  // Prevent the eye from popping out.
  goht->eye_scale = {0.8, 0.8, 0.05};
  // Disable the "eye pops out" cutscene.
  goht->goht_flags.Set(game::act::BossGoht::Flag::FinishedPlayingStunCutsceneOnce);
}

extern "C" RST_HOOK int rst_GetGyorgCollisionResponse(game::act::BossGyorg* gyorg) {
  const auto calc_fn = reinterpret_cast<uintptr_t>(gyorg->gyorg_calc);
  constexpr uintptr_t stunned = util::GetAddr(0x2802C8);
  constexpr uintptr_t stunned_2 = util::GetAddr(0x5560C0);
  constexpr uintptr_t stunned_attacked = util::GetAddr(0x27AAD0);
  return util::Contains(std::array{stunned, stunned_2, stunned_attacked}, calc_fn) ? 2 : 1;
}

void FixGyorg() {
  const auto* gctx = GetContext().gctx;
  auto* gyorg =
      gctx->FindActorWithId<game::act::BossGyorg>(game::act::Id::BossGyorg, game::act::Type::Boss);
  if (!gyorg)
    return;
  gyorg->eyeball_flags = 0;
  gyorg->eyeball_scale = {0.0, 0.0, 0.0};
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
    // if it weren't for the fact that Red Twinmold regularly burrows back into sand during phase
    // 2 and the hit counter is reset every time that happens. This makes for a confusing
    // experience the first time the player fights Twinmold, as there is nothing in the game that
    // indicates that the hit counter resets every time (and it's still frustrating on subsequent
    // playthroughs).
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

void FixBosses() {
  FixOdolwa();
  FixGoht();
  FixGyorg();
  FixTwinmold();
}

}  // namespace rst
